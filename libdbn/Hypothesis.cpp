#include "Hypothesis.h"


Hypothesis::Hypothesis()
{
}


Hypothesis::~Hypothesis()
{
}

void Hypothesis::insertParameter(int paRange, int varRange) {
	this->parameters.push_back(vector< vector<double> >(paRange, vector<double>(varRange)));
}

vector< vector <double> > & Hypothesis::getCPT(int i) {
	
	return parameters[i];
}
