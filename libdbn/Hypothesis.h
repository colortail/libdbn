#pragma once

#include <fstream>

#include "utils.h"
#include "BNet.h"

class BNet;
class Factor;
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
	
public:
	static Hypothesis* init(BNet& bnet);

	void releaseHypothesis();

	void setValue(BNet & bnet);

	void readBNetData(string & filename);

	void doMLE();

};

