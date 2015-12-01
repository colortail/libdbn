#pragma once
#include "BNet.h"

class Hypothesis
{
public:
	Hypothesis();
	~Hypothesis();

	//父节点取值的大小，节点范围的取值大小
	void insertParameter(int paRange, int valRange);

	vector< vector <double> > & getCPT(int i);

private:
	vector< vector< vector<double> > > parameters;
	vector< vector< vector<double> > > eigen;
};

