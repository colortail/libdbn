#pragma once
#include "GraphMatrix.h"
#include "Factor.h"
#include "JTree.h"

#include <vector>
#include <set>
#include <cstdint>

enum RandVarType { UNOBSERVED, OBSERVED, HIDDEN};
enum StructType {BNET, MORAL};

class Factor;
class JTree;

struct RandVar {
	int node; // id
	RandVarType type; // 是否观察到
	std::string name; //节点名

	//取值范围（add on 2015-11-10）
	size_t range;

	//概率值
	double nodeProb;

	RandVar(int _node, std::string & _name) 
		: node(_node), name(_name), type(UNOBSERVED), range(0), nodeProb(0) {}
	RandVar(int _node, std::string & _name, size_t _range)
		: node(_node), name(_name), type(UNOBSERVED), range(_range), nodeProb(0) {}
	friend bool operator<(const RandVar & var1, const RandVar & var2);
};

//in fact,it is a DAG
class BNet : public GraphMatrix<RandVar, double>
{
public:
	BNet();
	~BNet();
	BNet& operator=(const BNet& bn);

	StructType getStructType() const { return type; }

	uint32_t vertexSize() const { return this->n; }

	uint32_t edgeSize() const { return this->e; }

	std::set<Factor> getCPTs() const;

	void setCPTs(const std::set<Factor>&);

	std::set<int> getAllNbrs(const std::set<int>& restNode, int u);

	std::vector<std::string> getAllNodesName();

	//添加缺边
	void addFillEdge(int i, std::set<int> & restNode);

	//moralize
	void moralize();

	void introduceEdge(int, std::vector<bool> &);

	//节点编号的修正，使数据模型里编号和输入编号对应
	void correctNode();

	//更新所有节点，单个变量的边缘概率
	void updateAllVarProb(JTree * pJtree);

	friend class InOutUtils;
	//friend class InfEngine;

private:
	StructType type;
	std::set<Factor> cpts;
	JTree * pJtree;
};
