#include "Clique.h"

bool Clique::isEqual(vector<string>& elems) {
	if (vars.size() == 0 || vars.size() != elems.size())
		return false;
	bool isContain = false;
	for (set<RandVar>::iterator it = vars.begin();
		it != vars.end();
		it++) {

		isContain = false;
		for (uint32_t j = 0; j < elems.size(); j++) {
			if (it->name == elems[j]) {
				isContain = true;
				break;
			}
		}
		if (isContain)
			continue;
		else
			return false;
	}
	return true;
}

bool Clique::isContain(set<int>& varset) {
	for (set<int>::iterator it = varset.begin();
		it != varset.end();
		it++) {
		if (this->vars.find(RandVar(*it, string(""))) != vars.end()) {
			return false;
		}
	}
	return true;
}

void Clique::insert(RandVar & var) {
	vars.insert(var);
}

void Clique::insert(Factor & factor) {
	pots.insert(factor);
}