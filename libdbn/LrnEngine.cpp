#include "LrnEngine.h"

LrnEngine::~LrnEngine()
{
}

void LrnEngine::releaseStrategy(LearningStrategy * strategy) {
	if (strategy != NULL) {
		delete strategy;
	}
	strategy = NULL;
}

LearningStrategy* LrnEngine::inputStrategy(string & param) {
	//pass
	return new EMStrategy();
}

void LrnEngine::paramLearning(BNet & bnet, string & trainsetFileName, string & param) {
	
	//初始化假设集
	Hypothesis * hypo = Hypothesis::init(bnet);
	//读取观测数据文件（训练集）
	hypo->readBNetData(trainsetFileName);
	//设置初始模型参数
	LearningStrategy * pStrategy = inputStrategy(param);
	//学习过程
	pStrategy->doLearing(this, hypo, pStrategy);
	//数据向贝叶斯网络回填
	hypo->setValue(bnet);;
	
	//回收操作
	hypo->releaseHypothesis();
	releaseStrategy(pStrategy);
}

void LrnEngine::mle(Hypothesis * hy, const LearningStrategy * strategy) {

}

void LrnEngine::em(Hypothesis * hy, const LearningStrategy * strategy) {
	EMStrategy * emStrategy = (EMStrategy *)strategy;

	//test
	int i = 1, j = 1, k = 1;
	emStrategy->getParameter(hy, i, j, k);
	//do 

}
