#include "JTree.h"


JTree::JTree() : root(-1)
{
}


JTree::~JTree()
{
}

vector<int> JTree::findClique(set<int> & varset) {
	vector<int> cliques;
	for (int i = 0; i < this->n; i++) {
		if (this->vertex(i).isContain(varset))
			cliques.push_back(i);
	}
	
	return cliques;
}

void JTree::setProb(const Factor & factor) {
	vector<string>* pName = const_cast<vector<string>*>(factor.getElementsName());
	set<int> varset;
	if (pName != NULL) {
		for (int k = 0; k < pName->size(); k++) {
			varset.insert(factor.indexOf((*pName)[k]));
		}
		vector<int> belongeds = this->findClique(varset);
		
		//��ȡ��һ������ȡ���һ����
		if (belongeds.size() > 0) {
			this->vertex(belongeds.size() - 1).insert(factor);
		}
	}
}

//���ڵ���ѡ
int JTree::getRoot(vector<string> & queryset) {

	for (int i = 0; i < this->n; i++) {
		if (this->vertex(i).isContain(queryset)) {
			return i;
		}
	}
	return -1;
}

void JTree::collectMessage(int u, int v) {

}

void JTree::distributeMessage(int u, int v) {

}

void JTree::sendMessage(int u, int v) {

}

Factor JTree::retrieveMessage(int v, int u) {

}

void JTree::saveMessage(int u, int v, const Factor& pot) {

}
