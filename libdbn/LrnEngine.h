#pragma once
#include <string>

#include "Hypothesis.h"
#include "LearningData.h"

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

public:
	~LrnEngine();

	static LrnEngine* getInstance() {
		if (m_pInstance == NULL)
			m_pInstance = new LrnEngine();
		return m_pInstance;
	}

	//��ʼ�����輯����
	Hypothesis* initParameter(const BNet & bnet);
	//���ݼ��輯�����������
	void setProbability(BNet & bnet, Hypothesis * hy);

	//��������
	Hypothesis * sampling(Hypothesis * hy, const LearningData & data);

	//Maximun Likelihood Estimation
	void mle(Hypothesis * hy);

	//EM algorithm
	void em(Hypothesis * hy);

};

