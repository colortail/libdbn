#pragma once
#include <vector>
#include <string>
#include <bitset>

//CPD�������ʷֲ���POT�ƺ���
enum FactorType {CPD, POT};

class TableFactor
{
private:
	//�������ʱ�
	std::vector<std::string> names;
	std::vector<std::vector<double>> cpt; 
	FactorType factorType;
	//�����������У�������
	int varSize;
	int rows;
	//����������Ϣ false Ϊchildren�� true Ϊparents P(children | true)
	std::vector<bool> parents;
public:
	TableFactor();
	~TableFactor();

	TableFactor operator*(TableFactor &);
	TableFactor operator/(TableFactor &);

	TableFactor marginalize(std::vector<int>&);
	TableFactor maxMarginalize(std::vector<int>&);

	TableFactor absorbEvidence(std::vector<int>&, std::vector<double>& evidence);

};

