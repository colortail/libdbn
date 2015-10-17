#pragma once

//CPD条件概率分布，POT势函数
enum DistType { CPD, POT };

class Distribution{
public:
	virtual DistType distType() = 0;
};