#include "GraphMatrix.h"

//ÁÚ½Ó¾ØÕó²âÊÔº¯Êı
void graphmatrixTest() {

	GraphMatrix<int, int> g;
	g.insert(1);
	g.insert(2);
	g.insert(3);
	g.insert(1, 1, 0, 1);
	g.insert(2, 2, 1, 2);
	g.insert(3, 3, 2, 0);
	
	g.bfs(0);
	g.dfs(0);
	//g.Graph<int, int>::bfs(0);
	//g.Graph<int, int>::dfs(0);
	
}
