#pragma once
#include <vector>
#include <unordered_map>
#include <set>
#include <string>
#include <cassert>
#include <cstdint>

using namespace std;

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

	const vector<string>* getElementsName() {
		return pName;
	}

	const vector<vector<double>>* getElementsTable() {
		return pTable;
	}

	const vector<bool>* getParentsMark() {
		return this->pParents;
	}

	void setFactorType(FactorType type) {
		this->factorType = type;
	}

	void setParents(const vector<string> & parents);
	void setProb(const vector<double> &);

	//��������
	//��
	Factor multiplyFactor(Factor & a);
	//��Ե��(����Ϊ���Ӻͱ�����������ȥ)
	Factor summation(vector<string> & varset);
	//����֤��
	Factor setEvidence(unordered_map<string, double>&);

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
