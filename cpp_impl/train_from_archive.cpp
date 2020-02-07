#include <bits/stdc++.h>
#include <direct.h>
#include <future>

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


int main() {
	ios_base::sync_with_stdio(false); 
	cin.tie(NULL);

	//TrainingManager t;

	for (int i = 0; i < IINF; i++) {

		system("process_archive.exe");

		_chdir("..");
		system("python main_train_cpp.py");

		_chdir("cpp_impl");

		system("compare_networks.exe");
	}
	

	
}

