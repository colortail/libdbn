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
	
	//ģ������
	MODEL_TYPE modelType;
	//ģ������
	void * model;

	//���������(�ڵ���)
	uint32_t sampleNum;
	//��������
	uint32_t sampleSize;
	//ģ�Ͳ���
	vector<Factor> * param;
	//ѵ�������۲�����������
	vector<vector<double> > data;
	//ȱʧֵλ��
	vector<vector<int> > lacks;
	//ȱʧȨ��
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

