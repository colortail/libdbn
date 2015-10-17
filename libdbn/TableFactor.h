#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <bitset>

#include "Distribution.h"
#include "StdInOutUtils.h"

class TableFactor : public Distribution
{
private:
	//�������ʱ�
	std::unordered_map<std::string, int> names;

	std::vector<std::vector<double>> cpt; 

	//����������Ϣ false Ϊchildren�� true Ϊparents,
	//CPTһ����ʽΪ��P(children | true)
	std::vector<bool> parents;

	//�����������У�������
	int varSize;
	int rows;
	DistType factorType;

public:
	TableFactor();
	TableFactor(int);
	~TableFactor();
	DistType distType() { return factorType; }
	int getVarSize() { return varSize; }
	int getRowSize() { return rows; }
	
	void pushItem(std::vector<double>);

	TableFactor operator*(TableFactor &);
	TableFactor marginalize(std::vector<std::string>&);
	TableFactor operator/(TableFactor &);
	TableFactor& absorbEvidence(std::unordered_map<std::string, double>&);
	
	friend class StdInOutUtils;

	//������
	friend TableFactor prod(TableFactor &, TableFactor &);
	//��Ե���������Ԫ
	friend TableFactor marginalize(TableFactor &, std::vector<std::string>&);
	//friend TableFactor maxMarginalize(TableFactor &, std::vector<std::string>&);
	//��
	friend TableFactor div(TableFactor &, TableFactor &);
	//����֤��
	friend TableFactor& absorbEvidence(TableFactor &, std::unordered_map<std::string, double>&);

	friend std::unordered_map < std::string, std::vector<int> >commonVariables(TableFactor& a, TableFactor& b);

	friend TableFactor mergeTableFactor(TableFactor& a, TableFactor& b);
};

/*������*/
std::unordered_map < std::string, std::vector<int> > 
commonVariables(TableFactor& a, TableFactor& b){
	std::unordered_map < std::string, std::vector<int> > commonMap;
	int i = 0;
	for (auto itor1 = a.names.begin(); itor1 != a.names.end(); itor1++, i++) {
		int j = 0;
		for (auto itor2 = b.names.begin(); itor2 != b.names.end(); itor2++, j++)
			if (itor1->first == itor2->first) {
				std::pair<std::string, std::vector<int> > col(itor1->first, std::vector<int>(i, j));
				commonMap.insert(col);
			}
	}
	return commonMap;
}

/*�ϲ�����CPT,��CPTΪ��*/
TableFactor mergeTableFactor(TableFactor& a, TableFactor& b) {
	std::unordered_map < std::string, std::vector<int> > commonCols = commonVariables(a, b);
	TableFactor factor(a.getVarSize() + b.getVarSize() - commonCols.size());
	int factorSize = 0;
	for (auto it = a.names.begin(); it != a.names.end(); it++) {
		std::pair<std::string, int > colDef(it->first, factorSize);
		factor.names.insert(colDef);
		factorSize++;
	}

	for (auto it = b.names.begin(); it != b.names.end(); it++) {
		if (commonCols.find(it->first) == commonCols.end()) {
			std::pair<std::string, int > colDef(it->first, factorSize);
			factor.names.insert(colDef);
			factorSize++;
		}
	}
	factor.varSize = factorSize;
	
	return factor;
}

TableFactor prod(TableFactor & a, TableFactor & b) {
	std::unordered_map < std::string, std::vector<int> > commonCols = commonVariables(a, b);
	TableFactor factor = mergeTableFactor(a, b);

	for (int i = 0; i < a.getVarSize(); i++)
		for (int j = 0; j < b.getVarSize(); j++) {
			bool eq = true;
			for (auto eqItor = commonCols.begin(); eqItor != commonCols.end(); eqItor++) {
				if (a.cpt[i][(eqItor->second)[0]] != b.cpt[j][(eqItor->second)[1]]) {
					eq = false;
					break;
				}
			}
			
			if (eq) {
				std::vector<double> newCol(factor.getVarSize() + 1);
				for (auto newColIt = factor.names.begin(); newColIt != factor.names.end(); newColIt++) {
					std::unordered_map<std::string, int>::iterator flagItor;  
					if ((flagItor = a.names.find(newColIt->first)) != a.names.end()) {
						//��a�е���
						newCol[newColIt->second] = a.cpt[i][flagItor->second];
					}
					else if ((flagItor = b.names.find(newColIt->first)) != b.names.end()) {
						//��b�е���
						newCol[newColIt->second] = b.cpt[j][flagItor->second];
					}
				}
				//producted probability
				newCol[factor.getVarSize()] = a.cpt[i][a.getVarSize()] * b.cpt[j][b.getVarSize()];
				factor.pushItem(newCol);
			}
		}
}

//��Ե���������Ԫ
TableFactor marginalize(TableFactor &, std::vector<std::string>&) {

}

//TableFactor maxMarginalize(TableFactor &, std::vector<int>&) { }

//��
TableFactor div(TableFactor &, TableFactor &) {

}

//����֤��
TableFactor& absorbEvidence(TableFactor &, std::unordered_map<std::string, double>&) {

}
