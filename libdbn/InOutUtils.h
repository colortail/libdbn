#pragma once
#include "Factor.h"
#include "BNet.h"

class InOutUtils {
public:
	//static void scanTableFactor(TableFactor&);
	//CPT ����
	static void stdPrint(Factor & factor);

	//��ʾ��������
	static void stdPrintDBnet(BNet & bnet);
};
