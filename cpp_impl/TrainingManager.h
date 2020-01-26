#pragma once
#include <vector>
#include <tuple>
#include "NetworkWrapper.h"

const int TEMPERATURE_THRESHOLD = 12;
const int DUEL_TEMPERATURE_THRESHOLD = 0; //Strong as we can for duel/gating games, randomness is introduced thru rotations
const int TRAIN_GAME_SIZE = 500; //at least 500 games before training new net
const int DUEL_GAME_COUNT = 400; //how many games we use for gating
const int GATING_THRESHOLD = 210;
const int PLAY_BATCH_SIZE = 1000;


struct TrainingManager {
	NetworkWrapper net;
	int storedBest;

	TrainingManager();

	// first is training data, second is result (first pair of second is scores, second is num draws)
	std::pair<std::vector<std::tuple<std::vector<float>, std::vector<float>, int>>, std::pair<std::pair<double, double>, int>> playGames(int numGames, bool isTraining, int flip);

	void saveData(std::vector<std::tuple<std::vector<float>, std::vector<float>, int>> data, int numGames);
	std::pair<std::pair<double, double>, int> compareTwoNetworks(int firstPlayer, int secondPlayer);
	void compareBestToLatest();
};
