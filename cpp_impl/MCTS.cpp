#include "MCTS.h"
#include <cmath>
#include <random>
#include <chrono>
#include <iostream>
using namespace __gnu_pbds;
using namespace std;

typedef long long ll;

const ll RANDOM = chrono::high_resolution_clock::now().time_since_epoch().count();

mt19937 rng(RANDOM);

vector<double> dirichlet(int length) {
    gamma_distribution<double> gamma(ALPHA, 1.0f);

    vector<double> answer;
    double totalSum = 0.0f;
    for (int i = 0; i < length; i++) {
        answer.push_back(gamma(rng));
        totalSum += answer[i];
    }

    if (totalSum < 0) return answer;

    for (int i = 0; i < length; i++) {
        answer[i] /= totalSum;
    }

    return answer;
}

//This is the equivalent of the BatchedMCTS

UtttBoard MCTS::visitDown(UtttBoard board, bool useDirichletNoise) {
    ll boardHash = board.hash();

    if (gameEndedCache.find(boardHash) == gameEndedCache.end()) {
        gameEndedCache[boardHash] = make_pair(board.getGameEnded(), board.getWinResult());
    }

    if (gameEndedCache[boardHash].first) {
        return UtttBoard(true);
    }

    if (P.find(boardHash) != P.end()) {

        vector<int> validMoves = board.getValidMoves();

        vector<double> noise;
        int counter = 0;
        if (useDirichletNoise) {
            int length = 0;
            for (int i = 0; i < 81; i++) {
                if (validMoves[i] == 1) {
                    length++;
                }
            }

            noise = dirichlet(length);
        }

        vector<double> upperConfidenceBounds(81, -100000.0);

        for (int i = 0; i < 81; i++) {
            if (validMoves[i] != 1) continue;

            ll edgeHash = board.hashWithMove(i);

            if (Q.find(edgeHash) == Q.end()) {
                Q[edgeHash] = 0;
                N[edgeHash] = 0;
            }

            double thisP = P[boardHash][i];

            if (useDirichletNoise) {
                thisP = thisP * (1 - EPS) + noise[counter++] * EPS;
            }

            upperConfidenceBounds[i] = Q[edgeHash] + C_PUCT * thisP * sqrt(nodeVisitCount[boardHash]) / (1 + N[edgeHash]);
            //cout << "ucb " << i << " is " << upperConfidenceBounds[i] << " (q is " << Q[edgeHash] << ")" << endl;
        }

        int bestMove = -1;
        double bestResult = -100000.0;

        for (int i = 0; i < 81; i++) {
            if (upperConfidenceBounds[i] > bestResult) {
                bestResult = upperConfidenceBounds[i];
                bestMove = i;
            }
        }

        board.processMove(bestMove);
        dfsOrder.push_back(bestMove);

        return visitDown(board, false);
    }
    
    return board;
}

double MCTS::visitUp(UtttBoard board, vector<float> netpi, float netv, int nextMove) {
    ll boardHash = board.hash();

    if (gameEndedCache.find(boardHash) == gameEndedCache.end()) {
        gameEndedCache[boardHash] = make_pair(board.getGameEnded(), board.getWinResult());
    }

    pair<bool, int> gameEndedState = gameEndedCache[boardHash];
    if (gameEndedState.first) {
        int result = gameEndedState.second;

        if (result == 0) return 0;
        if (result == board.curPlayer) return -1;
        return 1;
    }

    if (nextMove == dfsOrder.size()) {
        // leaf node, give eval

        vector<int> validMoves = board.getValidMoves();
        float total = 0.0f;
        for (int i = 0; i < 81; i++) {
            if (validMoves[i] == 0) {
                netpi[i] = 0;
            }
            total += netpi[i];
        }



        if (total <= 0.0f) {
            cout << "No valid moves predicted" << endl;
            return 0;
        }

        for (int i = 0; i < 81; i++) {
            netpi[i] /= total;
        }

        P[boardHash] = netpi;
        nodeVisitCount[boardHash] = 0;
        return -netv;
    }

    int bestMove = dfsOrder[nextMove];

    ll edgeHash = board.hashWithMove(bestMove);

    board.processMove(bestMove);

    double v = visitUp(board, netpi, netv, nextMove + 1);
    Q[edgeHash] = (N[edgeHash] * Q[edgeHash] + v) / (N[edgeHash] + 1.0);
    N[edgeHash]++;
    //cout << "increased edgeHash count " << edgeHash << endl;
    nodeVisitCount[boardHash]++;

    return -v;
}

UtttBoard MCTS::beforeVisit(UtttBoard board, bool isTraining) {
    dfsOrder.clear();
    return visitDown(board, isTraining);
}

void MCTS::afterVisit(UtttBoard board, vector<float> netpi, float netv) {
    /*
    cout << "dfs ";
    for (auto i : dfsOrder) {
        cout << i << ' ';
    }
    cout << endl;*/
    visitUp(board, netpi, netv);
}

vector<float> MCTS::getProbabilities(UtttBoard board) {
    ll boardHash = board.hash();

    vector<int> validMoves = board.getValidMoves();

    vector<float> localN(81, 0.0f);

    float total = 0.0f;
    for (int i = 0; i < 81; i++) {
        ll edgeHash = board.hashWithMove(i);
        //cout << "checking edgeHash " << edgeHash << endl;

        localN[i] = (N.find(edgeHash) != N.end() ? N[edgeHash] : 0.0f);
        //cout << i << " localN is " << localN[i] << endl;
        total += localN[i];
    }

    for (int i = 0; i < 81; i++) {
        localN[i] /= total;
    }

    return localN;
}

pair<vector<float>, int> MCTS::getProbabilitiesAndBestMove(UtttBoard board, int temperature) {
    vector<float> probabilities = getProbabilities(board);

    if (temperature == 1) {
        discrete_distribution<int> dis(probabilities.begin(), probabilities.end());

        int bestMove = dis(rng);

        return make_pair(probabilities, bestMove);
    } else {
        int bestMove = -1;
        float bestResult = 0.0f;

        for (int i = 0; i < 81; i++) {
            if (probabilities[i] > bestResult) {
                bestResult = probabilities[i];
                bestMove = i;
            }
        }

        return make_pair(probabilities, bestMove);
    }
}

void MCTS::pruneRecursive(UtttBoard board, int saveEdge) { //saveEdge tells us which edge NOT to traverse (since we'll be resuming from there)
    //Erase edges and recurse
    for (int i = 0; i < 81; i++) {
        ll edgeHash = board.hashWithMove(i);

        if (Q.find(edgeHash) != Q.end()) {
            Q.erase(edgeHash);
            N.erase(edgeHash);

            //cout << "Pruned edge " << edgeHash << endl;
            UtttBoard nextBoard = board;
            nextBoard.processMove(i);
            if (i != saveEdge) pruneRecursive(nextBoard, -1);
        }
    }

    //Erase current node
    ll boardHash = board.hash();

    if (P.find(boardHash) != P.end()) {
        P.erase(boardHash);
        //cout << "Pruned board " << boardHash << endl;
        gameEndedCache.erase(boardHash);
        nodeVisitCount.erase(boardHash);
    }
}