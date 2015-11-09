#pragma once
#include "Factor.h"
#include "BNet.h"
#include "JTree.h"

class InOutUtils {
public:
	//static void scanTableFactor(TableFactor&);
	//CPT ����
	static void stdPrint(const Factor & factor);

	//��ʾ��������
	static void stdPrintDBnet(BNet & bnet);

	//��ʾClique
	static void stdPrintClique(Clique & c);
};
