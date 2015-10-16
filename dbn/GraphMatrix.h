#pragma once
#include "Graph.h"

template <typename Tv>
struct Vertex {
	Tv data;
	int inDegree, outDegree;
	VStatus status;
	int dTime, fTime;
	int parent; //�������еĸ��ڵ�
	int priority; //�������е����ȼ������ͨ·�����̿�ߣ�

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
	std::vector< Vertex <Tv> > V;	//���㼯
	std::vector< std::vector< Edge < Te > * > > E;	//�߼�

public:
	GraphMatrix();
	~GraphMatrix();

	virtual void insert(Te const &edge, int w, int i, int j);
	virtual Te remove(int i, int j);
	virtual int insert(Tv const &vertex);
	virtual Tv remove(int i);

	//ö���ڽӶ��㣬��ö�ٵ�����i���ھ�j
	virtual int nextNbr(int i, int j);
	//����i���׸��ھ� 
	virtual int firstNbr(int i);
	//����i��j֮���Ƿ���ڱ� 
	virtual bool exists(int i, int j);

	//�������
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
	//�߼�¼�Ƕ�̬����ģ����������������
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
* �߲��룬i,j ��ʾ�����±�
*/
template <typename Tv, typename Te>
void GraphMatrix<Tv, Te>::insert(Te const &edge, int w, int i, int j) {
	//�������еı�
	if (exists(i, j)) return;
	if (0 <= i && i < Graph<Tv, Te>::n && 0 <= j && j < Graph<Tv, Te>::n) {
		//�����±�
		E[i][j] = new Edge<Te>(edge, w);
		Graph<Tv, Te>::e++;
		V[i].outDegree++;
		V[j].inDegree++;
	}
}

//��ɾ��
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
����룬�±��0��ʼ
*/
template <typename Tv, typename Te>
int GraphMatrix<Tv, Te>::insert(Tv const &vertex) {
	//�߼�����һ�б�Ϊn * n + 1
	for (int j = 0; j < Graph<Tv, Te>::n; j++) {
		E[j].push_back(NULL);
	}
	++Graph<Tv, Te>::n;
	//��������1,�߼���һ�У��㼯��һ��
	E.push_back(std::vector<Edge<Te>*>(Graph<Tv, Te>::n, (Edge<Te>*)NULL));
	V.push_back(Vertex<Tv>(vertex));
	return Graph<Tv, Te>::n;
}

//��ɾ��
template <typename Tv, typename Te>
Tv GraphMatrix<Tv, Te>::remove(int i) {

	//ɾ����i��
	for (int j = 0; j < Graph<Tv, Te>::n; j++) {
		if (exists(i, j)) {
			delete E[i][j];
			V[j].inDegree--;
		}
	}
	E.erase(E.begin() + i);
	Graph<Tv, Te>::n--;

	//ɾ����i��
	for (int j = 0; j < Graph<Tv, Te>::n; j++) {
		if (exists(j, i)) {
			delete *(E[j].begin() + i);
			V[j].outDegree--;
		}
		E[j].erase(E[j].begin() + i);
	}

	//����
	Tv vBak = vertex(i);
	V.erase(V.begin() + i);
	return vBak;
}