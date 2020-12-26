#include <bits/stdc++.h>
#include <direct.h>
#include "MCTSNT.h"
#include "UtttBoard.h"
#include "NetworkWrapper.h"

using namespace std;

//g++ -O2 -std=c++14 botNT.cpp UtttBoard.cpp NetworkWrapper.cpp MCTSNT.cpp -L"C:\Users\farmersrice\AppData\Local\Programs\Python\Python37\libs" -lpython37 -I"C:\Users\farmersrice\AppData\Local\Programs\Python\Python37\include" -o botNT -I"C:\Users\farmersrice\Documents\boost_1_72_0" -lws2_32

const int TURN_TIME_MS = 800;


int getBestMove(MCTSNT &mctsItem, UtttBoard &b, NetworkWrapper &net) {

	//Make our move
	auto start = chrono::high_resolution_clock::now();

	int sims = 0;

	while (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() < TURN_TIME_MS) {

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

		sims++;
	}

	//cout << "Sims made: " << sims << endl;

	pair<vector<float>, int> piAndMove = mctsItem.getProbabilitiesAndBestMove(b, 0);


	//cout << "Moved " << piAndMove.second << endl;
	//cout << "Time for move: " << (clock() - start) / (1.0 * CLOCKS_PER_SEC) << endl;

	return piAndMove.second;

}

int main() {
	ios_base::sync_with_stdio(false); 
	cin.tie(NULL);

	//MCTSNT mctsItem;
	UtttBoard b;
	NetworkWrapper net;
	net.loadBestNetwork(0);

	// Run a prediction first
	vector<vector<float>> batchedQueries;
	batchedQueries.push_back(b.toNetInputVector());
	net.predict(batchedQueries, 0);

	while (!b.getGameEnded()) {

		MCTSNT mctsItem;

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