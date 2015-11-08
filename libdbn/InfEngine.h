#pragma once
#include <string>
#include "BNet.h"
#include "JTree.h"
#include "Metric.h"

#ifndef _FACTOR_
#define _FACTOR_
#include "Factor.h"
#endif

//推理算法接口
class InfEngine{
private:
	/*------------singleton-----------------------*/
	InfEngine(std::string & strategy) {	}

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
	int getMaxCardinalityElem(const BNet & moral, const vector<bool> & marked);

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

	//query ： 推理

	//贪婪消去，特例是最小缺边搜索
	vector<int> greedyOrdering(const BNet& moral, Metric & foo);

	//最大势搜索
	vector<int> maxCardinalitySearch(const BNet& moral, int root = 0);

	//变量消元法
	Factor variableElim(BNet& bnet, vector<string> queryset, unordered_map<string, double> evidset, vector<int> pi);

	//变量消元操作
	set<Factor>& eliminate(set<Factor>& factorset, vector<string> elimvars);

	//构建联合树（贝叶斯网引论 5.6 团树的构造）
	JTree buildJTree(BNet & bnet);
	JTree buildJTree(BNet & bnet, vector<int>);

	//端正图构造联合树（BNT Matlab ver.）
	JTree graphToJTree(BNet & moral);
};
