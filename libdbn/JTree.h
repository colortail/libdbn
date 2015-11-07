#pragma once
#include "GraphMatrix.h"
#include "Factor.h"

class JTree : public GraphMatrix<vector<Factor>, Factor>
{
private:
	
public:
	JTree();
	~JTree();
};

