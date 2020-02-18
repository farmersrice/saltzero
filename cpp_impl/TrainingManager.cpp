#include "TrainingManager.h"
#include "UtttBoard.h"
#include "MCTS.h"

#include <iostream>
#include <time.h>
#include <Windows.h>
#include <fstream>
#include <iomanip>
#include <random>
#include <chrono>
#include <map>

using namespace std;

TrainingManager::TrainingManager() {
	storedBest = net.getBestNetwork();
	net.loadBestNetwork(0);
}

pair<vector<tuple<vector<float>, vector<float>, int>>, pair<pair<double, double>, int>> TrainingManager::playGames(int numGames, bool isTraining, int flip) {

	vector<UtttBoard> boards(numGames, UtttBoard());

	int moveCounter = 0;

	vector<vector<pair<vector<float>, vector<float>>>> trainingData(numGames, vector<pair<vector<float>, vector<float>>>());

	int unfinishedGames = numGames;

	vector<MCTS> mctsItems(numGames, MCTS());
	vector<MCTS> mctsDuelItems(numGames, MCTS());

	vector<bool> noInput(numGames, false);
	vector<vector<float>> batchedQueries;
	vector<pair<vector<float>, float>> results;

	double result0 = 0;
	double result1 = 0;
	int draws = 0;

	while (unfinishedGames) {
		auto start = clock();

		for (int asdf = 0; asdf < VISITS; asdf++) {
			batchedQueries.clear();
			vector<UtttBoard> queryBoards;

			for (int i = 0; i < numGames; i++) {
				noInput[i] = false;
			}

			for (int i = 0; i < numGames; i++) {
				UtttBoard board = boards[i];

				if (board.getGameEnded()) {
					noInput[i] = true;
					continue;
				}

				MCTS &item = (isTraining ? mctsItems[i] : (moveCounter % 2 == 0 ? mctsItems[i] : mctsDuelItems[i]));

				UtttBoard query = item.beforeVisit(board, isTraining);

				if (query.invalid) {
					noInput[i] = true;
				} else {
					//cout << "adding board to query " << endl;
					//cout << query.toString() << endl;
					//batchedQueries.push_back(query.toNetInputVector());
					queryBoards.push_back(query);
				}
			}

			vector<pair<int, int>> rotationHistory(queryBoards.size());

			uniform_int_distribution<int> dis(0, 3);

			//Perform random rotations	

			for (int i = 0; i < queryBoards.size(); i++) {
				rotationHistory[i] = make_pair(dis(rng) % 2, dis(rng));
				//cout << "rotation pair " << rotationHistory[i].first << ' ' << rotationHistory[i].second << endl;
			}

			for (int i = 0; i < queryBoards.size(); i++) {
				if (rotationHistory[i].first) {
					//Flip it
					queryBoards[i].reflect();
				}

				for (int j = 0; j < rotationHistory[i].second; j++) {
					queryBoards[i].rotate90();
				}
			}

			for (int i = 0; i < queryBoards.size(); i++) {
				batchedQueries.push_back(queryBoards[i].toNetInputVector());
			}

			if (batchedQueries.size() > 0) {
				// 0 is always loaded as the best net
				//auto s1 = clock();
				results = net.predict(batchedQueries, (isTraining ? 0 : (moveCounter % 2 == flip ? 0 : 1)));
				//cout << "predict time " << (clock() - s1) / (1.0 * CLOCKS_PER_SEC ) << endl;
			}

			//Unrotate the results

			for (int i = 0; i < queryBoards.size(); i++) {
				if (rotationHistory[i].second > 0) {
					for (int j = 0; j < 4 - rotationHistory[i].second; j++) {
						results[i].first = rotateMatrix90<float>(results[i].first);
					}
				}

				if (rotationHistory[i].first) {
					results[i].first = reflectMatrix<float>(results[i].first);
				}
			}
			


			int counter = 0;

			for (int i = 0; i < numGames; i++) {
				UtttBoard board = boards[i];

				if (board.getGameEnded()) {
					continue;
				}

				MCTS &item = (isTraining ? mctsItems[i] : (moveCounter % 2 == 0 ? mctsItems[i] : mctsDuelItems[i]));

				if (noInput[i]) {
					item.afterVisit(board, vector<float>(), 0.0f);
				} else {
					//cout << "got predict " << results[counter].second << " from " << vectorToString(batchedQueries[counter]) << endl;
					item.afterVisit(board, results[counter].first, results[counter].second);
					counter++;
				}
			}
		}

		set<vector<float>> duplicateChecker; 
		int duplicates = 0;
		
		//int beforeUnfinished = unfinishedGames;

		for (int i = 0; i < numGames; i++) {
			UtttBoard board = boards[i];

			if (board.getGameEnded()) {
				continue;
			}

			MCTS &item = (isTraining ? mctsItems[i] : (moveCounter % 2 == 0 ? mctsItems[i] : mctsDuelItems[i]));

			pair<vector<float>, int> piAndMove = item.getProbabilitiesAndBestMove(board, ((isTraining && moveCounter < TEMPERATURE_THRESHOLD) || moveCounter < DUEL_TEMPERATURE_THRESHOLD || (moveCounter == 1 && FIRST_TURN_TEMP)) ? 1 : 0);

			auto symmetries = board.getSymmetries(piAndMove.first);

			for (pair<vector<float>, vector<float>> it : symmetries) {
				trainingData[i].push_back(it);
			}

			item.pruneRecursive(board, piAndMove.second);

			//cout << "moved " << piAndMove.second << endl;
			boards[i].processMove(piAndMove.second);

			//cout << "game " << i << " appears " << endl;
			//cout << boards[i].toString() << endl;

			if (boards[i].getGameEnded()) {
				unfinishedGames--;

				if (boards[i].getWinResult() == 0) {
					result0 += 0.5;
					result1 += 0.5;
					draws++;
				} else if (boards[i].getWinResult() == 1) {
					result0 += 1;
				} else {
					result1 += 1;
				}

				
				for (auto it : symmetries) {
					auto finalBoardState = it.first;

					if (duplicateChecker.find(finalBoardState) != duplicateChecker.end()) {
						duplicates++;
					}
				}

				for (auto it : symmetries) {
					auto finalBoardState = it.first;
					duplicateChecker.insert(finalBoardState);
				}
			}
		}

		cout << "finished move " << moveCounter << " in time " << (clock() - start) / (1.0 * CLOCKS_PER_SEC) << endl;

		//int gamesFinished = beforeUnfinished - unfinishedGames;

		//if (gamesFinished != 0) cout << "Finished: " << gamesFinished << endl;
		if (duplicates != 0) cout << "Duplicates: " << duplicates/8 << endl;
		moveCounter++;
	}

	vector<tuple<vector<float>, vector<float>, int>> trainingDataAnswer;

	for (int i = 0; i < numGames; i++) {
		UtttBoard board = boards[i];
		vector<pair<vector<float>, vector<float>>> thisData = trainingData[i];

		int result = board.getWinResult();

		if (result == 2) {
			result = -1;
		}

		int counter = 0;
		for (auto data : thisData) {
			int whoseTurn = (counter / 8) % 2;
			counter++;

			int winResult = (whoseTurn == 0 ? result : -result);

			trainingDataAnswer.push_back(make_tuple(data.first, data.second, winResult));
		}
	}

	return make_pair(trainingDataAnswer, make_pair(make_pair(result0, result1), draws));
}

//if you want to use this in other OSes, you will have to write it yourself

void TrainingManager::saveData(vector<tuple<vector<float>, vector<float>, int>> data, int numGames) {
	int fileCounter = 0;

	CreateDirectory("../archive", NULL);

	string stringPrefix = "../archive/" + to_string(numGames) + "_";
	string fileString = stringPrefix + to_string(fileCounter) + ".cpptactoe";

	while (fileExists(fileString.c_str())) {
		fileCounter++;
		fileString = stringPrefix + to_string(fileCounter) + ".cpptactoe";
	}

	ofstream dataStream;
	dataStream.open(fileString);

	for (auto it : data) {
		for (auto i : get<0>(it)) {
			dataStream << (abs(i - 1.0f) < 0.00001f ? 1 : 0);
		}
		dataStream << ' ';
		for (auto i : get<1>(it)) {
			if (i < 0.00001f) {
				dataStream << "0 ";
			} else {
				dataStream << fixed << setprecision(12) << i << ' ';
			}
		}
		dataStream << get<2>(it) << ' ';
		dataStream << '\n';
	}

	dataStream.close();
}

pair<pair<double, double>, int> TrainingManager::compareTwoNetworks(int firstPlayer, int secondPlayer) {
	cout << "comparing " << firstPlayer << " to " << secondPlayer << endl;
	net.loadNetwork(firstPlayer, 0);
	net.loadNetwork(secondPlayer, 1);
	auto resultBestFirst = playGames(DUEL_GAME_COUNT / 2, false, 0).second;
	cout << "first first got " << resultBestFirst.first.first << ' ' << resultBestFirst.first.second << endl;
	auto resultLatestFirst = playGames(DUEL_GAME_COUNT / 2, false, 1).second;
	cout << "second first got " << resultLatestFirst.first.first << ' ' << resultLatestFirst.first.second << endl;

	double res0 = resultBestFirst.first.first + resultLatestFirst.first.second;
	double res1 = resultBestFirst.first.second + resultLatestFirst.first.first;

	cout << "score is " << res0 << " to " << res1 << " old to new " << endl;

	int draws = resultBestFirst.second + resultLatestFirst.second;
	cout << "total draws " << draws << endl;

	return make_pair(make_pair(res0, res1), draws);
}

void TrainingManager::compareBestToLatest() {
	int curLatest = net.getLatestNetwork();
	cout << "Comparing best to latest, best and latest are " << storedBest << " " << curLatest << endl;
	//net.loadLatestNetwork(1);

	auto result = compareTwoNetworks(storedBest, curLatest);

	double res0 = result.first.first;
	double res1 = result.first.second;
	int draws = result.second;

	ofstream resultStream;
	resultStream.open("selfplay_results.txt", ios::out | ios::app);
	resultStream << storedBest << ' ' << curLatest << " score is " << res0 << " to " << res1 << " draws " << draws << endl;
	resultStream.close();

	if (res1 >= GATING_THRESHOLD) {
		cout << curLatest << " Passed! writing to file" << endl;

		ofstream dataStream;
		dataStream.open("../models/model_info");
		dataStream << curLatest << '\n';
		dataStream.close();

		net.loadNetwork(curLatest, 0); //load the best network, now the latest, into slot 0
		storedBest = curLatest;
	} else {
		cout << "Didn't pass." << endl;
	}
}