#include "InfEngine.h"

vector<int> InfEngine::greedyOrdering(const BNet& bnet, Metric& m) {
	return vector<int>();
}

vector<int> InfEngine::maxCardinalitySearch(const BNet& bnet) {
	return vector<int>();
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