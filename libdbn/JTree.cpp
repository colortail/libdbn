#include "JTree.h"


JTree::JTree() : root(-1)
{
}


JTree::~JTree()
{
}

JTree::JTree(const JTree & copy) {
	this->E = copy.E;
	this->e = copy.e;
	this->location = copy.location;
	this->n = copy.n;
	this->root = copy.root;
	this->V = copy.V;

	for (vector< vector< Edge < Factor > * > >::iterator rowIt = this->E.begin();
		rowIt != E.end();
		rowIt++) {
		for (vector< Edge < Factor > * >::iterator colIt = rowIt->begin();
			colIt != rowIt->end();
			colIt++) {
			if (*colIt != NULL)
				*colIt = new Edge<Factor>((*colIt)->data, 0);
		}
	}
}

JTree& JTree::operator=(const JTree & copy) {
	this->E = copy.E;
	this->e = copy.e;
	this->location = copy.location;
	this->n = copy.n;
	this->root = copy.root;
	this->V = copy.V;

	for (vector< vector< Edge < Factor > * > >::iterator rowIt = this->E.begin();
		rowIt != E.end();
		rowIt++) {
		for (vector< Edge < Factor > * >::iterator colIt = rowIt->begin();
			colIt != rowIt->end();
			colIt++) {
			if (*colIt != NULL)
				*colIt = new Edge<Factor>((*colIt)->data, 0);
		}
	}
	return *this;
}

vector<int> JTree::findClique(set<int> & varset) {
	vector<int> cliques;
	for (int i = 0; i < this->n; i++) {
		if (this->vertex(i).isContain(varset))
			cliques.push_back(i);
	}
	
	return cliques;
}

void JTree::maxContain(const vector<string> & varset, 
	std::pair<int, int> & result,
	int c,
	vector<bool>& marked) {

	if (marked[c] == true) {
		return;
	}

	marked[c] = true;

	int cnt = this->vertex(c).containElements(varset).size();

	if (result.second < cnt) {
		result.first = c;
		result.second = cnt;
	}

	if (cnt == varset.size())
		return;

	for (int u = firstNbr(c); -1 < u; u = nextNbr(c, u)) {
		if (!marked[u]) {
			maxContain(varset, result, u, marked);
		}
	}
}

vector<int> JTree::findCoverClique(vector<string> varset) {
	vector<int> cliques;
	
	while (!varset.empty()) {
		std::pair<int, int> state;
		maxContain(varset, state, 0, vector<bool>(this->n, false));

		set<string> coverElem = this->vertex(state.first).containElements(varset);
		cliques.push_back(state.first);

		libdbn::removeElements(varset, coverElem);
	}
	return cliques;
}

void JTree::setProb(const Factor & factor) {
	vector<string>* pName = const_cast<vector<string>*>(factor.getElementsName());
	set<int> varset;
	
	if (pName != NULL) {
		for (uint32_t k = 0; k < pName->size(); k++) {
			varset.insert(this->indexOf((*pName)[k]));
		}
		vector<int> belongeds = this->findClique(varset);
		
		//任取其一（这里取最后一个）
		if (belongeds.size() > 0) {
			
			this->vertex(belongeds[belongeds.size() - 1]).insert(factor);
		}
	}
}

//根节点任选
int JTree::getRoot(vector<string> & queryset) {
	if (this->n <= 0)
		return -1;

	for (int i = 0; i < this->n; i++) {
		if (this->vertex(i).isContain(queryset)) {
			return i;
		}
	}
	//没有包含所有查询变量的clique就取第一个为根
	return 0;
}

Factor JTree::getCliquePotential(int c) {
	set<Factor> * pVarSet = &(this->vertex(c).pots);

	Factor factor;

	for (set<Factor>::iterator facIt = pVarSet->begin();
		facIt != pVarSet->end();
		facIt++) {

		factor = factor.multiply(*facIt);
	}

	for (int u = firstNbr(c); -1 < u; u = nextNbr(c, u)) {
		factor = factor.multiply(this->edge(c,u));
	}
	return factor;
}

void JTree::collectMessage(int u, int v) {

	for (int nbr = firstNbr(v); -1 < nbr; nbr = nextNbr(v, nbr)) {
		//叶子节点会退出，因为只有一个邻居
		if (nbr != u) {
			collectMessage(v, nbr);
		}
	}
	sendMessage(v, u);

	char msg[5];
	sprintf_s(msg, "%d->%d", v, u);
	BenchMark::debugStdOut(string("collect"), true, msg);
}

void JTree::distributeMessage(int u, int v) {
	sendMessage(u, v);

	char msg[5];
	sprintf_s(msg, "%d->%d", u, v);
	BenchMark::debugStdOut(string("distribute"), true, msg);

	for (int nbr = firstNbr(v); -1 < nbr; nbr = nextNbr(v, nbr)) {
		//叶子节点会退出，因为只有一个邻居
		if (nbr != u) {
			distributeMessage(v, nbr);
		}
	}
}

void JTree::sendMessage(int u, int v) {

	if (!exists(u, v))
		return;
	Factor separator = this->edge(u, v);
	vector<string> sepNames = *separator.getElementsName();
	
	separator = separator.multiply(this->vertex(u).getCliqueInitPotential());

	for (int nbr = firstNbr(u); -1 < nbr; nbr = nextNbr(u, nbr)) {
		if (nbr != v) {
			separator = separator.multiply(retrieveMessage(u, nbr));
		}
	}
	
	vector<string> completeSet = *separator.getElementsName();
	vector<string> supSet = libdbn::get2VectorSubstract(completeSet, sepNames);
	separator = separator.summation(supSet);
	saveMessage(u, v, separator);
}

Factor JTree::retrieveMessage(int v, int u) {
	if (exists(v, u)) {
		return this->edge(v, u);
	}
	else
		return Factor();
}

void JTree::saveMessage(int u, int v, const Factor& pot) {
	
	vector<double> probs = pot.getProbVector();

	Factor & separatorLeft = this->edge(u, v);
	Factor & separatorRight = this->edge(v, u);
	
	//有一种情况，一个clique没有分到任何CPT
	//且它是一个叶子节点,是的，就是这么坑爹，那么这个pot实际可以去掉了
	//且separator的值全为1
	if (pot.getVarSize() == 0) {
		vector<double> v(separatorLeft.getRowsSize(), 1);
		separatorLeft.setProb(v);
		separatorRight.setProb(v);
		return;
	}

	assert(separatorLeft.getRowsSize() == pot.getRowsSize());
	assert(separatorRight.getRowsSize() == pot.getRowsSize());

	separatorLeft.setProb(probs);
	separatorRight.setProb(probs);
}


int JTree::insertClique(Clique const &vertex) {

	Clique & c = const_cast<Clique&>(vertex);

	for (set<RandVar>::iterator it = c.vars.begin(); 
		it != c.vars.end();
		it++) {
		
		this->location.insert({ it->name, it->node });
	}

	return GraphMatrix<Clique, Factor>::insert(vertex);
}

void JTree::insert(Factor const &edge, int w, int i, int j) {
	return GraphMatrix<Clique, Factor>::insert(edge, w, i, j);
}

int JTree::indexOf(string & name) {
	
	unordered_map<string, int>::iterator it = location.find(name);
	if (it != location.end()) {
		return it->second;
	}
	return -1;
}

void JTree::clearTabular() {
	for (int i = 0; i < n; i++) {
		this->vertex(i).clearTabular();
	}
}

vector<pair<int, int> > JTree::findCoverSeparator(vector<string> & query) {
	
	bool cover = false;
	vector<pair<int, int> > coveredSep;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (exists(i, j)) {
				vector<string>* names = this->edge(i, j).getElementsName();
				cover = false;
				for (vector<string>::iterator pN = names->begin();
					pN != names->end();
					pN++) {
					
					for (vector<string>::iterator qIt = query.begin();
						qIt != query.end(); ) {

						if (*qIt == *pN) {
							cover = true;
							qIt = query.erase(qIt);
						}
						else
							qIt++;
					}
				}
				if (cover) {
					coveredSep.push_back(pair<int, int>(i, j));

					if (query.empty())
						return coveredSep;
				}
			}
		}
	}
	return coveredSep;
}