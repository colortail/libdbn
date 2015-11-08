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

//CPD�������ʷֲ���POT�ƺ���
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

	//��������
	//��
	Factor multiply(Factor & a);
	//��Ե��(����Ϊ���Ӻͱ�����������ȥ)
	Factor summation(vector<string> & varset);
	//����֤��
	Factor setEvidence(unordered_map<string, double>&);

	friend bool operator<(const Factor lhs, const Factor rhs);

	friend class InfEngine;

private:
	//Ԫ������
	vector<string> *pName;
	unordered_map<string, int> *pLocation;

	//Ԫ��ȡֵ����
	vector<size_t> *pElemSize;

	//�������ʱ�
	vector<vector<double>> *pTable;

	//�������ͣ��������ʣ��ƺ���
	FactorType factorType;

	//����������Ϣ false Ϊchildren�� true Ϊparents,
	//CPT������ʽΪ��P(children | NULL)
	//����Potential function��ΪNULL
	vector<bool> *pParents;

	//�����������У�������
	uint32_t varSize;
	uint32_t rows;
	
	//utils
	uint32_t indexOf(string & name);
	size_t getElemSize(string & name);
	vector<size_t> getElemSize(vector<string> & names);
	//get the same variable in two different set
	static unordered_map<string, pair<int, int> > getUnion(Factor* a, Factor* b);

};
