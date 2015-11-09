#include "Factor.h"
#include "InOutUtils.h"

class Clique {
private:
	set<RandVar> vars;
	set<Factor> pots;

public:
	bool isEqual(vector<string>&);
	bool isContain(set<int>&);
	void insert(RandVar & var);
	void insert(Factor & factor);

	friend class InOutUtils;
};