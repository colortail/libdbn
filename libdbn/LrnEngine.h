#pragma once
#include <string>

#include "Hypothesis.h"
#include "LearningStrategy.h"
#include "LearningData.h"
#include "InfEngine.h"

class Hypothesis;
class LearningStrategy;

class LrnEngine
{
private:
	/*------------singleton-----------------------*/
	LrnEngine() {}

	static LrnEngine* m_pInstance;

	//����GC�ڲ���
	class EngineGC{
	public:
		~EngineGC(){
			if (LrnEngine::m_pInstance) {
				delete LrnEngine::m_pInstance;
				LrnEngine::m_pInstance = NULL;
			}
		}
	};
	//�Զ�����GC���������
	static EngineGC engineGC;

	void releaseStrategy(LearningStrategy *);
public:
	~LrnEngine();

	static LrnEngine* getInstance() {
		if (m_pInstance == NULL)
			m_pInstance = new LrnEngine();
		return m_pInstance;
	}
	
	//����ѧϰ
	void paramLearning(BNet & bnet, string & trainsetFileName, string & param);

	//����ģ�Ͳ���
	LearningStrategy* inputStrategy(string & param);
	
	//��ʼ�����輯����
	//Hypothesis* initParameter(const BNet & bnet);
	
	//���ݼ��輯�����������
	//void setProbability(BNet & bnet, Hypothesis * hy);

	//��������
	//Hypothesis * sampling(Hypothesis * hy, const LearningData & data);

	//Maximun Likelihood Estimation
	void mle(Hypothesis * hy, const LearningStrategy * strategy);

	//EM algorithm
	void em(Hypothesis * hy, const LearningStrategy * strategy);

};

