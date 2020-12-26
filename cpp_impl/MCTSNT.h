#pragma once
#include <ext/pb_ds/assoc_container.hpp>


#include <vector>
#include <random>

#include "UtttBoard.h"


const int VISITS = 200;
const double C_PUCT = 1.1;
const double ALPHA = 0.3;
const double EPS = 0.25;


extern std::mt19937 rng;

struct MCTSNT { // MCTS no transpositions

	__gnu_pbds::gp_hash_table<long long, int> N, nodeVisitCount;
	__gnu_pbds::gp_hash_table<long long, double> Q;
	__gnu_pbds::gp_hash_table<long long, std::vector<float>> P;
	__gnu_pbds::gp_hash_table<long long, std::pair<bool, int>> gameEndedCache;

	std::vector<int> dfsOrder;


	/*

	You might be wondering: Why is this all pass by value? Isn't that slow? Don't we want speed?
	Well, the thing is, with C++ our bottleneck is finally the neural net (unlike in Python, where that was the fastest part of the code).
	Pass by value here is much simpler to work with and won't cost much. Estimated cost ~1 second per move over 1k games. Whereas the cost for NN is projected as 50s.

	*/
	double visitUp(UtttBoard board, std::vector<float> netpi, float netv, int nextMove = 0, std::vector<int> curOrder = std::vector<int>());
	UtttBoard visitDown(UtttBoard board, bool useDirichletNoise);
	UtttBoard beforeVisit(UtttBoard board, bool isTraining);
	void afterVisit(UtttBoard board, std::vector<float> pi, float v);
	std::vector<float> getProbabilities(UtttBoard board);
	std::pair<std::vector<float>, int> getProbabilitiesAndBestMove(UtttBoard board, int temperature);

};