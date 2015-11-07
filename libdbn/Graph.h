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

	//顶点操作
	//顶点数据
	virtual Tv& vertex(int) = 0;
	//入度
	virtual int inDegree(int) = 0;
	//出度
	virtual int outDegree(int) = 0;
	//顶点访问状态
	virtual VStatus& status(int) = 0;
	//时间标签dTime fTime
	virtual int& dTime(int) = 0;
	virtual int& fTime(int) = 0;
	//遍历树中的父节点
	virtual int& parent(int) = 0;
	//优先级数
	virtual int& priority(int) = 0;
	
	//边操作
	//边数据
	virtual Te& edge(int, int) = 0;
	//边类型
	virtual EType& type(int, int) = 0;
	//边权重
	virtual int& weight(int, int) = 0;

	void bfs(int);	//多连通域广度优先

	void dfs(int);	//多连通域深度优先

	std::stack<Tv>* tSort(int); //拓扑排序
};



/*
单趟广搜
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
			if (UNDISCOVERED == status(u)) {	//未被发现
				status(u) = DISCOVERED;
				q.push(u);
				type(v, u) = TREE;	//树边
				parent(u) = v;
			}
			else {	//已被发现（在队列中），或者已访问（出队列）
				type(v, u) = CROSS;	//跨边
			}
		}
		status(v) = VISITED;
	}
}

/*
广搜
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
单趟深搜
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
		default:	//VISITED 视承袭关系分为前向边或者跨边
			type(v, u) = dTime(v) < dTime(u) ? FORWARD : CROSS;
			break;
		}
	}
	status(v) = VISITED;
	fTime(v) = ++clock;
}

/*
深搜
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
拓扑排序
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
	S->push(vertex(v)); //顶点被标记为VISITED时，随即入栈
	return true; //v及其后代可以拓扑排序
}

/**
基于DFS的拓扑排序算法
返回的栈指针需要被手动释放
*/
template <typename Tv, typename Te>
std::stack<Tv>* Graph<Tv, Te>::tSort(int s) {
	reset(); int clock = 0; int v = s;
	std::stack<Tv>* S = new std::stack<Tv>;
	do {
		if (UNDISCOVERED == status(v))
			if (!TSort(v, clock, S)) {

				/*do something*/
				while (!S->empty()) //任一连通域（亦即整图）非DAG
					S->pop();
				break; //则不必继续计算，故直接返回
			}
	} while (s != (v = (++v % n)));
	return S; //若输入为DAG，则S内各顶点自顶向底排序；否则（不存在拓扑排序），S空
}

