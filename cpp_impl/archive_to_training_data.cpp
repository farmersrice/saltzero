#include <bits/stdc++.h>
#include <direct.h>
#include <future>

using namespace std;

bool fileExists(const char* name) {
	FILE *pFile = fopen(name, "r");

	if (pFile != NULL) {
		fclose(pFile);
		return true;
	}
	return false;
}

const long long RANDOM = std::chrono::high_resolution_clock::now().time_since_epoch().count();
mt19937 rng(RANDOM);

int main() {
	ios_base::sync_with_stdio(false); 
	cin.tie(NULL);

	int fileCounter = 0;

	string stringPrefix = "../archive/1000_";
	string fileString = stringPrefix + to_string(fileCounter) + ".cpptactoe";

	while (fileExists(fileString.c_str())) {
		fileCounter++;
		fileString = stringPrefix + to_string(fileCounter) + ".cpptactoe";
	}

	ofstream games;
	games.open("../games/1000_0.cpptactoe");

	uniform_int_distribution<int> dis(0, 999);

	for (int i = 0; i < fileCounter; i++) {
		fileString = stringPrefix + to_string(i) + ".cpptactoe";

		ifstream dataStream;
		dataStream.open(fileString);

		cout << "Processing file " << i << endl;

		auto start = chrono::steady_clock::now();

		vector<string> allLines;

		string line;
		while (getline(dataStream, line)) {
			allLines.push_back(line);
		}


		uniform_int_distribution<int> dis(0, 49);
		for (int j = 0; j < allLines.size(); j += 8) {
			if (dis(rng) == 0) {
				//Add to training data
				for (int k = j; k < min((int)allLines.size(), j + 8); k++) {
					games << allLines[k] << '\n';
				}
			}
		}

		dataStream.close();
		auto end = chrono::steady_clock::now();

		cout << "Time taken: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms " << endl;
	}

	games.close();
}

