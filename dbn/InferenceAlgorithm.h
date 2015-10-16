#pragma once
#include <string>

//推理算法接口
class InferenceAlgorithm {
private:
	/*------------singleton-----------------------*/
	InferenceAlgorithm(std::string & strategy);
	static InferenceAlgorithm* m_pInstance;

	//单例GC内部类
	class EngineGC{
	public:
		~EngineGC(){
			if (InferenceAlgorithm::m_pInstance) {
				delete InferenceAlgorithm::m_pInstance;
				InferenceAlgorithm::m_pInstance = NULL;
			}
		}
	};
	//自动调用GC对象的析构
	static EngineGC engineGC;

	/*--------------------------------------*/
public:

	//constructor : 初始化联合树
	static InferenceAlgorithm* getInstance() {
		return getInstance(std::string("jtree"));
	}

	static InferenceAlgorithm* getInstance(std::string & strategy) {
		if (m_pInstance == NULL)
			m_pInstance = new InferenceAlgorithm(strategy);
		return m_pInstance;
	}

	//evidence： 添加证据

	//query ： 推理

};
