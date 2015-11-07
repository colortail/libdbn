#pragma once
#include "GraphMatrix.h"
#include <vector>
#include <cstdint>

struct RandVar {
	int node;
	std::string name;
	RandVar(int _node, std::string & _name) : node(_node), name(_name) {}
};

enum StructType {BNET, MORAL};

//in fact,it is a DAG
class BNet : public GraphMatrix<RandVar, double>
{
public:
	BNet();
	~BNet();

	StructType getStructType() const {
		return type;
	}

	void moralize();

	friend class InOutUtils;
private:
	StructType type;
};

