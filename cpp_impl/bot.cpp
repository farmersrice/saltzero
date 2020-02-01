#include <bits/stdc++.h>
#include <direct.h>
#include "MCTS.h"
#include "UtttBoard.h"
#include "NetworkWrapper.h"
#include "TrainingManager.h"

using namespace std;

//g++ -O2 -std=c++14 bot.cpp UtttBoard.cpp NetworkWrapper.cpp TrainingManager.cpp MCTS.cpp -L"C:\Users\farmersrice\AppData\Local\Programs\Python\Python37\libs" -lpython37 -I"C:\Users\farmersrice\AppData\Local\Programs\Python\Python37\include" -o bot -I"C:\Users\farmersrice\Documents\boost_1_72_0" -lws2_32

int getBestMove(MCTS &mctsItem, UtttBoard &b, NetworkWrapper &net) {

	//Make our move
	auto start = clock();

	for (int asdf = 0; asdf < 400; asdf++) {

		bool noInput = false;

		UtttBoard query = mctsItem.beforeVisit(b, false);
		UtttBoard queryBoard;

		if (query.invalid) {
			noInput = true;
		} else {
			queryBoard = query;
		}

		pair<int, int> rotationHistory;

		uniform_int_distribution<int> dis(0, 3);

		//Perform random rotations	

		rotationHistory = make_pair(dis(rng) % 2, dis(rng));

		if (rotationHistory.first) {
			//Flip it
			queryBoard.reflect();
		}

		for (int i = 0; i < rotationHistory.second; i++) {
			queryBoard.rotate90();
		}

		vector<vector<float>> batchedQueries;
		batchedQueries.push_back(queryBoard.toNetInputVector());

		vector<pair<vector<float>, float>> results;

		if (batchedQueries.size() > 0) {
			// 0 is always loaded as the best net
			//auto s1 = clock();
			results = net.predict(batchedQueries, 0);
			//cout << "predict time " << (clock() - s1) / (1.0 * CLOCKS_PER_SEC ) << endl;
		}

		pair<vector<float>, float> result = results[0];

		//Unrotate the results

		if (rotationHistory.second > 0) {
			for (int i = 0; i < 4 - rotationHistory.second; i++) {
				result.first = rotateMatrix90<float>(result.first);
			}
		}

		if (rotationHistory.first) {
			result.first = reflectMatrix<float>(result.first);
		}
		
		if (noInput) {
			mctsItem.afterVisit(b, vector<float>(), 0.0f);
		} else {
			mctsItem.afterVisit(b, result.first, result.second);
		}
	}


	pair<vector<float>, int> piAndMove = mctsItem.getProbabilitiesAndBestMove(b, 0);

	mctsItem.pruneRecursive(b, piAndMove.second);

	//cout << "Moved " << piAndMove.second << endl;
	//cout << "Time for move: " << (clock() - start) / (1.0 * CLOCKS_PER_SEC) << endl;

	return piAndMove.second;

}

int main() {
	ios_base::sync_with_stdio(false); 
	cin.tie(NULL);

	MCTS mctsItem;
	UtttBoard b;
	NetworkWrapper net;
	net.loadBestNetwork(0);

	while (!b.getGameEnded()) {

		//Get inputs

		int row, col;
		cin >> row >> col;

		if (row != -1) {
			
			int bigrow = row / 3;
			int bigcol = col / 3;

			int big = 9 * (bigrow * 3 + bigcol);

			int smallrow = row % 3;
			int smallcol = col % 3;

			int small = smallrow * 3 + smallcol;

			int opponentMove = big + small;


			mctsItem.pruneRecursive(b, opponentMove);

			b.processMove(opponentMove);
		}

		int bestMove = getBestMove(mctsItem, b, net);

		int big = bestMove / 9;
		int small = bestMove % 9;

		int humanRow = (3 * (big / 3)) + (small / 3);
		int humanCol = (3 * (big % 3)) + (small % 3);

		cout << humanRow << ' ' << humanCol << endl;

		b.processMove(bestMove);
	}

	
}