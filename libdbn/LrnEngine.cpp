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
	
	//��ʼ�����輯
	Hypothesis * hypo = Hypothesis::init(bnet);
	//��ȡ�۲������ļ���ѵ������
	hypo->readBNetData(trainsetFileName);
	//���ó�ʼģ�Ͳ���
	LearningStrategy * pStrategy = inputStrategy(param);
	//ѧϰ����
	pStrategy->doLearing(this, hypo, pStrategy);
	//������Ҷ˹�������
	hypo->setValue(bnet);;
	
	//���ղ���
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
