#pragma once
#include <vector>
#include <unordered_map>
#include <set>
#include <string>
#include <cassert>
#include <cstdint>
#include "utils.h"

using namespace std;
using namespace libdbn;

//CPD条件概率分布，POT势函数
enum FactorType { CPD, POT };

class Factor
{
public:
	Factor(const Factor&);
	//Factor();
	Factor(const vector<string> & elems);
	//full constructor
	Factor(const vector<string> & elems, const vector<size_t> & elemSize);
	void newFactor(const vector<string> & elems, const vector<size_t> & elemSize);

	//destructor
	~Factor();
	Factor& operator=(const Factor&);

	FactorType getFactorType() const { return this->factorType; }

	vector<string>* getElementsName() const {
		return pName;
	}

	vector<vector<double>>* getElementsTable() const {
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

	void setParents(const vector<string> & parents);
	void setProb(const vector<double> &);

	//基本操作
	//乘
	Factor multiply(Factor & a);
	//边缘化(参数为被加和变量，将被消去)
	Factor summation(vector<string> & varset);
	//设置证据
	Factor setEvidence(unordered_map<string, double>&);

	friend bool operator<(const Factor lhs, const Factor rhs);

	friend class InfEngine;

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
	
	//utils
	uint32_t indexOf(string & name);
	size_t getElemSize(string & name);
	vector<size_t> getElemSize(vector<string> & names);
	//get the same variable in two different set
	static unordered_map<string, pair<int, int> > getUnion(Factor* a, Factor* b);

};
