#include "BNet.h"


BNet::BNet() :type(BNET) {}

BNet::~BNet() { }

BNet& BNet::operator=(const BNet& bn) {
	this->type = bn.type;
	this->e = bn.e;
	this->n = bn.n;

	this->V = bn.V;
	this->E = bn.E;
	for (int i = 0; i < n; i++)
		for (int j = firstNbr(i); -1 < j; j = nextNbr(i, j)) {
			this->E[i][j] = new Edge <double>((bn.E[i][j])->data, (bn.E[i][j])->weight);
		}
	return *this;
}

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

void BNet::introduceEdge(int k, std::vector<bool> & marked) {
	std::vector<int> nbrs;
	if (this->n != marked.size()) {
		throw "添加缺边时，参数不匹配";
	}
	for (int u = firstNbr(k); -1 < u; u = nextNbr(k, u)) {
		if (!marked[u])
			nbrs.push_back(u);
	}

	for (std::vector<int>::iterator iIt = nbrs.begin();
		iIt != nbrs.end();
		iIt++){
		for (std::vector<int>::iterator jIt = nbrs.begin();
			jIt != nbrs.end();
			jIt++) {
			if (*iIt != *jIt && !this->exists(*iIt, *jIt)) {
				this->insert(0, 0, *iIt, *jIt);
			}
		}
	}
}

void BNet::triangulate() {

}