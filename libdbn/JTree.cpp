#include "JTree.h"


JTree::JTree()
{
}


JTree::~JTree()
{
}

int JTree::findClique(set<int> & varset) {
	for (int i = 0; i < this->n; i++) {
		if (this->vertex(i).isContain(varset))
			return i;
	}
	
	return -1;
}