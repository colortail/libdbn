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

//推理算法接口
class InfEngine{
private:
	/*------------singleton-----------------------*/
	InfEngine(std::string & strategy) : consistence(true) {}

	static InfEngine* m_pInstance;

	//单例GC内部类
	class EngineGC{
	public:
		~EngineGC(){
			if (InfEngine::m_pInstance) {
				delete InfEngine::m_pInstance;
				InfEngine::m_pInstance = NULL;
			}
		}
	};
	//自动调用GC对象的析构
	static EngineGC engineGC;

	/*--------------------------------------*/

	//if JTree have setted evidences(false is not, true mean that have setted)
	bool consistence;

	static unordered_map<string, double> evidCache;

	int getMaxCardinalityElem(const BNet & moral, const vector<bool> & marked);

	set<Factor>& setEvidence(set<Factor>& factorset, unordered_map<string, double> &);

public:

	//constructor : 初始化联合树推理引擎
	static InfEngine* getInstance() {
		return getInstance(std::string("jtree"));
	}

	static InfEngine* getInstance(std::string & strategy) {
		if (m_pInstance == NULL)
			m_pInstance = new InfEngine(strategy);
		return m_pInstance;
	}

	void initJTreeCPD(JTree & jtree, const BNet & bnet);

	//query ： 推理
	Factor inference(BNet &, vector<string> &, unordered_map<string, double> &, InferStrategy & strategy);

	bool getJTreeStatus() { return this->consistence; }

	//贪婪消去，特例是最小缺边搜索(moral需被端正化)
	vector<int> greedyOrdering(const BNet& moral, Metric & foo);

	//最大势搜索
	vector<int> maxCardinalitySearch(const BNet& moral, int root = 0);

	//变量消元法
	Factor variableElim(BNet& bnet, 
		const vector<string> & queryset, 
		unordered_map<string, double> & evidset,
		const vector<int> & pi);

	//变量消元操作
	set<Factor>& eliminate(set<Factor>& factorset, vector<string>& elimvars);

	//构建联合树（贝叶斯网引论 5.6 团树的构造）
	JTree buildJTree(BNet & bnet);
	JTree& buildJTree(JTree&, BNet & moral, vector<int> & pi);
	JTree& buildJTree(JTree&, BNet &, vector<int> &, set<int> &, int);

	/*================================*/
	/*
	这个三角化过程会改变图的结构
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
	端正图构造联合树
	make_max_clique_graph
	输入是个三角化图
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
