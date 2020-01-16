#include "TrainingManager.h"
#include "UtttBoard.h"
#include "MCTS.h"

#include <iostream>
#include <time.h>
#include <Windows.h>
#include <fstream>
#include <iomanip>

using namespace std;

TrainingManager::TrainingManager() {
	storedBest = net.getBestNetwork();
	net.loadBestNetwork(0);
}

pair<vector<tuple<vector<float>, vector<float>, int>>, pair<double, double>> TrainingManager::playGames(int numGames, bool isTraining, int flip) {

	if (storedBest != net.getBestNetwork()) { 
		//Always use the best net
		storedBest = net.getBestNetwork();
		net.loadBestNetwork(0);
	}

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

	while (unfinishedGames) {
		auto start = clock();

		for (int asdf = 0; asdf < VISITS; asdf++) {
			batchedQueries.clear();
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
					batchedQueries.push_back(query.toNetInputVector());
				}
			}

			if (batchedQueries.size() > 0) {
				// 0 is always loaded as the best net
				//auto s1 = clock();
				results = net.predict(batchedQueries, (isTraining ? 0 : (moveCounter % 2 == flip ? 0 : 1)));
				//cout << "predict time " << (clock() - s1) / (1.0 * CLOCKS_PER_SEC ) << endl;
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

		for (int i = 0; i < numGames; i++) {
			UtttBoard board = boards[i];

			if (board.getGameEnded()) {
				continue;
			}

			MCTS &item = (isTraining ? mctsItems[i] : (moveCounter % 2 == 0 ? mctsItems[i] : mctsDuelItems[i]));

			pair<vector<float>, int> piAndMove = item.getProbabilitiesAndBestMove(board, ((isTraining && moveCounter < TEMPERATURE_THRESHOLD) || moveCounter < DUEL_TEMPERATURE_THRESHOLD) ? 1 : 0);

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
				} else if (boards[i].getWinResult() == 1) {
					result0 += 1;
				} else {
					result1 += 1;
				}
			}
		}

		cout << "finished move " << moveCounter << " in time " << (clock() - start) / (1.0 * CLOCKS_PER_SEC) << endl;
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

	return make_pair(trainingDataAnswer, make_pair(result0, result1));
}

//if you want to use this in other OSes, you will have to write it yourself

void TrainingManager::saveData(vector<tuple<vector<float>, vector<float>, int>> data, int numGames) {
	int fileCounter = 0;

	CreateDirectory("../games", NULL);

	//In future, delete excess files
	string stringPrefix = "../games/" + to_string(numGames) + "_";
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

pair<double, double> TrainingManager::compareTwoNetworks(int other) { // for now only take best
	cout << "comparing best to " << other << endl;
	net.loadNetwork(other, 1);
	auto resultBestFirst = playGames(DUEL_GAME_COUNT / 2, false, 0);
	cout << "best first got " << resultBestFirst.second.first << ' ' << resultBestFirst.second.second << endl;
	auto resultLatestFirst = playGames(DUEL_GAME_COUNT / 2, false, 1);
	cout << "latest first got " << resultLatestFirst.second.first << ' ' << resultLatestFirst.second.second << endl;

	double res0 = resultBestFirst.second.first + resultLatestFirst.second.second;
	double res1 = resultBestFirst.second.second + resultLatestFirst.second.first;

	cout << "score is " << res0 << " to " << res1 << " old to new " << endl;

	return make_pair(res0, res1);
}

void TrainingManager::compareBestToLatest() {
	int curLatest = net.getLatestNetwork();
	cout << "Comparing best to latest, best and latest are " << storedBest << " " << curLatest << endl;
	//net.loadLatestNetwork(1);

	auto result = compareTwoNetworks(curLatest);
	/*
	auto resultBestFirst = playGames(DUEL_GAME_COUNT / 2, false, 0);
	cout << "best first got " << resultBestFirst.second.first << ' ' << resultBestFirst.second.second << endl;
	auto resultLatestFirst = playGames(DUEL_GAME_COUNT / 2, false, 1);
	cout << "latest first got " << resultLatestFirst.second.first << ' ' << resultLatestFirst.second.second << endl;

	double res0 = resultBestFirst.second.first + resultLatestFirst.second.second;
	double res1 = resultBestFirst.second.second + resultLatestFirst.second.first;

	cout << "score is " << res0 << " to " << res1 << " old to new " << endl;*/

	double res0 = result.first;
	double res1 = result.second;

	if (res1 > res0) {
		//50% gating

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