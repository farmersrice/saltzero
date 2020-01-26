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
#define mp make_pair
#define pb push_back
#define remove pop
#define all(x) x.begin(), x.end()

//g++ -O2 -std=gnu++14 generate_games.cpp UtttBoard.cpp NetworkWrapper.cpp TrainingManager.cpp MCTS.cpp -L"C:\Users\farmersrice\AppData\Local\Programs\Python\Python37\libs" -lpython37 -o generate_games -I"C:\Users\farmersrice\AppData\Local\Programs\Python\Python37\include

const int THREADS = 4;

int main() {
	ios_base::sync_with_stdio(false); 
	cin.tie(NULL);

	TrainingManager t;

	//multithreaded game generation

	vector<future<pair<vector<tuple<vector<float>, vector<float>, int>>, pair<pair<double, double>, int>>>> items;

	cout << "beginning generation" << endl;

	for (int i = 0; i < THREADS; i++) {
		// Apparently python's global interpreter lock means that python is really only single-threaded, unless we use the multiprocessing library.
		// So, our performance gain must be quite limited. However, it does make the C++ game-handling cost nearly zero, which provides 30% speedup.
		// This also means that dramatically scaling up the number of threads doesn't increase speed, since python is the bottleneck yet again.
		// However, the multiprocessing library can't be effectively used per-thread, since we don't really have a "unified thread" to call join()  on.

		// With the old network architecture we were hitting 16.5 seconds per move per 1k games on 8 threads. New one is 30 seconds per move. Both cpu only.
		// New one takes a lot more cpu during prediction.

		// With gpu, 14.75 seconds for new network, similar for old.


		items.pb(async([&t]{ return t.playGames(PLAY_BATCH_SIZE, true, 0); }));
	}

	for (int i = 0; i < THREADS; i++) {
		t.saveData(items[i].get().first, PLAY_BATCH_SIZE);
	}

	cout << "Finished generating games" << endl;
}
