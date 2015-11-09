#pragma once
#include "GraphMatrix.h"
#include "BNet.h"
#include "Factor.h"
#include "Clique.h"
#include "InOutUtils.h"

class JTree : public GraphMatrix< Clique , Factor >
{
private:
	int root;
public:
	JTree();
	~JTree();

	int getRoot() const { return root; }
	int findClique(set<int> & varset);

	friend class InOutUtils;
};

