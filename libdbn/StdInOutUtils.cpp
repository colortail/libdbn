#pragma once
#include <iostream>
#include "StdInOutUtils.h"

void StdInOutUtils::showTableFactor(TableFactor& table) {
	for (auto it = table.names.begin(); it != table.names.end(); it++) {
		std::cout << it->first<<"\t";
	}
	std::cout << std::endl;

	int probIdx = table.getVarSize();
	for (int i = 0; i < table.cpt.size(); i++) {
		for (auto it = table.names.begin(); it != table.names.end(); it++) {
			std::cout << table.cpt[i][it->second] << "\t";
		}
		std::cout << table.cpt[i][probIdx] << std::endl;
	}
}

