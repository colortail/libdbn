#pragma once
#include "Factor.h"
#include "BNet.h"

class InOutUtils {
public:
	//static void scanTableFactor(TableFactor&);
	//CPT ¸ÅÂÊ
	static void stdPrint(Factor & factor);

	//ÏÔÊ¾ÍøÂçÍØÆË
	static void stdPrintDBnet(BNet & bnet);
};
