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
	int getMaxCardinalityElem(const BNet & moral, const vector<bool> & marked);

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

	//̰����ȥ����������Сȱ������
	vector<int> greedyOrdering(const BNet& moral, Metric & foo);

	//���������
	vector<int> maxCardinalitySearch(const BNet& moral, int root = 0);

	//������Ԫ��
	Factor variableElim(BNet& bnet, vector<string> queryset, unordered_map<string, double> evidset, vector<int> pi);

	//������Ԫ����
	set<Factor>& eliminate(set<Factor>& factorset, vector<string> elimvars);

	//��������������Ҷ˹������ 5.6 �����Ĺ��죩
	JTree buildJTree(BNet & bnet);
	JTree buildJTree(BNet & bnet, vector<int>);

	//����ͼ������������BNT Matlab ver.��
	JTree graphToJTree(BNet & moral);
};
