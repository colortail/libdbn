#pragma once

//CPD�������ʷֲ���POT�ƺ���
enum DistType { CPD, POT };

class Distribution{
public:
	virtual DistType distType() = 0;
};