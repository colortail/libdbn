#pragma once
#include <vector>
#include <unordered_map>
#include <set>
#include <string>
#include <cassert>
#include <cstdint>

#include "BNet.h"
#include "utils.h"

using namespace std;
using namespace libdbn;

//CPD�������ʷֲ���POT�ƺ���
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

	//ĳ�����Ƿ����
	bool exists(const string& var) const;

	//varset����Щ��������
	//commonvars �ǽ�����Զ����
	vector<string>& exists(vector<string>& commonvars, const vector<string>& varset) const;

	//utils
	uint32_t indexOf(string & name) const;
	size_t getElemSize(string & name);
	vector<size_t> getElemSize(vector<string> & names);
	vector<double> getProbVector() const;
	uint32_t hashCode();

	//��������
	//��
	Factor multiply(const Factor & a) const;
	//��Ե��(����Ϊ���Ӻͱ�����������ȥ)
	Factor summation(vector<string> & varset) const;
	//����֤��
	Factor setEvidence(unordered_map<string, double>&) const;

	friend bool operator<(const Factor & lhs, const Factor & rhs);

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
	uint32_t hash;

	void newFactor(const vector<string> & elems, const vector<size_t> & elemSize);

	//get the same variable in two different set
	static unordered_map<string, pair<int, int> > getIntersection(const Factor* a, const Factor* b);

};
