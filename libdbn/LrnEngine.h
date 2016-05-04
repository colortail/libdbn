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

	void releaseStrategy(LearningStrategy *);
public:
	~LrnEngine();

	static LrnEngine* getInstance() {
		if (m_pInstance == NULL)
			m_pInstance = new LrnEngine();
		return m_pInstance;
	}
	
	//参数学习
	void paramLearning(BNet & bnet, string & trainsetFileName, string & param);

	//调整模型参数
	LearningStrategy* inputStrategy(string & param);
	
	//初始化假设集参数
	//Hypothesis* initParameter(const BNet & bnet);
	
	//根据假设集设置网络参数
	//void setProbability(BNet & bnet, Hypothesis * hy);

	//采样数据
	//Hypothesis * sampling(Hypothesis * hy, const LearningData & data);

	//Maximun Likelihood Estimation
	void mle(Hypothesis * hy, const LearningStrategy * strategy);

	//EM algorithm
	void em(Hypothesis * hy, const LearningStrategy * strategy);

};

