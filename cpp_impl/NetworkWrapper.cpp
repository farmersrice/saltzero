#include "NetworkWrapper.h"
#include <iostream>
#include <Python.h>
//#include "include/Python.h"
#include <sstream>
#include <fstream>

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

inline vector<float> parseFloats(string s, int length) {
    vector<float> answer(length);

    int counter = 0;
    int index = 0;
    while (index < s.size()) {
        while (index < s.size() && (s[index] == '\'' || s[index] == ' ')) index++;

        if (index >= s.size()) break;

        //Starting a new value

        int start = index;
        int end = index;

        while (end < s.size() && (s[end] != '\'' && s[end] != ' ')) end++;

        bool negative = false;
        if (s[start] == '-') {
            negative = true;
            start++;
        }
        int doubleIndex = end - start;
        for (int j = start; j < end; j++) {
            if (s[j] == '.') {
                doubleIndex = j - start;
                break;
            }
        }

        double curPower = 1.0;
        for (int j = 0; j < doubleIndex - 1; j++) {
            curPower *= 10.0;
        }

        double thisAns = 0.0;
        for (int j = start; j < end; j++) {
            if (s[j] == '.') continue;
            thisAns += curPower * (s[j] - '0');
            curPower /= 10.0;
        }

        answer[counter++] = (negative ? -thisAns : thisAns);
        index = end;

    }

    return answer;
}

vector<pair<vector<float>, float>> NetworkWrapper::predict(vector<vector<float>> inputs, int index) {
    //auto start = clock();

    vector<float> queries(inputs.size() * 189);
    int counter = 0;
    for (int i = 0; i < inputs.size(); i++) {
        for (int j = 0; j < 189; j++) {
            queries[counter++] = inputs[i][j];
        }
    }

    PyObject *queryStringArgs = PyTuple_New(2);
    PyTuple_SET_ITEM(queryStringArgs, 0, PyUnicode_FromString(vectorToString(queries).c_str()));
    PyTuple_SET_ITEM(queryStringArgs, 1, PyLong_FromLong(index));

    //auto mid = clock();

    PyObject *queryResult = PyObject_CallObject(pFuncPredict, queryStringArgs);

    PyObject *objectsRepresentation = PyObject_Repr(queryResult);
    const char* s = PyUnicode_AsUTF8(objectsRepresentation);
    string answerString(s);

    //auto mid2 = clock();

    Py_DECREF(queryStringArgs);
    Py_DECREF(queryResult);
    Py_DECREF(objectsRepresentation);


    vector<pair<vector<float>, float>> answer(inputs.size());

    vector<float> parsed = parseFloats(answerString, inputs.size() * 82);

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

    return answer;
}
