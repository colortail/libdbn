#pragma once
#include <vector>
#include <string>
#include <bitset>

//CPD条件概率分布，POT势函数
enum FactorType {CPD, POT};

class TableFactor
{
private:
	//条件概率表
	std::vector<std::string> names;
	std::vector<std::vector<double>> cpt; 
	FactorType factorType;
	//变量个数（列），行数
	int varSize;
	int rows;
	//条件概率信息 false 为children， true 为parents P(children | true)
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

