#include <bits/stdc++.h>
#include <direct.h>
#include "MCTS.h"
#include "UtttBoard.h"
#include "NetworkWrapper.h"
#include "TrainingManager.h"


//fix boost bugs on mingw
#ifndef __kernel_entry
#define __kernel_entry
#endif

#include <boost/process.hpp>

namespace bp = boost::process; 

using namespace std;

//g++ -O2 -std=c++14 arbiter.cpp UtttBoard.cpp NetworkWrapper.cpp TrainingManager.cpp MCTS.cpp -L"C:\Users\farmersrice\AppData\Local\Programs\Python\Python37\libs" -lpython37 -I"C:\Users\farmersrice\AppData\Local\Programs\Python\Python37\include" -o arbiter -I"C:\Users\farmersrice\Documents\boost_1_72_0" -L"C:\Users\farmersrice\Documents\boost\lib" -lws2_32 -lboost_filesystem-mgw81-mt-x64-1_72 -lboost_system-mgw81-mt-x64-1_72

int pairToSingle(int row, int col) {
	int bigrow = row / 3;
	int bigcol = col / 3;

	int big = 9 * (bigrow * 3 + bigcol);

	int smallrow = row % 3;
	int smallcol = col % 3;

	int small = smallrow * 3 + smallcol;

	return big + small;
}

pair<int, int> singleToPair(int move) {
	int big = move / 9;
	int small = move % 9;

	int humanRow = (3 * (big / 3)) + (small / 3);
	int humanCol = (3 * (big % 3)) + (small % 3);

	return make_pair(humanRow, humanCol);
}

int main() {
	ios_base::sync_with_stdio(false); 
	cin.tie(NULL);

	int numGames;
	cout << "Number of games to play: " << endl;
	cin >> numGames;

	string firstPath;
	cout << "Path to first executable: " << endl;
	cin >> firstPath;

	string secondPath;
	cout << "Path to second executable: " << endl;
	cin >> secondPath;

	
	int firstWins = 0;
	int draws = 0;
	int secondWins = 0;

	double firstScore = 0;
	double secondScore = 0;

	for (int i = 0; i < numGames; i++) {
		UtttBoard b;

		cout << "Starting new game " << i << endl;
		bp::opstream firstIn, secondIn; //putting stuff into the executables
		bp::ipstream firstOut, secondOut; //reading executable outputs

		bp::child firstChild(firstPath, bp::std_out > firstOut, bp::std_in < firstIn);
		bp::child secondChild(secondPath, bp::std_out > secondOut, bp::std_in < secondIn);

		cout << "Processes created " << endl;

		if (i < numGames / 2) {
			//firstChild goes first

			firstIn << -1 << ' ' << -1 << endl;

			int previousMoveRow, previousMoveCol;
			firstOut >> previousMoveRow >> previousMoveCol;

			cout << previousMoveRow << ' ' << previousMoveCol << endl;

			int previousMove = pairToSingle(previousMoveRow, previousMoveCol);

			b.processMove(previousMove);

			int moveCounter = 1;
			while (!b.getGameEnded()) {
				if (moveCounter % 2) {
					//Second goes
					secondIn << previousMoveRow << ' ' << previousMoveCol << endl;
					secondOut >> previousMoveRow >> previousMoveCol;
				} else {
					//first goes
					firstIn << previousMoveRow << ' ' << previousMoveCol << endl;
					firstOut >> previousMoveRow >> previousMoveCol;
				}
				cout << previousMoveRow << ' ' << previousMoveCol << endl;
				moveCounter++;
				previousMove = pairToSingle(previousMoveRow, previousMoveCol);
				b.processMove(previousMove);
			}

			int result = b.getWinResult();

			if (result == 0) {
				draws++;
				firstScore += 0.5;
				secondScore += 0.5;
				cout << "Draw" << endl;
			} else if (result == 1) {
				firstWins++;
				firstScore++;
				cout << firstPath << " wins " << endl;
			} else {
				secondWins++;
				secondScore++;
				cout << secondPath << " wins " << endl;
			}
		} else {
			//secondChild goes first

			secondIn << -1 << ' ' << -1 << endl;

			int previousMoveRow, previousMoveCol;
			secondOut >> previousMoveRow >> previousMoveCol;
			
			cout << previousMoveRow << ' ' << previousMoveCol << endl;

			int previousMove = pairToSingle(previousMoveRow, previousMoveCol);

			b.processMove(previousMove);

			int moveCounter = 1;
			while (!b.getGameEnded()) {
				if (moveCounter % 2) {
					//first goes
					firstIn << previousMoveRow << ' ' << previousMoveCol << endl;
					firstOut >> previousMoveRow >> previousMoveCol;
				} else {
					//Second goes
					secondIn << previousMoveRow << ' ' << previousMoveCol << endl;
					secondOut >> previousMoveRow >> previousMoveCol;
				}
				cout << previousMoveRow << ' ' << previousMoveCol << endl;
				moveCounter++;
				previousMove = pairToSingle(previousMoveRow, previousMoveCol);
				b.processMove(previousMove);
			}

			int result = b.getWinResult();

			if (result == 0) {
				draws++;
				firstScore += 0.5;
				secondScore += 0.5;
				cout << "Draw" << endl;
			} else if (result == 2) {
				firstWins++;
				firstScore++;
				cout << firstPath << " wins " << endl;
			} else {
				secondWins++;
				secondScore++;
				cout << secondPath << " wins " << endl;
			}
		}


		firstChild.terminate();
		secondChild.terminate();
	}

	cout << "Result of " << firstPath << " vs " << secondPath << endl;
	cout << "Finished! Counts are " << firstWins << ' ' << draws << ' ' << secondWins << " (firstWins/draws/secondWins), points are " << firstScore << ' ' << secondScore << endl;
	
}