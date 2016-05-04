#pragma once

#ifndef _FACTOR_
#define _FACTOR_
#include "Factor.h"
#endif

#include "Clique.h"
#include "BNet.h"
#include "JTree.h"
#include "InferStrategy.h"
#include "Metric.h"
#include "utils.h"
#include "InOutUtils.h"
#include <string>
#include <map>

class InferStrategy;
class Metric;

//�����㷨�ӿ�
class InfEngine{
private:
	/*------------singleton-----------------------*/
	InfEngine(std::string & strategy) : consistence(true) {}

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

	//if JTree have setted evidences(false is not, true mean that have setted)
	bool consistence;

	static unordered_map<string, double> evidCache;

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

	void initJTreeCPD(JTree & jtree, const BNet & bnet);

	//query �� ����
	Factor inference(BNet &, vector<string> &, unordered_map<string, double> &, InferStrategy & strategy);

	bool getJTreeStatus() { return this->consistence; }

	//̰����ȥ����������Сȱ������(moral�豻������)
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
	JTree& buildJTree(JTree&, BNet & moral, vector<int> & pi);
	JTree& buildJTree(JTree&, BNet &, vector<int> &, set<int> &, int);

	/*================================*/
	/*
	������ǻ����̻�ı�ͼ�Ľṹ
	*/
	BNet triangulate(const BNet &, vector<int>& pi);

	//set<int> BronKerboschRecursive(set<int> r, set<int> p, set<int> x);
	void BronKerboschRecursive(map<int, set<int>> & nnbrs,
		set<int> & cand,
		set<int> & done,
		vector<int> & sofar,
		vector< vector<int> > & clique);

	vector<Clique>* findCliques(BNet &);
	vector<Clique>* findCliquesRecursive(BNet &);

	/** 
	����ͼ����������
	make_max_clique_graph
	�����Ǹ����ǻ�ͼ
	*/
	void graphToJTree(JTree & jtree, BNet & triGraph);
	/*================================*/

	//Shafer Shenoy Algorithm
	void messagePropagation(BNet & bnet,
		JTree & jtree,
		vector<string> & queryset,
		unordered_map<string, double>& evidset);

	void setEvidence(JTree & jtree, const BNet & bn, unordered_map<string, double> & evidset);

	Factor getTabularFromJTree(BNet & bnet,
		JTree & jtree,
		vector<string> & queryset,
		unordered_map<string, double>& evidset);

	//Hugin Algorithm
	Factor messagePassing(BNet & bnet, vector<string> & queryset, unordered_map<string, double>& evidset);

};
