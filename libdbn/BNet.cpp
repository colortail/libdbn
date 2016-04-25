#include "BNet.h"

bool operator<(const RandVar & var1, const RandVar & var2) {
	return var1.node < var2.node;
}

BNet::BNet() :type(StructType::BNET), pJTree(NULL) {}

BNet::~BNet() {
	if (pJTree != NULL) {
		delete pJTree;
		pJTree = NULL;
	}
}

BNet::BNet(const BNet& bn) {
	this->type = bn.type;
	this->cpts = bn.cpts;

	this->e = bn.e;
	this->n = bn.n;

	this->V = bn.V;
	this->E = bn.E;
	if (bn.pJTree != NULL)
		this->pJTree = new JTree(*(bn.pJTree));
	else
		this->pJTree = NULL;

	for (int i = 0; i < n; i++)
		for (int j = firstNbr(i); -1 < j; j = nextNbr(i, j)) {
			this->E[i][j] = new Edge <double>((bn.E[i][j])->data, (bn.E[i][j])->weight);
		}
}

BNet& BNet::operator=(const BNet& bn) {
	this->type = bn.type;
	this->cpts = bn.cpts;

	this->e = bn.e;
	this->n = bn.n;

	this->V = bn.V;
	this->E = bn.E;

	if (bn.pJTree != NULL)
		this->pJTree = new JTree(*(bn.pJTree));
	else
		this->pJTree = NULL;

	for (int i = 0; i < n; i++)
		for (int j = firstNbr(i); -1 < j; j = nextNbr(i, j)) {
			this->E[i][j] = new Edge <double>((bn.E[i][j])->data, (bn.E[i][j])->weight);
		}
	return *this;
}

void BNet::moralize() {

	if (this->type == MORAL)
		return;

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

void BNet::setCPTs(const std::set<Factor>& _cpts) {
	if (_cpts.size() == this->n)
		this->cpts = _cpts;
	else
		throw exception("设置有误，条件概率表个数应与网络节点个数一致");
}

std::set<int> BNet::getAllNbrs(const std::set<int>& restNode, int u) {
	set<int> nbrs;
	for (int v = firstNbr(u); -1 < v; v = nextNbr(u, v)) {
		if (restNode.find(v) != restNode.end())
			nbrs.insert(v);
	}
	return nbrs;
}

void BNet::addFillEdge(int i, std::set<int> & restNode) {
	vector<int> nbrs;
	for (int j = firstNbr(i); -1 < j; j = nextNbr(i, j)) {
		if (restNode.find(j) != restNode.end())
			nbrs.push_back(j);
	}
	for (uint32_t k = 0; k < nbrs.size(); k++) {
		for (uint32_t w = 0; w < nbrs.size(); w++) {
			if (k != w && !exists(nbrs[k], nbrs[k]))
				this->insert(0, 0, nbrs[k], nbrs[w]);
		}
	}
}

std::vector<string> BNet::getAllNodesName() {
	vector<string> names;
	for (int i = 0; i < this->n; i++) {
		names.push_back(this->vertex(i).name);
	}
	return names;
}

std::set<Factor> BNet::getCPTs() const {
	return this->cpts; 
}

void BNet::correctNode() {
	for (int i = 0; i < this->n; i++)
		this->vertex(i).node = i;
}

void BNet::setnNbrs(std::map<int, std::set<int> > & map) {
	for (int i = 0; i < n; i++) {
		set<int> nbrs;
		for (int j = firstNbr(i); 0 <= j; j = nextNbr(i, j)) {
			nbrs.insert(j);
		}
		map.insert({ i, nbrs });
	}
	
}

void BNet::setJTree(JTree * _pJTree) {
	
	if (this->pJTree != NULL)
		delete this->pJTree;
	this->pJTree = _pJTree;

}

JTree * BNet::getJTree() {
	return this->pJTree;
}

int BNet::getVarRange(int i) const {
	return V[i].data.range;
}

int BNet::getPaVarsRange(int i) const {
	int paRange = 1;
	for (int pa = 0; pa < n; pa++) {
		//replace exists() since const property
		if ((0 <= i) && (i < n) && (0 <= pa) && (pa < n) && E[pa][i] != NULL) {
			paRange *= V[pa].data.range;
		}
	}
	return paRange;
}

vector<int> BNet::getPaVars(int i) const {
	vector<int> paVars;
	for (int pa = 0; pa < n; pa++) {
		//replace exists() since const property
		if ((0 <= i) && (i < n) && (0 <= pa) && (pa < n) && E[pa][i] != NULL) {
			paVars.push_back(pa);
		}
	}
	return paVars;
}

void BNet::insertCPT(const Factor & factor) {
	this->cpts.insert(factor);
}

void BNet::removeCPT(const Factor & factor) {
	this->cpts.erase(factor);
}

int BNet::getNodeIndex(std::string & name) {
	for (int i = 0; i < this->n; i++)
		if (name == this->V[i].data.name)
			return i;
	return -1;
}