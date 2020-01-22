#include "NetworkWrapper.h"
#include <iostream>
#include <Python.h>
//#include "include/Python.h"
#include <sstream>
#include <fstream>
#include <mutex>

using namespace std;

const int IINF = 1000000000;

bool fileExists(const char* name) {
	FILE *pFile = fopen(name, "r");

	if (pFile != NULL) {
		fclose(pFile);
		return true;
	}
	return false;
}

string vectorToString(vector<float> values, bool spaces) {
	string answer(values.size(), '0');

	for (int i = 0; i < values.size(); i++) {
		if (abs(values[i] - 1.0f) <= 0.0001f) {
			answer[i] = '1';
		}
	}

	return answer;
}

NetworkWrapper::NetworkWrapper() {
	Py_Initialize();
	pName = PyUnicode_FromString((char*)"network_wrapper");
	pModule = PyImport_Import(pName);
	pDict = PyModule_GetDict(pModule);

	pFuncLoadModel = PyDict_GetItemString(pDict, (char*)"load_model");
	pFuncPredict = PyDict_GetItemString(pDict, (char*)"predict");
}
/*
NetworkWrapper::~NetworkWrapper() {
	cout << "Finalizing interpreter " << endl;

	PyObject *pFuncResetKeras = PyDict_GetItemString(pDict, (char*)"reset_keras");
	PyObject_CallObject(pFuncResetKeras, NULL);
	//PyRun_SimpleString("from numba import cuda;print('do we have cuda');print(cuda);cuda.select_device(0);cuda.close();cuda.select_device(0)");
	cout << "GPU memory should be freed." << endl;
	Py_Finalize();
}*/

void NetworkWrapper::loadNetwork(int modelNum, int index) {
	pArgs = PyTuple_New(2);
	PyTuple_SET_ITEM(pArgs, 0, PyLong_FromLong(modelNum));
	PyTuple_SET_ITEM(pArgs, 1, PyLong_FromLong(index));

	Py_XDECREF(PyObject_CallObject(pFuncLoadModel, pArgs)); 
	Py_DECREF(pArgs);
}

int NetworkWrapper::getBestNetwork() {
	ifstream best;
	best.open("../models/model_info");
	int answer;
	best >> answer;
	best.close();
	return answer;
}

int NetworkWrapper::getLatestNetwork() {
	//check file existence and such for this part

	int networkId = 0;
	for (int i = 0; i < IINF; i++) {
		string test = "../models/best_" + to_string(i) + ".h5";

		const char *name = test.c_str();

		if (!fileExists(name)) {
			networkId = i - 1;
			break;
		}
	}

	return networkId;
}

void NetworkWrapper::loadBestNetwork(int index) {
	int bestNetwork = getBestNetwork();
	cout << "loading best net " << bestNetwork << endl;
	loadNetwork(bestNetwork, index);
}

void NetworkWrapper::loadLatestNetwork(int index) {
	int latestNetwork = getLatestNetwork();
	cout << "loading latest net " << latestNetwork << endl;
	loadNetwork(latestNetwork, index);
}

mutex pythonLock;

vector<pair<vector<float>, float>> NetworkWrapper::predict(vector<vector<float>> inputs, int index) {
	//auto start = clock();

	vector<float> queries(inputs.size() * 189);
	int counter = 0;
	for (int i = 0; i < inputs.size(); i++) {
		for (int j = 0; j < 189; j++) {
			queries[counter++] = inputs[i][j];
		}
	}

	pythonLock.lock();
	PyObject *queryStringArgs = PyTuple_New(2);
	PyTuple_SET_ITEM(queryStringArgs, 0, PyUnicode_FromString(vectorToString(queries).c_str())); //this is faster than from string bytes
	PyTuple_SET_ITEM(queryStringArgs, 1, PyLong_FromLong(index));

	//auto mid = clock();

	PyObject *queryResult = PyObject_CallObject(pFuncPredict, queryStringArgs);

	const char *s = PyBytes_AS_STRING(queryResult);
	const float *parsed = reinterpret_cast<const float*>(s);

	//auto mid2 = clock();

	vector<pair<vector<float>, float>> answer(inputs.size());


	counter = 0;
	for (int i = 0; i < inputs.size(); i++) {
		vector<float> p1(81);

		for (int j = 0; j < 81; j++) {
			p1[j] = parsed[counter++];
		}
		float p2 = parsed[counter++];

		answer[i].first = p1;
		answer[i].second = p2;
	}

	//auto end = clock();

	//cout << "before python time " << (mid - start) / (1.0 * CLOCKS_PER_SEC ) << endl;
	//cout << "after python time " << (end - mid2) / (1.0 * CLOCKS_PER_SEC ) << endl;
	//cout << "total time " << (end - start) / (1.0 * CLOCKS_PER_SEC ) << endl;

	Py_DECREF(queryStringArgs);
	Py_DECREF(queryResult);
	pythonLock.unlock();

	
	return answer;
}
