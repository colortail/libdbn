#pragma once
#include <string>

//�����㷨�ӿ�
class InferenceAlgorithm {
private:
	/*------------singleton-----------------------*/
	InferenceAlgorithm(std::string & strategy);
	static InferenceAlgorithm* m_pInstance;

	//����GC�ڲ���
	class EngineGC{
	public:
		~EngineGC(){
			if (InferenceAlgorithm::m_pInstance) {
				delete InferenceAlgorithm::m_pInstance;
				InferenceAlgorithm::m_pInstance = NULL;
			}
		}
	};
	//�Զ�����GC���������
	static EngineGC engineGC;

	/*--------------------------------------*/
public:

	//constructor : ��ʼ��������
	static InferenceAlgorithm* getInstance() {
		return getInstance(std::string("jtree"));
	}

	static InferenceAlgorithm* getInstance(std::string & strategy) {
		if (m_pInstance == NULL)
			m_pInstance = new InferenceAlgorithm(strategy);
		return m_pInstance;
	}

	//evidence�� ���֤��

	//query �� ����

};
