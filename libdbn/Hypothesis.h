#pragma once
#include "BNet.h"

class Hypothesis
{
public:
	Hypothesis();
	~Hypothesis();

	//���ڵ�ȡֵ�Ĵ�С���ڵ㷶Χ��ȡֵ��С
	void insertParameter(int paRange, int valRange);

	vector< vector <double> > & getCPT(int i);

private:
	vector< vector< vector<double> > > parameters;
	vector< vector< vector<double> > > eigen;
};

