#pragma once
#include <cstdio>
#include <iostream>
#include <set>
#include "InOutUtils.h"

void InOutUtils::stdPrint(const Factor & factor) {
	const vector<string>* pName = factor.getElementsName();
	const vector<vector<double>>* pTable = factor.getElementsTable();
	const vector<bool>* paMarks = factor.getParentsMark();
	printf("%s\n", factor.getFactorType() == CPD ? "条件概率表" : "势函数");
	for (uint32_t i = 0; i < pName->size(); i++) {
		printf("%s", (*pName)[i].c_str());
		if (paMarks != NULL && (*paMarks)[i]) {
				printf("(pa)");
		}
		printf("|\t");
	}
	printf("\n");
	for (vector<string>::const_iterator cit = pName->begin(); cit != pName->end(); cit++) {
		printf("--\t", cit->c_str());
	}
	printf("\n");

	for (vector<vector<double>>::const_iterator cit = pTable->begin(); cit != pTable->end(); cit++) {
		uint32_t i = 0;
		while (i < cit->size() - 1) {
			printf("%.0f|\t", (*cit)[i++]);
		}
		printf("%.4f\n\n", (*cit)[i]);
	}
}

void InOutUtils::stdPrintDBnet(BNet & bnet) {
	int u, v, e = 0;
	printf("%s：\n\n", bnet.getStructType() == BNET ? "贝叶斯网络" : "端正图(无向)");
	
	for (u = 0; u < bnet.n; u++) {
		printf("%d: %s |\t", bnet.vertex(u).node, bnet.vertex(u).name.c_str());
	}
	printf("\n");
	printf("节点(%d)\n", bnet.n);
	while (u--)
		printf("===");
	printf("\n");
	for (u = 0; u < bnet.n; u++) {
		for (v = 0; v < bnet.n - u; v++) {
			if (bnet.exists(u, v)) {
				printf("(%d)%s -> (%d)%s\n",
					u, bnet.vertex(u).name.c_str(),
					v, bnet.vertex(v).name.c_str());
				e++;
			}
		}
	}
	printf("边(%d)\n", e);
	while (u--)
		printf("===");
	printf("\n");
}

void InOutUtils::stdPrintClique(const Clique & c) {
	
	for (auto setIt = c.vars.begin();
		setIt != c.vars.end();
		setIt++) {
		printf("%d: %s (%s)|\t", setIt->node, setIt->name.c_str(), RANDVARTYPE[setIt->type]);
	}
	printf("\n");
}

void InOutUtils::stdPrintJTree(JTree & jtree, int mode) {
	
	int u, v, e = 0;
	printf("联合树：\n\n");

	for (u = 0; u < jtree.n; u++) {
		printf("节点%d:  \n", u);
		stdPrintClique(jtree.vertex(u));
	}

	printf("\n");
	printf("节点(%d)\n", jtree.n);
	while (u--)
		printf("===");
	printf("\n");

	if (mode == 0) {
		set<UndirectEdge> edges;
		for (u = 0; u < jtree.n; u++) {
			for (v = jtree.firstNbr(u); -1 < v; v = jtree.nextNbr(u, v)) {
				edges.insert(UndirectEdge(u,v));
			}
		}
		for (set<UndirectEdge>::iterator it = edges.begin();
			it != edges.end();
			it++) {
			printf("(%d) -> (%d)\n", it->i, it->j);
		}
		printf("\n");
		printf("边(%d)\n", edges.size());
		while (u--)
			printf("===");
		printf("\n");
	}
	else if (mode == 1) {
		for (u = 0; u < jtree.n; u++) {
			for (v = jtree.firstNbr(u); -1 < v; v = jtree.nextNbr(u, v)) {
				printf("(%d) -> (%d)\n", u, v);
			}
		}
	}
}
