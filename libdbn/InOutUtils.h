#pragma once
#include "Factor.h"
#include "BNet.h"
#include "Clique.h"
#include "JTree.h"
#include "Hypothesis.h"

struct UndirectEdge;

class Clique;
class JTree;
class BNet;
class Factor;
class Hypothesis;

class InOutUtils {
public:
	//static void scanTableFactor(TableFactor&);
	//CPT 概率
	static void stdPrint(const Factor & factor);

	//显示网络拓扑
	static void stdPrintDBnet(BNet & bnet);

	//显示Clique
	static void stdPrintClique(const Clique & c);

	//显示Junction Tree
	static void stdPrintJTree(JTree & jtree, int mode = 0);

	//将训练集读入，初始化参数假设
	static void readTrainSet(Hypothesis & hypothesis);
};

static const char* RANDVARTYPE[] = {
	"unobserved",
	"observed",
	"hidden"
};