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
	//CPT ����
	static void stdPrint(const Factor & factor);

	//��ʾ��������
	static void stdPrintDBnet(BNet & bnet);

	//��ʾClique
	static void stdPrintClique(const Clique & c);

	//��ʾJunction Tree
	static void stdPrintJTree(JTree & jtree, int mode = 0);

	//��ѵ�������룬��ʼ����������
	static void readTrainSet(Hypothesis & hypothesis);
};

static const char* RANDVARTYPE[] = {
	"unobserved",
	"observed",
	"hidden"
};