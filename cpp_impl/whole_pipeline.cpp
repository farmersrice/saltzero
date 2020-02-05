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

//g++ -O2 -std=gnu++14 whole_pipeline.cpp UtttBoard.cpp NetworkWrapper.cpp TrainingManager.cpp MCTS.cpp -L"C:\Users\farmersrice\AppData\Local\Programs\Python\Python37\libs" -lpython37 -o whole -I"C:\Users\farmersrice\AppData\Local\Programs\Python\Python37\include"

/*
g++ -O2 -std=gnu++14 compare_networks.cpp UtttBoard.cpp NetworkWrapper.cpp TrainingManager.cpp MCTS.cpp -L"C:\Users\farmersrice\AppData\Local\Programs\Python\Python37\libs" -lpython37 -o compare_networks -I"C:\Users\farmersrice\AppData\Local\Programs\Python\Python37\include"
g++ -O2 -std=gnu++14 generate_games.cpp UtttBoard.cpp NetworkWrapper.cpp TrainingManager.cpp MCTS.cpp -L"C:\Users\farmersrice\AppData\Local\Programs\Python\Python37\libs" -lpython37 -o generate_games -I"C:\Users\farmersrice\AppData\Local\Programs\Python\Python37\include
g++ -O2 -std=gnu++14 whole_pipeline.cpp -o whole
*/

/*

You might be wondering: Why do we call all these random executables instead of actually doing it the "right way" and having it all done in code?

The answer is: Keras/TensorFlow doesn't let us easily release used gpu memory. Tried a variety of methods, including https://github.com/keras-team/keras/issues/12625
as well as allowing memory growth. Only using the numba library was it possible to clear gpu memory, and this also had the side effect of not allowing us to use the gpu again.
Therefore, the simple solution is to create a dedicated executable, since after running the memory will be freed. Works like a charm.

Please note that you will have to compile compare_networks and generate_games for this to work. Also, if you aren't using windows, you will need to modify the system calls, of course.

*/
int main() {
	ios_base::sync_with_stdio(false); 
	cin.tie(NULL);

	//TrainingManager t;

	while (true) {

		system("generate_games.exe");

		_chdir("..");
		system("python main_train_cpp.py");

		_chdir("cpp_impl");

		system("compare_networks.exe");
	}
	

	
}

