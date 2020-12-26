#include <bits/stdc++.h>
#include "UtttBoard.h"
#include "NetworkWrapper.h"
#include "TrainingManager.h"

using namespace std;
typedef long long ll;
typedef pair<int, int> pair2;
typedef pair<int, pair<int, int> > pair3;
typedef pair<int, pair<int, pair<int, int> > > pair4;
#define MAXN 200013
#define INF 1000000000000000000LL
#define MOD 1000000007LL
#define IINF 1000000000
#define EPS 0.00001
#define mp make_pair
#define pb push_back
#define remove pop
#define all(x) x.begin(), x.end()

//g++ -O2 -std=gnu++14 play_elo_games.cpp UtttBoard.cpp NetworkWrapper.cpp TrainingManager.cpp MCTS.cpp -L"C:\Users\farmersrice\AppData\Local\Programs\Python\Python37\libs" -lpython37 -o play_elo_games -I"C:\Users\farmersrice\AppData\Local\Programs\Python\Python37\include"

//https://www.yuzeh.com/etc/2019-04-07-bayeselo-for-games
//Write to pgn for bayeselo.exe to process

/*
[White "p1"][Black "p2"][Result "1-0"] 1. c4 Nf6
[White "p1"][Black "p2"][Result "0-1"] 1. c4 Nf6
*/

int main() {
	ios_base::sync_with_stdio(false); 
	cin.tie(NULL);

	TrainingManager t;

	ofstream dataStream;
	dataStream.open("elo_results2.pgn");

	for (int i = 35; i < 61; i++) {
		//Compare i and i + 1
		auto result = t.compareTwoNetworks(i, i + 1);

		int iwins = (int) (result.first.first - 0.5 * result.second + EPS);
		int ipluswins = (int) (result.first.second - 0.5 * result.second + EPS);
		int draws = result.second;


		for (int j = 0; j < iwins; j++) {
			dataStream << "[White \"p" << i << "\"][Black \"p" << i + 1 << "\"][Result \"1-0\"] 1. c4 Nf6" << endl;
		}

		for (int j = 0; j < ipluswins; j++) {
			dataStream << "[White \"p" << i + 1 << "\"][Black \"p" << i << "\"][Result \"1-0\"] 1. c4 Nf6" << endl;
		}

		//remember to set drawelo 0, advantage 0

		for (int j = 0; j < draws / 2; j++) {
			dataStream << "[White \"p" << i << "\"][Black \"p" << i + 1 << "\"][Result \"1/2-1/2\"] 1. c4 Nf6" << endl;
		}

		for (int j = 0; j < draws - (draws / 2); j++) {
			dataStream << "[White \"p" << i + 1 << "\"][Black \"p" << i << "\"][Result \"1/2-1/2\"] 1. c4 Nf6" << endl;
		}
	}


	dataStream.close();
}
