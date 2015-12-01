#pragma once
#include "Factor.h"
#include "Clique.h"
#include "GraphMatrix.h"
#include "BNet.h"
#include "InOutUtils.h"
#include "utils.h"

class Clique;

class JTree : public GraphMatrix<Clique, Factor>
{
private:
	int root;
	unordered_map<string, int> location;
	//�򵥵����ѣ����ڲ��Ұ���������varset������
	//<index, count>
	void maxContain(const vector<string> & varset, 
		std::pair<int, int> & result, 
		int c, 
		vector<bool>& marked);

public:
	JTree();
	~JTree();
	JTree(const JTree &);
	JTree& operator=(const JTree & copy);

	int getRoot(vector<string> & queryset);
	size_t getVertexSize() { return n; }
	size_t getEdgeSize() { return e; }

	int indexOf(string & name);

	int insertClique(Clique const &vertex);
	void insert(Factor const &edge, int w, int i, int j);

	//Ѱ�Ұ���varset�����б��(�����±�)����
	vector<int> findClique(set<int> & varset);
	//�ҵ����Ǳ��������ż�(��Ҫ�ĳ�����)
	vector<int> findCoverClique(vector<string>);
	//transform set, so reference again
	vector<pair<int, int> > findCoverSeparator(vector<string> &);
	
	void setProb(const Factor & factor);

	Factor getCliquePotential(int c);

	void clearTabular();

	//message propagation algorithm

	//clique(u) <- clique(v)
	void collectMessage(int u, int v);
	//clique(u) -> clique(v)
	void distributeMessage(int u, int v);
	//send message (from u to v)
	void sendMessage(int u, int v);

	Factor retrieveMessage(int v, int u);
	void saveMessage(int u, int v, const Factor& pot);

	friend class InOutUtils;
};

