#include "InfEngine.h"

vector<int> InfEngine::greedyOrdering(const BNet& bnet, Metric& m) {
	return vector<int>();
}

vector<int> InfEngine::maxCardinalitySearch(const BNet& bnet) {
	return vector<int>();
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
Factor variableElim(BNet& bnet, vector<string> queryset, unordered_map<string, double> evidset, vector<int> pi) {
	return Factor(vector<string>());
}

/*
1 ��factorset��ɾȥ�����漰Z�ĺ��� { f_1, ... , f_k }
2 g <- \sum_(i=1)^k{f_i}
3 h <- \sum_Z{g}
4 ��h�Ż�factorset
5 return factorset
*/
set<Factor>& eliminate(set<Factor>& factorset, vector<string> elimvars) {

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