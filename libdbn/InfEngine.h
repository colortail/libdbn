#pragma once
#include <string>

#ifndef _FACTOR_
#define _FACTOR_
#include "Factor.h"
#endif

//�����㷨�ӿ�
class InfEngine{
private:
	/*------------singleton-----------------------*/
	InfEngine(std::string & strategy) {	}

	static InfEngine* m_pInstance;

	//����GC�ڲ���
	class EngineGC{
	public:
		~EngineGC(){
			if (InfEngine::m_pInstance) {
				delete InfEngine::m_pInstance;
				InfEngine::m_pInstance = NULL;
			}
		}
	};
	//�Զ�����GC���������
	static EngineGC engineGC;

	/*--------------------------------------*/
public:

	//constructor : ��ʼ����������������
	static InfEngine* getInstance() {
		return getInstance(std::string("jtree"));
	}

	static InfEngine* getInstance(std::string & strategy) {
		if (m_pInstance == NULL)
			m_pInstance = new InfEngine(strategy);
		return m_pInstance;
	}

	//query �� ����
	

};
