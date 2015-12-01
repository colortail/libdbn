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

	//单例GC内部类
	class EngineGC{
	public:
		~EngineGC(){
			if (LrnEngine::m_pInstance) {
				delete LrnEngine::m_pInstance;
				LrnEngine::m_pInstance = NULL;
			}
		}
	};
	//自动调用GC对象的析构
	static EngineGC engineGC;

public:
	~LrnEngine();

	static LrnEngine* getInstance() {
		if (m_pInstance == NULL)
			m_pInstance = new LrnEngine();
		return m_pInstance;
	}

	//初始化假设集参数
	Hypothesis* initParameter(const BNet & bnet);
	//根据假设集设置网络参数
	void setProbability(BNet & bnet, Hypothesis * hy);

	//采样数据
	Hypothesis * sampling(Hypothesis * hy, const LearningData & data);

	//Maximun Likelihood Estimation
	void mle(Hypothesis * hy);

	//EM algorithm
	void em(Hypothesis * hy);

};

