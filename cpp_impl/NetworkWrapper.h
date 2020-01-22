#pragma once
#include <string>
#include <vector>

#ifndef PyObject_HEAD
struct _object;
typedef _object PyObject;
#endif


bool fileExists(const char* name);
std::string vectorToString(std::vector<float> values, bool spaces = false);

struct NetworkWrapper {
	PyObject *pName, *pModule, *pDict, *pFuncLoadModel, *pValue, *pArgs, *pFuncPredict;
	
	NetworkWrapper();
	//~NetworkWrapper(); //finalize interpreter 
	void loadNetwork(int modelNum, int index);
	int getBestNetwork();
	int getLatestNetwork();
	void loadBestNetwork(int index); //Read from a file in the models folder
	void loadLatestNetwork(int index);
	std::vector<std::pair<std::vector<float>, float>> predict(std::vector<std::vector<float>> inputs, int index);
};


