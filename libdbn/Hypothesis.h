#pragma once

#include <fstream>

#include "utils.h"
#include "BNet.h"
#include "LearningStrategy.h"
#include "InfEngine.h"

class BNet;
class Factor;
class LearningStrategy;

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

	//采样点个数(节点数)
	uint32_t sampleNum;
	//样本个数
	uint32_t sampleSize;
	//模型参数
	vector<Factor> * param;
	//训练集（观测样本）数据
	vector<vector<double> > data;
	//缺失值位置
	vector<vector<int> > lacks;
	//缺失权重
	vector<double> weight;
	
public:
	static Hypothesis* init(BNet& bnet);

	void releaseHypothesis();

	void setValue(BNet & bnet);

	void readBNetData(string & filename);

	void doMLE(const LearningStrategy & strategy);

	void repairDataSet();

	void doEM(const LearningStrategy & strategy);

};

