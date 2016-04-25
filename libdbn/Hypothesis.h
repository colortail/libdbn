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
	
	//ģ������
	MODEL_TYPE modelType;
	//ģ������
	void * model;

	//���������
	uint32_t sampleNum;
	//��������
	uint32_t sampleSize;
	//ģ�Ͳ���
	vector<double *> param;
	//ѵ�������۲�����������
	vector<vector<double>> data;
	
public:
	static Hypothesis* init(BNet& bnet);

	void releaseHypothesis();

	void setValue(BNet & bnet);

	void readBNetData(string & filename);

};

