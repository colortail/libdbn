#include "InfEngine.h"

InfEngine* InfEngine::m_pInstance = NULL;

Factor InfEngine::inference(
	BNet & bnet,
	vector<string> & queryset, 
	unordered_map<string, double> & evidset,
	InferStrategy & strategy) {

	return strategy(bnet, queryset, evidset, this);
}

vector<int> InfEngine::greedyOrdering(const BNet& moral, Metric& metric) {
	vector<int> pi(moral.vertexSize(), -1);
	vector<bool> marked(moral.vertexSize(), false);

	BNet tmpMoral;
	tmpMoral = moral;

	for (uint32_t i = 0; i < moral.vertexSize(); i++) {
		int k = metric(tmpMoral, marked);
		pi[i] = k;
		//introduce all neighbour of variable(k)
		tmpMoral.introduceEdge(k, marked);

		marked[k] = true;
	}
	return pi;
}

//最大势，最多已标记节点的未标记元素
int InfEngine::getMaxCardinalityElem(const BNet & moral, const vector<bool> & marked) {
	BNet& m = const_cast<BNet&>(moral);

	if (m.vertexSize() != marked.size())
		return -1;

	int cnt = 0;
	int root[2] = { -1, -1 };
	for (uint32_t i = 0; i < m.vertexSize(); i++) {
		if (marked[i])
			continue;
		cnt = 0;
		for (int j = m.firstNbr(i); -1 < j; j = m.nextNbr(i, j)) {
			if (marked[j])
				cnt++;
		}
		if (root[1] < cnt) {
			root[0] = i;
			root[1] = cnt;
		}
	}
	return root[0];
}

/*
最大势搜索：按如下规则从大到小编号
在第i步中，选择未编号节点中拥有最多已编号相邻节点（多个时任选一）
将其编号为 n - i - 1
所有节点均编号后，编号由小到大排序得到消元顺序
*/
vector<int> InfEngine::maxCardinalitySearch(const BNet& moral, int root) {
	vector<int> pi(moral.vertexSize(), -1);
	vector<bool> marked(moral.vertexSize(), false);

	int k = moral.vertexSize() - 1;
	pi[k--] = root;
	marked[root] = true;

	while (-1 < k) {
		root = getMaxCardinalityElem(moral, marked);
		pi[k--] = root;
		marked[root] = true;
	}
	
	return pi;
}

set<Factor>& InfEngine::setEvidence(set<Factor>& factorset, unordered_map<string, double> & evidset) {
	vector<string> evidence;
	for (auto it = evidset.begin();
		it != evidset.end();
		it++) {
		evidence.push_back(it->first);
	}

	vector<string> commonvars;
	set<Factor> evidsetted;
	set<Factor>::iterator setIt = factorset.begin();

	while (setIt != factorset.end()) {
		commonvars = setIt->exists(commonvars, evidence);
		if (!commonvars.empty()) {
			evidsetted.insert(setIt->setEvidence(evidset));
			setIt = factorset.erase(setIt);
		}
		else {
			setIt++;
		}
	}
	
	factorset.insert(evidsetted.begin(), evidsetted.end());
	return factorset;
}

/*
1 F <- Bnet中概率分布的集合
2 F的因子中，将证据全部设置为观测值
3 while pi != \emptyset
    Z为pi中最前面的变量，将其从pi中删去
	F <-eliminate(F, Z)
4 将F中所有因子相乘得到一个queryset的一个函数h(q) 
5 return h(q) / \sum_Q{h(q)} 
*/
Factor InfEngine::variableElim(BNet& bnet, 
	const vector<string> & queryset, 
	unordered_map<string, double> & evidset, //no reason
	const vector<int> & pi) {
	
	set<Factor> factorset = bnet.getCPTs();
	Factor result;
	if (pi.size() <= bnet.vertexSize()) {
		//set evidence
		factorset = this->setEvidence(factorset, evidset);
		//do elimination
		vector<string> resultVars = libdbn::getVectorMAPUnion(queryset, evidset);
		for (uint32_t i = 0; i < pi.size(); i++) {
			//这里乘的是所有包含变量 bnet.vertex(pi[i]).name 的所有因子
			if (!libdbn::inVector(resultVars, bnet.vertex(pi[i]).name)) {
				factorset = eliminate(factorset, vector<string>(1, bnet.vertex(pi[i]).name));
			}
		}
		//multipy the rest factor
		for (set<Factor>::iterator setIt = factorset.begin();
			setIt != factorset.end();
			setIt++) {
			result = result.multiply(*setIt);
		}
		//normalization
		result.normalize();
	}
	return result;
}

/*
1 从factorset中删去所有涉及Z的函数 { f_1, ... , f_k }
2 g <- \prod_(i=1)^k{f_i}
3 h <- \sum_Z{g}
4 将h放回factorset
5 return factorset
*/
set<Factor>& InfEngine::eliminate(set<Factor>& factorset, vector<string>& elimvars) {
	vector<string> commonvars;
	set<Factor>::iterator setIt = factorset.begin();
	
	Factor cpt;
	//product
	while (setIt != factorset.end()) {
		commonvars = setIt->exists(commonvars, elimvars);
		if (!commonvars.empty()) {
			cpt = cpt.multiply(*setIt);
			setIt = factorset.erase(setIt);
		}
		else
			setIt++;
	}

	//sum
	cpt = cpt.summation(elimvars);

	setIt = factorset.find(cpt);
	if (setIt != factorset.end()) {
		cpt = cpt.multiply(*setIt);
	}
	factorset.insert(cpt);
	return factorset;
}
/*
1 从 pi中删去第一个元素Z
2 S<-nb(Z) C<-{Z} \and S
3 if (C 包含 bnet中所有节点)
    return 以C为唯一节点的JTree
4 else
    bnet<-Elim(bnet, Z)
    Jtree<- buildJTree(bnet, pi)
5 在J中找到一个包含S的团 C',将C与C'相连
6 return J
*/
JTree& InfEngine::buildJTree(JTree & jtree, BNet & moral, vector<int> & pi) {
	
	if (moral.getStructType() != MORAL)
		moral.moralize();

	if (pi.size() != moral.vertexSize())
		return jtree;

	BNet inducedGraph;
	inducedGraph = moral;

	inducedGraph.moralize();
	set<int> restNode;
	for (uint32_t i = 0; i < moral.vertexSize(); i++)
		restNode.insert(i);

	return buildJTree(jtree, inducedGraph, pi, restNode, 0);
}

JTree& InfEngine::buildJTree(JTree & jtree, BNet & induced, vector<int> & pi, set<int> & restNode, int i) {
	if (pi.size() <= (uint32_t)i)
		return jtree;
	int currentElem = pi[i];
	Clique clique;
	set<int> nbrs = induced.getAllNbrs(restNode, currentElem);

	clique.insert(induced.vertex(currentElem));

	for (set<int>::const_iterator nbrIt = nbrs.begin();
		nbrIt != nbrs.end();
		nbrIt++) {
		if (restNode.find(*nbrIt) != restNode.end())
			clique.insert(induced.vertex(*nbrIt));
	}
	
	int c = jtree.insertClique(clique);
	c--;

	if (clique.isEqual(restNode)) {
		return jtree;
	}
	else {
		induced.addFillEdge(currentElem, restNode);
		//induced.remove(i);
		restNode.erase(currentElem);
		jtree = buildJTree(jtree, induced, pi, restNode, i + 1);

	}
	vector<int> cp = jtree.findClique(nbrs);

	if (!cp.empty()) {
		for (uint32_t i = 0; i < cp.size(); i++) {
			if (cp[i] == c)
				continue;
			
			//separator被多个clique满足时
			//任取一个clique即可,根据不同的条件可能产生不同的junction tree
			//（此处取最先生成的那个clique）
			vector<string> separator;
			vector<size_t> separatorSize;
			for (set<int>::iterator w = nbrs.begin(); w != nbrs.end(); w++) {
				separator.push_back(induced.vertex(*w).name);
				separatorSize.push_back(induced.vertex(*w).range ? induced.vertex(*w).range : 2);
			}

			Factor sepFactor(separator, separatorSize);
			sepFactor.setProb(vector<double>(sepFactor.getRowsSize(), 1));

			jtree.insert(sepFactor, 0, c, cp[i]);
			jtree.insert(sepFactor, 0, cp[i], c);
			break;
		}
	}
	return jtree;
}

void InfEngine::initJTreeCPD(JTree & jtree, const BNet & bnet) {
	set<Factor> factorset = bnet.getCPTs();
	jtree.clearTabular();
	for (set<Factor>::iterator setIt = factorset.begin();
		setIt != factorset.end();
		setIt++) {
		jtree.setProb(*setIt);
	}
}

void InfEngine::setEvidence(JTree & jtree,const BNet & bnet, unordered_map<string, double> & evidset) {

	//this->consistence == false, 为了判断是否**确实**没有更新证据
	if (!this->consistence && evidCache.size() == evidset.size()) {
		for (unordered_map<string, double>::iterator it = evidCache.begin();
			it != evidCache.end();
			it++) {

			unordered_map<string, double>::iterator target = evidset.find(it->first);
			if (target == evidset.end() || target->second != it->second) {
				//表示实际更新了证据，修正consistence
				evidCache = evidset;
				this->consistence = true;
				break;
			}
		}
		
		if (!this->consistence)
			return;
	}

	this->initJTreeCPD(jtree, bnet);

	for (uint32_t i = 0; i < jtree.getVertexSize(); i++) {
		
		this->setEvidence(jtree.vertex(i).getPots(), evidset);
	}

	
}

//Shafer Shenoy Algorithm
void InfEngine::messagePropagation(BNet & bnet,
	JTree & jtree,
	vector<string> & queryset, 
	unordered_map<string, double>& evidset) {
	
	int root = jtree.getRoot(queryset);

	//collection phase
	for (int nbr = jtree.firstNbr(root); -1 < nbr; nbr = jtree.nextNbr(root, nbr)) {
		jtree.collectMessage(root, nbr);
	}

	//distribution phase
	for (int nbr = jtree.firstNbr(root); -1 < nbr; nbr = jtree.nextNbr(root, nbr)) {
		jtree.distributeMessage(root, nbr);
	}

	this->consistence = false;
	
}


Factor InfEngine::getTabularFromJTree(BNet & bnet,
	JTree & jtree,
	vector<string> & queryset,
	unordered_map<string, double>& evidset) {

	vector<string> tmpQueryset = queryset;
	//posterior computaion
	Factor posterior;

	/* 
	//发现该算法有误的问题所在
	//是因为分隔集中的概率才达到了一致

	vector<int> cqs = jtree.findCoverClique(queryset);
	//覆盖查询变量的所有团
	for (uint32_t i = 0; i < cqs.size(); i++) {
		posterior = posterior.multiply(jtree.getCliquePotential(cqs[i]));
	}
	vector<string> querySup = libdbn::get2VectorSubstract(*posterior.getElementsName(), queryset);
	return posterior.summation(querySup).normalize();
	*/
	vector<pair<int, int> > seps = jtree.findCoverSeparator(tmpQueryset);
	for (int i = 0; i < seps.size(); i++) {
		posterior = posterior.multiply(jtree.edge((seps[i]).first, (seps[i]).second));
	}
	//tmpQueryset为空表分隔集可以完整覆盖查询变量，否则无法覆盖
	if (!tmpQueryset.empty()) {

		vector<int> cqs = jtree.findCoverClique(tmpQueryset);
		//覆盖查询变量的所有团
		for (uint32_t i = 0; i < cqs.size(); i++) {
			Factor pots = jtree.getCliquePotential(cqs[i]);
			vector<string> singleCliqueSup = libdbn::get2VectorSubstract(*pots.getElementsName(), tmpQueryset);
			
			posterior = posterior.multiply(pots.summation(singleCliqueSup));
		}

	}
	//vector<string> querySup = libdbn::get2VectorSubstract(*posterior.getElementsName(), queryset);
	//return posterior.summation(querySup).normalize();
	return posterior.normalize();
}

//Hugin Algorithm
Factor InfEngine::messagePassing(BNet & bnet,
	vector<string> & queryset,
	unordered_map<string, double>& evidset) { 
	return Factor();
}


BNet InfEngine::triangulate(const BNet & bnet, vector<int>& pi) {
	BNet moral;
	moral = bnet;

	if (moral.getStructType() != MORAL)
		moral.moralize();

	set<int> restNode;
	for (uint32_t i = 0; i < moral.vertexSize(); i++) {
		restNode.insert(i);
	}
	for (uint32_t i = 0; i < pi.size(); i++) {
		set<int> nbrs = moral.getAllNbrs(restNode, pi[i]);
		for (set<int>::iterator iIt = nbrs.begin(); iIt != nbrs.end(); iIt++) {
			for (set<int>::iterator jIt = nbrs.begin(); jIt != nbrs.end(); jIt++) {
				if (*iIt != *jIt && !moral.exists(*iIt, *jIt)) {
					moral.insert(0, 0, *iIt, *jIt);
				}
			}
		}
		restNode.erase(pi[i]);
	}
	return moral;
}


vector<Clique>* InfEngine::findCliques(BNet & triGraph) {
	vector<Clique> * p = new vector<Clique>();
	return p;
}

void InfEngine::BronKerboschRecursive(map<int, set<int> > & nnbrs,
	set<int> & cand,
	set<int> & done,
	vector<int> & sofar,
	vector< vector<int> > & clique) {

	//find pivot mode
	int maxconn = -1;
	int numb_cand = cand.size();
	set<int> pivotnbrs;

	for (set<int>::iterator nIt = done.begin();
		nIt != done.end();
		nIt++) {
		map<int, set<int>>::iterator nbrIt = nnbrs.find(*nIt);
		if (nbrIt == nnbrs.end())
			continue;

		set<int> cn = setIntersection(cand, nbrIt->second);
		int conn = cn.size();
		if (conn > maxconn) {
			pivotnbrs = cn;
			maxconn = conn;
			if (conn == numb_cand) {
				// all possible cliques already found
				return;
			}
		}
	}

	for (set<int>::iterator nIt = cand.begin();
		nIt != cand.end();
		nIt++) {

		map<int, set<int> >::iterator nbrIt = nnbrs.find(*nIt);
		if (nbrIt == nnbrs.end())
			continue;

		set<int> cn = setIntersection(cand, nbrIt->second);
		int conn = cn.size();
		if (conn > maxconn) {
			pivotnbrs = cn;
			maxconn = conn;
		}
	}

	set<int> smallcand = setSub(cand, pivotnbrs);

	for (set<int>::iterator nIt = smallcand.begin();
		nIt != smallcand.end();
		nIt++) {
		cand.erase(*nIt);
		sofar.push_back(*nIt);

		map<int, set<int> >::iterator nbrIt = nnbrs.find(*nIt);
		if (nbrIt == nnbrs.end())
			continue;

		set<int> newCand = setIntersection(cand, nbrIt->second);
		set<int> newDone = setIntersection(done, nbrIt->second);

		if (newCand.size() == 0 && newDone.size() == 0) {
			//find the clique
			clique.push_back(sofar);
		}
		else if (newDone.size() == 0 && newCand.size() == 1) {
			vector<int> concatsofar(sofar);
			for (set<int>::iterator it = newCand.begin();
				it != newCand.end();
				it++) {

				concatsofar.push_back(*it);
			}
			clique.push_back(concatsofar);
		}
		else {
			BronKerboschRecursive(nnbrs,
				newCand,
				newDone,
				sofar,
				clique);
		}
		done.insert(sofar.back());
		sofar.pop_back();
	}

}

vector<Clique>* InfEngine::findCliquesRecursive(BNet & triGraph) {

	vector< vector<int> > cliques;
	vector<Clique>* result = new vector<Clique>();

	vector<int> sofar;
	map<int, set<int>> nnbrs;
	set<int> cand;
	set<int> done;

	triGraph.setnNbrs(nnbrs);

	if (nnbrs.size() == 0)
		return result;

	for (uint32_t i = 0; i < triGraph.vertexSize(); i++) {
		cand.insert(i);
	}

	BronKerboschRecursive(nnbrs, cand, done, sofar, cliques);

	//construct Clique
	for (vector< vector<int> >::iterator it = cliques.begin();
		it != cliques.end();
		it++) {
		Clique c;
		for (vector<int>::iterator eIt = it->begin();
			eIt != it->end();
			eIt++) {

			c.insert(triGraph.vertex(*eIt));
		}
		result->push_back(c);
	}
	
	return result;
}
/*
inspired by BNT(from Prof. K.Murphy)

Find an optimal elimination ordering (NP-hard problem!)
triangulate
Connect the cliques up into a jtree,
*/
void InfEngine::graphToJTree(JTree & jtree, BNet & triGraph) {

	vector<Clique>* cliqs = findCliquesRecursive(triGraph);

	set<int> U;
	set<int> V;

	int max = -1;
	pair<int, int> edge;
	int cnt = 0;

	for (uint32_t i = 0; i < cliqs->size(); i++) {
		jtree.insertClique(cliqs->at(i));
		U.insert(i);
	}

	if (U.size() != 0) {
		V.insert(*(U.begin()));
		U.erase(U.begin());
	}

	while (U.size()) {
		max = -1;
		for (set<int>::iterator uIt = U.begin();
			uIt != U.end();
			uIt++) {
			for (set<int>::iterator vIt = V.begin();
				vIt != V.end();
				vIt++) {

				cnt = Clique::joinElement(cliqs->at(*uIt), cliqs->at(*vIt)).size();
				if (cnt > max) {
					max = cnt;
					edge.first = *uIt;
					edge.second = *vIt;
				}
			}
		}

		V.insert(edge.first);
		U.erase(edge.first);

		set<RandVar> randvarset = Clique::joinElement(cliqs->at(edge.first), cliqs->at(edge.second));
		vector<RandVar> randvars;
		for (set<RandVar>::iterator randIt = randvarset.begin();
			randIt != randvarset.end();
			randIt++) {

			randvars.push_back(*randIt);
		}
		Factor separator(randvars);

		separator.setProb(vector<double>(separator.getRowsSize(), 1));

		jtree.insert(separator, cnt, edge.first, edge.second);
		jtree.insert(separator, cnt, edge.second, edge.first);
	}

	delete cliqs;

}

unordered_map<string, double> InfEngine::evidCache;