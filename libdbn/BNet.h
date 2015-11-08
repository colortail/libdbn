#pragma once
#include "GraphMatrix.h"
#include <vector>
#include <cstdint>

struct RandVar {
	int node; // id
	bool observed; // 是否观察到，default 0，1 iff observed
	std::string name; //节点名
	RandVar(int _node, std::string & _name) : node(_node), name(_name), observed(0) {}
};

enum StructType {BNET, MORAL};

//in fact,it is a DAG
class BNet : public GraphMatrix<RandVar, double>
{
public:
	BNet();
	~BNet();
	BNet& operator=(const BNet& bn);

	StructType getStructType() const { return type; }

	int vertexSize() const { return this->n; }

	int edgeSize() const { return this->e; }

	//moralize
	void moralize();

	//triangulate
	void triangulate();

	void introduceEdge(int, std::vector<bool> &);

	friend class InOutUtils;
private:
	StructType type;
};

