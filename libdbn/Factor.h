#pragma once
#include <vector>
#include <unordered_map>
#include <set>
#include <string>
#include <cassert>
#include <cstdint>

#include "Hypothesis.h"
#include "BNet.h"
#include "utils.h"

using namespace std;
using namespace libdbn;

//CPD条件概率分布，POT势函数
enum FactorType { CPD, POT };

struct RandVar;

class Factor
{
public:
	Factor(const Factor&);
	//Factor();
	Factor();
	Factor(const vector<string> & elems);
	//full constructor
	Factor(const vector<string> & elems, const vector<size_t> & elemSize);

	Factor(const vector<RandVar> & randVars);
	//destructor
	~Factor();
	Factor& operator=(const Factor&);

	FactorType getFactorType() const { return this->factorType; }

	vector<string>* getElementsName() const {
		return pName;
	}

	vector< vector < double > >* getElementsTable() const {
		return pTable;
	}

	vector<bool>* getParentsMark() const {
		return this->pParents;
	}

	uint32_t getVarSize() const {
		return varSize;
	}

	uint32_t getRowsSize() const {
		return rows;
	}

	void setFactorType(FactorType type) {
		this->factorType = type;
	}

	Factor& setParents(const vector<string> & parents);
	Factor& setProb(const vector<double> &);

	Factor& normalize();

	//某变量是否存在
	bool exists(const string& var) const;

	//varset里哪些变量存在
	//commonvars 是结果，自动清空
	vector<string>& exists(vector<string>& commonvars, const vector<string>& varset) const;

	//name变量最可能的取值，用于learning
	double getMaxProValue(string & name);

	//utils
	uint32_t indexOf(string & name) const;
	size_t getElemSize(string & name);
	vector<size_t> getElemSize(vector<string> & names);
	vector<double> getProbVector() const;
	uint32_t hashCode();

	//基本操作
	//乘
	Factor multiply(const Factor & a) const;
	//边缘化(参数为被加和变量，将被消去)
	Factor summation(vector<string> & varset) const;
	//设置证据
	Factor setEvidence(unordered_map<string, double>&) const;

	friend bool operator<(const Factor & lhs, const Factor & rhs);

	friend class InfEngine;
	friend class Hypothesis;

private:
	//元素名称
	vector<string> *pName;
	unordered_map<string, int> *pLocation;

	//元素取值个数
	vector<size_t> *pElemSize;

	//条件概率表
	vector<vector<double>> *pTable;

	//函数类型：条件概率，势函数
	FactorType factorType;

	//条件概率信息 false 为children， true 为parents,
	//CPT特殊形式为：P(children | NULL)
	//若是Potential function则为NULL
	vector<bool> *pParents;

	//变量个数（列），行数
	uint32_t varSize;
	uint32_t rows;
	uint32_t hash;

	void newFactor(const vector<string> & elems, const vector<size_t> & elemSize);

	//get the same variable in two different set
	static unordered_map<string, pair<int, int> > getIntersection(const Factor* a, const Factor* b);

};
