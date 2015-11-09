#pragma once
#include "Factor.h"
#include "BNet.h"
#include "JTree.h"

class InOutUtils {
public:
	//static void scanTableFactor(TableFactor&);
	//CPT ¸ÅÂÊ
	static void stdPrint(const Factor & factor);

	//ÏÔÊ¾ÍøÂçÍØÆË
	static void stdPrintDBnet(BNet & bnet);

	//ÏÔÊ¾Clique
	static void stdPrintClique(Clique & c);
};
