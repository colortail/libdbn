#pragma once
#include "GraphMatrix.h"
#include "Factor.h"

class JTree : public GraphMatrix<vector<Factor>, Factor>
{
private:
	int root;
public:
	JTree();
	~JTree();

	int getRoot() const { return root; }
};

