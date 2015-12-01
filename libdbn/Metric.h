#pragma once
#include <string>
#include <vector>
#include "BNet.h"

class Metric {
public:
	virtual std::string getType() = 0;
	//消去顺序标准
	virtual int operator()(const BNet & bn, std::vector<bool>&) = 0;
};

//Min-neighbors: 
//The cost of a vertex is the number of neighbors it has in the current graph.
class MinNeighbors : public Metric {
	std::string getType() { return "MinNeighbors"; }
	int operator()(const BNet & bn, std::vector<bool>&);
};

//Min-weight:
//The cost of a vertex is the product of weights — domain cardinality — of its neighbors.
class MinWeight : public Metric {
	std::string getType() { return "MinWeight"; }
	int operator()(const BNet & bn, std::vector<bool>&);
};

//Min-fill: - 
//The cost of a vertex is the number of edges that need to be added to the graph
//due to its elimination.
class MinFill : public Metric {
	std::string getType() { return "MinFill(最小缺边)"; }
	int operator()(const BNet & bn, std::vector<bool>&);
};

//Weighted-min-fill: 
//The cost of a vertex is the sum of weights of the edges that need to
//be added to the graph due to its elimination, where a weight of an edge is the product of
//weights of its constituent vertices.
class WeightedMinFill : public Metric {
	std::string getType() { return "WeightedMinFill"; }
	int operator()(const BNet & bn, std::vector<bool>&);
};


//Likelihood Function
class LL {
public:

};