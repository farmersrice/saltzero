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

int main() {
	ios_base::sync_with_stdio(false); 
	cin.tie(NULL);

	string filename;
	cin >> filename;

	string p1, p2;
	cin >> p1 >> p2;

	double resfirst, ressecond;
	int draws;

	cin >> resfirst >> ressecond >> draws;

	ofstream dataStream;
	dataStream.open(filename + ".pgn", ios::out | ios::app);


	int iwins = (int) (resfirst - 0.5 * draws + EPS);
	int ipluswins = (int) (ressecond - 0.5 * draws + EPS);

	for (int j = 0; j < iwins; j++) {
		dataStream << "[White \"p" << p1 << "\"][Black \"p" << p2 << "\"][Result \"1-0\"] 1. c4 Nf6" << endl;
	}

	for (int j = 0; j < ipluswins; j++) {
		dataStream << "[White \"p" << p2 << "\"][Black \"p" << p1 << "\"][Result \"1-0\"] 1. c4 Nf6" << endl;
	}

	//remember to set drawelo 0, advantage 0

	for (int j = 0; j < draws / 2; j++) {
		dataStream << "[White \"p" << p1 << "\"][Black \"p" << p2 << "\"][Result \"1/2-1/2\"] 1. c4 Nf6" << endl;
	}

	for (int j = 0; j < draws - (draws / 2); j++) {
		dataStream << "[White \"p" << p2 << "\"][Black \"p" << p1 << "\"][Result \"1/2-1/2\"] 1. c4 Nf6" << endl;
	}


	dataStream.close();
}
