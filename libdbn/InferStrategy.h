#pragma once
#include "BNet.h"
#include "InfEngine.h"

#include "InOutUtils.h"

//�������
class InferStrategy {
public:
	virtual Factor operator()(BNet & bnet,
		vector<string> & queryset,
		unordered_map<string, double> & evidset,
		InfEngine* infEngine) = 0;
};

class VariableElimination : public InferStrategy {
public:
	virtual Factor operator()(BNet & bnet,
		vector<string> & queryset,
		unordered_map<string, double> & evidset,
		InfEngine* infEngine);
};

class JTreeInference : public InferStrategy {
public:
	virtual Factor operator()(BNet & bnet,
		vector<string> & queryset,
		unordered_map<string, double> & evidset,
		InfEngine* infEngine);
};