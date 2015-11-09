#pragma once
#include <string>
#include "BNet.h"
#include "JTree.h"
#include "InferStrategy.h"
#include "Metric.h"
#include "utils.h"
#include "InOutUtils.h"

#ifndef _FACTOR_
#define _FACTOR_
#include "Factor.h"
#endif

enum INFERSTRATEGY {VE, JTREE};

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

	set<Factor>& setEvidence(set<Factor>& factorset, unordered_map<string, double> &);

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
	Factor inference(BNet &, 
		vector<string> &, 
		unordered_map<string, double> &,
		INFERSTRATEGY);

	//̰����ȥ����������Сȱ������
	vector<int> greedyOrdering(const BNet& moral, Metric & foo);

	//���������
	vector<int> maxCardinalitySearch(const BNet& moral, int root = 0);

	//������Ԫ��
	Factor variableElim(BNet& bnet, 
		const vector<string> & queryset, 
		unordered_map<string, double> & evidset,
		const vector<int> & pi);

	//������Ԫ����
	set<Factor>& eliminate(set<Factor>& factorset, vector<string>& elimvars);

	//��������������Ҷ˹������ 5.6 �����Ĺ��죩
	JTree buildJTree(BNet & bnet);
	JTree& buildJTree(JTree & jtree, BNet & moral, vector<int> & pi);
	JTree& buildJTree(JTree & jtree, BNet & moral, vector<int> & pi, int i);

	//����ͼ������������BNT Matlab ver.��
	JTree graphToJTree(BNet & moral);
};
