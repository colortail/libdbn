#include "Clique.h"

bool operator<(const UndirectEdge & ue1, const UndirectEdge & ue2) {
	if (ue1.i != ue2.i)
		return ue1.i < ue2.i;
	else
		return ue1.j < ue2.j;
}

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

bool Clique::isEqual(set<int>& elems) {
	if (vars.size() == 0 || vars.size() != elems.size())
		return false;
	bool isContain = false;
	for (set<RandVar>::iterator it = vars.begin();
		it != vars.end();
		it++) {

		isContain = false;
		for (set<int>::iterator setIt = elems.begin(); setIt != elems.end(); setIt++) {
			if (it->node == *setIt) {
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
	if (this->vars.size() == 0)
		return false;

	for (set<int>::iterator it = varset.begin();
		it != varset.end();
		it++) {
		RandVar rdVar(*it, string(""));
		if (this->vars.find(rdVar) == vars.end()) {
			return false;
		}
	}
	return true;
}

bool Clique::isContain(vector<string>& varset) {
	if (this->vars.size() == 0)
		return false;
	bool singleContain = false;
	for (vector<string>::iterator it = varset.begin();
		it != varset.end();
		it++) {

		singleContain = false;
		for (set<RandVar>::iterator rdvIt = this->vars.begin();
			rdvIt != this->vars.end();
			rdvIt++) {
			if (rdvIt->name == *it) {
				singleContain = true;
				break;
			}
		}
		if (!singleContain)
			return false;
	}
	return true;
}

void Clique::insert(const RandVar & var) {
	vars.insert(var);
}

void Clique::insert(const Factor & factor) {
	pots.insert(factor);
}

set<Factor>& Clique::getPots() {
	return this->pots;
}

set<string> Clique::containElements(const vector<string> & varset) {
	set<string> elems;
	for (int i = 0; i < varset.size(); i++) {
		for (set<RandVar>::iterator jIt = this->vars.begin();
			jIt != this->vars.end();
			jIt++) {

			if (jIt->name == varset[i])
				elems.insert(varset[i]);
		}
	}
	return elems;
}

Factor Clique::getCliqueInitPotential() {
	Factor factor;
	for (set<Factor>::iterator it = this->pots.begin();
		it != this->pots.end();
		it++) {

		factor = factor.multiply(*it);
	}
	return factor;
}