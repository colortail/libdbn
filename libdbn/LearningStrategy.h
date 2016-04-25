#pragma once
#include "LrnEngine.h"

class LrnEngine;

class LearningStrategy
{
public:
	LearningStrategy();
	~LearningStrategy();
	virtual void doLearing(LrnEngine * engine, Hypothesis * hy, const LearningStrategy * strategy) = 0;
};

//EM 算法模型参数
class EMStrategy : public LearningStrategy {
public:
	void doLearing(LrnEngine * engine, Hypothesis * hy, const LearningStrategy * strategy);
	double getParameter(Hypothesis * hy, int i, int j, int k);
	double getData(Hypothesis * hy, int i, int j);
};

