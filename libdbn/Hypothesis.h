#pragma once

#include <fstream>

#include "utils.h"
#include "BNet.h"

class BNet;

using namespace std;

enum MODEL_TYPE { SBNET, DBNET };

class Hypothesis
{
private:
	Hypothesis();
	~Hypothesis();
	
	//模型类型
	MODEL_TYPE modelType;
	//模型数据
	void * model;

	//采样点个数
	uint32_t sampleNum;
	//样本个数
	uint32_t sampleSize;
	//模型参数
	vector<double *> param;
	//训练集（观测样本）数据
	vector<vector<double>> data;
	
public:
	static Hypothesis* init(BNet& bnet);

	void releaseHypothesis();

	void setValue(BNet & bnet);

	void readBNetData(string & filename);

};

