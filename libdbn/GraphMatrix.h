#pragma once
#include "Graph.h"

template <typename Tv>
struct Vertex {
	Tv data;
	int inDegree, outDegree;
	VStatus status;
	int dTime, fTime;
	int parent; //遍历树中的父节点
	int priority; //遍历树中的优先级（最短通路，极短跨边）

	Vertex(Tv const &d) :
		data(d), inDegree(0), outDegree(0), status(UNDISCOVERED),
		dTime(-1), fTime(-1), parent(-1), priority(INT_MAX){}
};

template <typename Te>
struct Edge {
	Te data;
	int weight;
	EType type;

	Edge(Te const &d, int w) :
		data(d), weight(w), type(UNDETERMINED) {}
};


template <typename Tv, typename Te>
class GraphMatrix : public Graph <Tv, Te> {
private:
	std::vector< Vertex <Tv> > V;	//顶点集
	std::vector< std::vector< Edge < Te > * > > E;	//边集

public:
	GraphMatrix();
	~GraphMatrix();

	virtual void insert(Te const &edge, int w, int i, int j);
	virtual Te remove(int i, int j);
	virtual int insert(Tv const &vertex);
	virtual Tv remove(int i);

	//枚举邻接顶点，已枚举到顶点i的邻居j
	virtual int nextNbr(int i, int j);
	//顶点i的首个邻居 
	virtual int firstNbr(int i);
	//顶点i和j之间是否存在边 
	virtual bool exists(int i, int j);

	//顶点操作
	virtual Tv& vertex(int i);
	virtual int inDegree(int i);
	virtual int outDegree(int i);
	virtual VStatus& status(int i);
	virtual int& dTime(int i);
	virtual int& fTime(int i);
	virtual int& parent(int i);
	virtual int& priority(int i);
	virtual Te& edge(int i, int j);
	virtual EType& type(int i, int j);
	virtual int& weight(int i, int j);
	
};


template <typename Tv, typename Te>
GraphMatrix<Tv, Te>::GraphMatrix() {
	Graph<Tv, Te>::n = Graph<Tv, Te>::e = 0;
}

template <typename Tv, typename Te>
GraphMatrix<Tv, Te>::~GraphMatrix()
{
	//边记录是动态申请的，在析构函数中清除
	for (int j = 0; j < Graph<Tv, Te>::n; j++)
		for (int k = 0; k < Graph<Tv, Te>::n; k++)
			delete E[j][k];
}

template <typename Tv, typename Te>
Tv& GraphMatrix<Tv, Te>::vertex(int i) {
	return V[i].data;
}

template <typename Tv, typename Te>
int GraphMatrix<Tv, Te>::inDegree(int i) {
	return V[i].inDegree;
}

template <typename Tv, typename Te>
int GraphMatrix<Tv, Te>::outDegree(int i) {
	return V[i].outDegree;
}

template <typename Tv, typename Te>
VStatus& GraphMatrix<Tv, Te>::status(int i) {
	return V[i].status;
}

template <typename Tv, typename Te>
int& GraphMatrix<Tv, Te>::dTime(int i) {
	return V[i].dTime;
}

template <typename Tv, typename Te>
int& GraphMatrix<Tv, Te>::fTime(int i) {
	return V[i].fTime;
}

template <typename Tv, typename Te>
int& GraphMatrix<Tv, Te>::parent(int i) {
	return V[i].parent;
}

template <typename Tv, typename Te>
int& GraphMatrix<Tv, Te>::priority(int i) {
	return V[i].priority;
}

template <typename Tv, typename Te>
Te& GraphMatrix<Tv, Te>::edge(int i, int j) {
	return E[i][j]->data;
}

template <typename Tv, typename Te>
EType& GraphMatrix<Tv, Te>::type(int i, int j) {
	return E[i][j]->type;
}

template <typename Tv, typename Te>
int& GraphMatrix<Tv, Te>::weight(int i, int j) {
	return E[i][j]->weight;
}


template <typename Tv, typename Te>
int GraphMatrix<Tv, Te>::nextNbr(int i, int j) {
	while ((-1 < j) && !exists(i, --j));
	return j;
}

template <typename Tv, typename Te>
int GraphMatrix<Tv, Te>::firstNbr(int i) {
	return nextNbr(i, Graph<Tv, Te>::n);
}

template <typename Tv, typename Te>
bool GraphMatrix<Tv, Te>::exists(int i, int j) {
	return (0 <= i) && (i < Graph<Tv, Te>::n) && (0 <= j) && (j < Graph<Tv, Te>::n) && E[i][j] != NULL;
}

/**
* 边插入，i,j 表示顶点下标
*/
template <typename Tv, typename Te>
void GraphMatrix<Tv, Te>::insert(Te const &edge, int w, int i, int j) {
	//忽略已有的边
	if (exists(i, j)) return;
	if (0 <= i && i < Graph<Tv, Te>::n && 0 <= j && j < Graph<Tv, Te>::n) {
		//创建新边
		E[i][j] = new Edge<Te>(edge, w);
		Graph<Tv, Te>::e++;
		V[i].outDegree++;
		V[j].inDegree++;
	}
}

//边删除
template <typename Tv, typename Te>
Te GraphMatrix<Tv, Te>::remove(int i, int j) {
	Te eBak = edge(i, j);
	delete E[i][j];
	E[i][j] = NULL;
	Graph<Tv, Te>::e--;
	V[i].outDegree--;
	V[j].inDegree--;
	return eBak;
}

/**
点插入，下标从0开始
*/
template <typename Tv, typename Te>
int GraphMatrix<Tv, Te>::insert(Tv const &vertex) {
	//边集增加一列变为n * n + 1
	for (int j = 0; j < Graph<Tv, Te>::n; j++) {
		E[j].push_back(NULL);
	}
	++Graph<Tv, Te>::n;
	//矩阵扩大1,边集加一行，点集加一项
	E.push_back(std::vector<Edge<Te>*>(Graph<Tv, Te>::n, (Edge<Te>*)NULL));
	V.push_back(Vertex<Tv>(vertex));
	return Graph<Tv, Te>::n;
}

//点删除
template <typename Tv, typename Te>
Tv GraphMatrix<Tv, Te>::remove(int i) {

	//删除第i行
	for (int j = 0; j < Graph<Tv, Te>::n; j++) {
		if (exists(i, j)) {
			delete E[i][j];
			V[j].inDegree--;
		}
	}
	E.erase(E.begin() + i);
	Graph<Tv, Te>::n--;

	//删除第i列
	for (int j = 0; j < Graph<Tv, Te>::n; j++) {
		if (exists(j, i)) {
			delete *(E[j].begin() + i);
			V[j].outDegree--;
		}
		E[j].erase(E[j].begin() + i);
	}

	//备份
	Tv vBak = vertex(i);
	V.erase(V.begin() + i);
	return vBak;
}