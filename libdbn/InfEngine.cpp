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

//����ƣ�����ѱ�ǽڵ��δ���Ԫ��
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
����������������¹���Ӵ�С���
�ڵ�i���У�ѡ��δ��Žڵ���ӵ������ѱ�����ڽڵ㣨���ʱ��ѡһ��
������Ϊ n - i - 1
���нڵ����ź󣬱����С��������õ���Ԫ˳��
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
1 F <- Bnet�и��ʷֲ��ļ���
2 F�������У���֤��ȫ������Ϊ�۲�ֵ
3 while pi != \emptyset
    ZΪpi����ǰ��ı����������pi��ɾȥ
	F <-eliminate(F, Z)
4 ��F������������˵õ�һ��queryset��һ������h(q) 
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
			//����˵������а������� bnet.vertex(pi[i]).name ����������
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
1 ��factorset��ɾȥ�����漰Z�ĺ��� { f_1, ... , f_k }
2 g <- \prod_(i=1)^k{f_i}
3 h <- \sum_Z{g}
4 ��h�Ż�factorset
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
1 �� pi��ɾȥ��һ��Ԫ��Z
2 S<-nb(Z) C<-{Z} \and S
3 if (C ���� bnet�����нڵ�)
    return ��CΪΨһ�ڵ��JTree
4 else
    bnet<-Elim(bnet, Z)
    Jtree<- buildJTree(bnet, pi)
5 ��J���ҵ�һ������S���� C',��C��C'����
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
	
	int c = jtree.insert(clique);
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
			
			//separator�����clique����ʱ
			//��ȡһ��clique����,���ݲ�ͬ���������ܲ�����ͬ��junction tree
			//���˴�ȡ�������ɵ��Ǹ�clique��
			vector<string> separator;
			vector<size_t> separatorSize;
			for (set<int>::iterator w = nbrs.begin(); w != nbrs.end(); w++) {
				separator.push_back(induced.vertex(*w).name);
				separatorSize.push_back(induced.vertex(*w).range ? induced.vertex(*w).range : 2);
			}
			jtree.insert(Factor(separator, separatorSize), 0, c, cp[i]);
			jtree.insert(Factor(separator, separatorSize), 0, cp[i], c);
			break;
		}
	}
	return jtree;
}

/*
  inspired by BNT(from Prof. K.Murphy)

  Find an optimal elimination ordering (NP-hard problem!)
  triangulate
  Connect the cliques up into a jtree,
*/
JTree InfEngine::graphToJTree(BNet & moral) {
	return JTree();
}

void InfEngine::initJTreeCPD(JTree & jtree, const BNet & bnet) {
	set<Factor> factorset = bnet.getCPTs();
	for (set<Factor>::iterator setIt = factorset.begin();
		setIt != factorset.end();
		setIt++) {
		jtree.setProb(*setIt);
	}
}

void InfEngine::setEvidence(JTree & jtree, unordered_map<string, double> & evidset) {
	for (uint32_t i = 0; i < jtree.getVertexSize(); i++) {
		setEvidence(jtree.vertex(i).getPots(), evidset);
	}
}

//Shafer Shenoy Algorithm
Factor InfEngine::messagePropagation(BNet & bnet,
	JTree & jtree,
	vector<string> & queryset, 
	unordered_map<string, double>& evidset,
	vector<int> & pi) {
	
	this->initJTreeCPD(jtree, bnet);
	int root = jtree.getRoot(queryset);
	
	//collection phase
	for (int nbr = jtree.firstNbr(root); -1 < nbr; nbr = jtree.nextNbr(root, nbr)) {
		jtree.collectMessage(root, nbr);
	}

	//distribution phase
	for (int nbr = jtree.firstNbr(root); -1 < nbr; nbr = jtree.nextNbr(root, nbr)) {
		jtree.distributeMessage(root, nbr);
	}

	//posterior computaion
	Factor posterior;
	vector<int> cqs = jtree.findCoverClique(queryset);
	//����֤�ݵ�������
	for (uint32_t i = 0; i < cqs.size(); i++) {
		posterior = posterior.multiply(jtree.getCliquePotential(cqs[i]));
	}
	vector<string> querySup = libdbn::get2VectorSubstract(*posterior.getElementsName(), queryset);
	return posterior.summation(querySup).normalize();
}

//Hugin Algorithm
Factor InfEngine::messagePassing(BNet & bnet,
	vector<string> & queryset,
	unordered_map<string, double>& evidset) { 
	return Factor();
}
