#include "LearningStrategy.h"


LearningStrategy::LearningStrategy()
{
}


LearningStrategy::~LearningStrategy()
{
}

void EMStrategy::doLearing(LrnEngine * engine, Hypothesis * hy, const LearningStrategy * strategy) {
	engine->em(hy, strategy);
}

double EMStrategy::getParameter(Hypothesis * hy, int i, int j, int k) {
	return 0;
}