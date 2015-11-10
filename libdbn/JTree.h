#pragma once
#include "Factor.h"
#include "Clique.h"
#include "GraphMatrix.h"
#include "BNet.h"
#include "InOutUtils.h"

class Clique;

class JTree : public GraphMatrix<Clique, Factor>
{
private:
	int root;
public:
	JTree();
	~JTree();

	int getRoot(vector<string> & queryset);
	size_t getVertexSize() { return n; }
	size_t getEdgeSize() { return e; }

	//Ѱ�Ұ���varset�����б�ŵ���
	vector<int> findClique(set<int> & varset);
	void setProb(const Factor & factor);

	//message propagation
	void collectMessage(int u, int v);
	void distributeMessage(int u, int v);
	void sendMessage(int u, int v);
	Factor retrieveMessage(int v, int u);
	void saveMessage(int u, int v, const Factor& pot);

	friend class InOutUtils;
};

