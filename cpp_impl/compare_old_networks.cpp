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

//g++ -O2 -std=gnu++14 compare_old_networks.cpp UtttBoard.cpp NetworkWrapper.cpp TrainingManager.cpp MCTS.cpp -L"C:\Users\farmersrice\AppData\Local\Programs\Python\Python37\libs" -lpython37 -o compare_old -I"C:\Users\farmersrice\AppData\Local\Programs\Python\Python37\include"


int main() {
    if (fopen("FILENAME.in", "r")) {
        freopen("FILENAME.in", "r", stdin);
        freopen("FILENAME.out", "w", stdout);
    }
    ios_base::sync_with_stdio(false); 
    cin.tie(NULL);

    TrainingManager t;

    auto result = t.compareTwoNetworks(0);

    cout << " best " << result.first << " old " << result.second << endl;
}

//GCD GCD GCD USE GCD IN MATH