#pragma once
#include <limits.h> 
#include <stdio.h>

#include <vector>
#include <stack>
#include <queue>

typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus;
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } EType;

template <typename Tv, typename Te>
class Graph {
private:
	//
	void reset() {
		for (int i = 0; i < n; i++) {
			status(i) = UNDISCOVERED;
			dTime(i) = fTime(i) = -1;
			parent(i) = -1;
			priority(i) = INT_MAX;
			for (int j = 0; j < n; j++) {
				if (exists(i, j))
					type(i, j) = UNDETERMINED;
			}
		}
	}
	
	void BFS(int v, int& clock);
	void DFS(int v, int& clock);
	bool TSort(int, int&, std::stack<Tv>*);

protected:
	int n, e;

public:
	virtual void insert(Te const &, int, int, int) = 0;
	virtual Te remove(int, int) = 0;
	virtual int insert(Tv const &) = 0;
	virtual Tv remove(int) = 0;

	virtual int nextNbr(int, int) = 0;
	virtual int firstNbr(int) = 0;
	virtual bool exists(int, int) = 0;

	//�������
	//��������
	virtual Tv& vertex(int) = 0;
	//���
	virtual int inDegree(int) = 0;
	//����
	virtual int outDegree(int) = 0;
	//�������״̬
	virtual VStatus& status(int) = 0;
	//ʱ���ǩdTime fTime
	virtual int& dTime(int) = 0;
	virtual int& fTime(int) = 0;
	//�������еĸ��ڵ�
	virtual int& parent(int) = 0;
	//���ȼ���
	virtual int& priority(int) = 0;
	
	//�߲���
	//������
	virtual Te& edge(int, int) = 0;
	//������
	virtual EType& type(int, int) = 0;
	//��Ȩ��
	virtual int& weight(int, int) = 0;

	void bfs(int);	//����ͨ��������

	void dfs(int);	//����ͨ���������

	std::stack<Tv>* tSort(int); //��������
};



/*
���˹���
*/
template<typename Tv, typename Te>
void Graph<Tv, Te> ::BFS(int v, int& clock) {
	std::queue<int> q;
	status(v) = DISCOVERED;
	q.push(v);

	while (!q.empty()) {
		//
		int v = q.front();
		q.pop();
		/*do something*/

		dTime(v) = ++clock;
		for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) {
			if (UNDISCOVERED == status(u)) {	//δ������
				status(u) = DISCOVERED;
				q.push(u);
				type(v, u) = TREE;	//����
				parent(u) = v;
			}
			else {	//�ѱ����֣��ڶ����У��������ѷ��ʣ������У�
				type(v, u) = CROSS;	//���
			}
		}
		status(v) = VISITED;
	}
}

/*
����
*/
template<typename Tv, typename Te>
void Graph<Tv, Te>::bfs(int s) {
	reset();
	int clock = 0;
	int v = s;
	do {
		if (UNDISCOVERED == status(v))
			BFS(v, clock);
	} while (s != (v = (++v % n)));
}

/*
��������
*/
template<typename Tv, typename Te>
void Graph<Tv, Te>::DFS(int v, int& clock) {
	dTime(v) = ++clock;
	status(v) = DISCOVERED;

	/*do something*/

	for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) {
		switch (status(u)) {
		case UNDISCOVERED:
			type(v, u) = TREE;
			parent(u) = v;
			DFS(u, clock);
			break;
		case DISCOVERED:
			type(v, u) = BACKWARD;
			break;
		default:	//VISITED �ӳ�Ϯ��ϵ��Ϊǰ��߻��߿��
			type(v, u) = dTime(v) < dTime(u) ? FORWARD : CROSS;
			break;
		}
	}
	status(v) = VISITED;
	fTime(v) = ++clock;
}

/*
����
*/
template<typename Tv, typename Te>
void Graph<Tv, Te>::dfs(int s) {
	reset();
	int clock = 0;
	int v = s;
	do {
		if (UNDISCOVERED == status(v))
			DFS(v, clock);
	} while (s != (v = (++v) % n));
}

/*
��������
*/
template <typename Tv, typename Te>
bool Graph<Tv, Te>::TSort(int v, int& clock, std::stack<Tv>* S) {
	dTime(v) = ++clock; status(v) = DISCOVERED;
	for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u))
		switch (status(u)) {
		case UNDISCOVERED:
			parent(u) = v; type(v, u) = TREE;
			if (!TSort(u, clock, S))
				return false;
			break;
		case DISCOVERED:
			type(v, u) = BACKWARD;
			return false;
		default: //VISITED (digraphs only)
			type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
			break;
	}
	status(v) = VISITED;
	S->push(vertex(v)); //���㱻���ΪVISITEDʱ���漴��ջ
	return true; //v������������������
}

/**
����DFS�����������㷨
���ص�ջָ����Ҫ���ֶ��ͷ�
*/
template <typename Tv, typename Te>
std::stack<Tv>* Graph<Tv, Te>::tSort(int s) {
	reset(); int clock = 0; int v = s;
	std::stack<Tv>* S = new std::stack<Tv>;
	do {
		if (UNDISCOVERED == status(v))
			if (!TSort(v, clock, S)) {

				/*do something*/
				while (!S->empty()) //��һ��ͨ���༴��ͼ����DAG
					S->pop();
				break; //�򲻱ؼ������㣬��ֱ�ӷ���
			}
	} while (s != (v = (++v % n)));
	return S; //������ΪDAG����S�ڸ������Զ�������򣻷��򣨲������������򣩣�S��
}

