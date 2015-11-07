#include "BNet.h"


BNet::BNet() :type(BNET) {}

BNet::~BNet() { }

void BNet::moralize() {

	std::vector< std::vector<int> > pa;
	for (int u = 0; u < this->n; u++) {
		pa.push_back(std::vector<int>());
		for (int v = 0; v < this->n; v++) {
			if (exists(v, u)) {
				pa[u].push_back(v);
			}
		}
	}
	for (int u = 0; u < this->n; u++) {
		for (uint32_t i = 0; i < pa[u].size(); i++) {
			for (uint32_t j = 0; j < pa[u].size(); j++) {
				if (i != j && !exists(pa[u][i], pa[u][j])) {
					this->insert(0, 0, pa[u][i], pa[u][j]);
				}
			}
		}
	}

	for (int u = 0; u < this->n; u++) {
		for (int v = firstNbr(u); -1 < v; v = nextNbr(u, v)) {
			if (!this->exists(v, u)) {
				this->insert(0, 0, v, u);
			}
		}
	}

	this->type = MORAL;
}