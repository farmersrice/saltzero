#include "NetworkWrapper.h"
#include "TrainingManager.h"
#include "UtttBoard.h"
#include <bits/stdc++.h>
#include <direct.h>

using namespace std;
typedef long long ll;
typedef pair<int, int> pair2;
typedef pair<int, pair<int, int>> pair3;
typedef pair<int, pair<int, pair<int, int>>> pair4;
#define MAXN 200013
#define INF 1000000000000000000LL
#define MOD 1000000007LL
#define IINF 1000000000
#define mp make_pair
#define pb push_back
#define remove pop
#define all(x) x.begin(), x.end()

// g++ -O2 -std=gnu++14 temp.cpp UtttBoard.cpp NetworkWrapper.cpp
// TrainingManager.cpp MCTS.cpp
// -L"C:\Users\farmersrice\AppData\Local\Programs\Python\Python37\libs"
// -lpython37

int main() {
    if (fopen("FILENAME.in", "r")) {
        freopen("FILENAME.in", "r", stdin);
        freopen("FILENAME.out", "w", stdout);
    }
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    TrainingManager t;

    UtttBoard b;

    vector<vector<float>> inputQueries;

    for (int i = 0; i < 1000; i++) {
        inputQueries.pb(b.toNetInputVector());
    }

    clock_t start = clock();

    vector<pair<vector<float>, float>> values = t.net.predict(inputQueries, 0);

    cout << "time taken " << (clock() - start) / (1.0 * CLOCKS_PER_SEC) << endl;

    cout << "last result is ";

    for (auto i : values[values.size() - 1].first) {
        cout << i << ' ';
    }
    cout << values[values.size() - 1].second << ' ';
    cout << endl;
}

// GCD GCD GCD USE GCD IN MATH