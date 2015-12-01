#pragma once
#include <set>
#include <vector>
#include "BNet.h"
#include "Factor.h"
#include "InOutUtils.h"

using namespace std;
struct RandVar;
class Factor;

struct UndirectEdge {
	uint32_t i;
	uint32_t j;
	UndirectEdge(uint32_t _i, uint32_t _j) {
		i = min(_i, _j);
		j = max(_i, _j);
	}
	friend bool operator<(const UndirectEdge & ue1, const UndirectEdge & ue2);
};

class Clique {
private:
	set<RandVar> vars;
	set<Factor> pots;
	//Factor message;

public:
	bool isEqual(vector<string>&);
	bool isEqual(set<int>&);
	bool isContain(set<int>&);
	bool isContain(vector<string>&);
	void insert(const RandVar & var);
	void insert(const Factor & factor);
	void removeTabular(const Factor & factor);
	void clearTabular();

	set<string> containElements(const vector<string>&);

	set<Factor>& getPots();

	Factor getCliqueInitPotential();

	static set<RandVar> joinElement(const Clique &, const Clique &);

	friend class JTree;
	friend class InOutUtils;
};

