#pragma once
#include <string>
#include "BNet.h"
#include "JTree.h"
#include "Metric.h"

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
	vector<int> greedyOrdering(const BNet& bnet, Metric & foo);
	vector<int> maxCardinalitySearch(const BNet& bnet);

	Factor variableElim(BNet& bnet, vector<string> queryset, unordered_map<string, double> evidset, vector<int> pi);
	set<Factor>& eliminate(set<Factor>& factorset, vector<string> elimvars);

	JTree buildJTree(BNet & bnet);
	JTree buildJTree(BNet & bnet, vector<int>);
};
