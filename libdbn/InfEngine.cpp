#include "InfEngine.h"

vector<int> InfEngine::greedyOrdering(const BNet& moral, Metric& metric) {
	vector<int> pi(moral.vertexSize(), -1);
	vector<bool> marked(moral.vertexSize(), false);

	BNet tmpMoral;
	tmpMoral = moral;

	for (int i = 0; i < moral.vertexSize(); i++) {
		int k = metric(tmpMoral, marked);
		pi[i] = k;
		//introduce all neighbour of variable(k)
		tmpMoral.introduceEdge(k, marked);

		marked[k] = true;
	}
	return pi;
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

/*
1 F <- Bnet中概率分布的集合
2 F的因子中，将证据全部设置为观测值
3 while pi != \emptyset
    Z为pi中最前面的变量，将其从pi中删去
	F <-eliminate(F, Z)
4 将F中所有因子相乘得到一个queryset的一个函数h(q)
5 return h(q) / \sum_Q{h(q)}
*/
Factor variableElim(BNet& bnet, vector<string> queryset, unordered_map<string, double> evidset, vector<int> pi) {
	return Factor(vector<string>());
}

/*
1 从factorset中删去所有涉及Z的函数 { f_1, ... , f_k }
2 g <- \sum_(i=1)^k{f_i}
3 h <- \sum_Z{g}
4 将h放回factorset
5 return factorset
*/
set<Factor>& eliminate(set<Factor>& factorset, vector<string> elimvars) {

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

//最大势，最多已标记节点的未标记元素
int InfEngine::getMaxCardinalityElem(const BNet & moral, const vector<bool> & marked) {
	BNet& m = const_cast<BNet&>(moral);

	if (m.vertexSize() != marked.size())
		return -1;

	int cnt = 0;
	int root[2] = { -1, -1 };
	for (int i = 0; i < m.vertexSize(); i++) {
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