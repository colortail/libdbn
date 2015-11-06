#include "DBNet.h"


DBNet::DBNet()
{
}


DBNet::~DBNet()
{
}

void DBNet::addNode(int node, std::string & name) {
	
	graph.insert(RandVar(node, name));
}

