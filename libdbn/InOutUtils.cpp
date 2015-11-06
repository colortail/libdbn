#pragma once
#include <cstdio>
#include <iostream>
#include "InOutUtils.h"

void InOutUtils::stdPrint(Factor& factor) {
	const vector<string>* pName = factor.getElementsName();
	const vector<vector<double>>* pTable = factor.getElementsTable();
	const vector<bool>* paMarks = factor.getParentsMark();
	printf("%s\n", factor.getFactorType() == CPD ? "条件概率表" : "势函数");
	for (int i = 0; i < pName->size(); i++) {
		printf("%s", (*pName)[i].c_str());
		if (paMarks != NULL && (*paMarks)[i]) {
				printf("(pa)");
		}
		printf("|\t");
	}
	printf("\n");
	for (vector<string>::const_iterator cit = pName->begin(); cit != pName->end(); cit++) {
		printf("--\t", cit->c_str());
	}
	printf("\n");

	for (vector<vector<double>>::const_iterator cit = pTable->begin(); cit != pTable->end(); cit++) {
		int i = 0;
		while (i < cit->size() - 1) {
			printf("%.0f|\t", (*cit)[i++]);
		}
		printf("%.4f\n\n", (*cit)[i]);
	}
}

