#include "InfEngine.h"

Factor InfEngine::inference(
	BNet & bnet,
	vector<string> & queryset, 
	unordered_map<string, double> & evidset,
	INFERSTRATEGY strategy) {
	if (strategy == VE)
		return VariableElimination()(bnet, queryset, evidset, this);

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
		for (uint32_t i = 0; i < pi.size(); i++) {
			//����˵������а������� bnet.vertex(pi[i]).name ����������
			if (!libdbn::inVector(queryset, bnet.vertex(pi[i]).name)) {
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
JTree InfEngine::buildJTree(BNet & bnet, vector<int> pi) {
	return JTree();
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

InfEngine* InfEngine::m_pInstance = NULL;