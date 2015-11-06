#pragma once
#include "GraphMatrix.h"

struct RandVar {
	int node;
	std::string name;
	RandVar(int _node, std::string & _name) : node(_node), name(_name) {}
};

class DBNet
{
private:
	GraphMatrix<RandVar, double> graph;

public:
	DBNet();
	~DBNet();

	void addNode(int, std::string&);
	//void addEdge(double, int startNode, int endNode, int weight);
};

