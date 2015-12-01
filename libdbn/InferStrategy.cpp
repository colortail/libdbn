#include "InferStrategy.h"

Factor VariableElimination::operator()(BNet & bnet,
	vector<string> & queryset,
	unordered_map<string, double> & evidset,
	InfEngine* infEngine) {

	if (infEngine == NULL)
		return Factor();

	bnet.moralize();
	vector<int> pi = infEngine->greedyOrdering(bnet, MinFill());
	return infEngine->variableElim(bnet, queryset, evidset, pi);
}


Factor JTreeInference::operator()(BNet & bnet,
	vector<string> & queryset,
	unordered_map<string, double> & evidset,
	InfEngine* infEngine) {
	
	bnet.moralize();
	JTree * pJTree = bnet.getJTree();

	if (pJTree == NULL) {

		pJTree = new JTree();
		/*方法1*/
		/*
		vector<int> pi_ = infEngine->maxCardinalitySearch(bnet);

		infEngine->buildJTree(*pJTree, bnet, pi_);
		*/

		/*方法2*/
		///*
		vector<int> pi = infEngine->greedyOrdering(bnet, MinFill());
		BNet triGraph = infEngine->triangulate(bnet, pi);
		
		infEngine->graphToJTree(*pJTree, triGraph);
		//*/

		bnet.setJTree(pJTree);
		
	}

	infEngine->setEvidence(*pJTree, bnet, evidset);

	if (infEngine->getJTreeStatus()) {
		infEngine->messagePropagation(bnet, *pJTree, queryset, evidset);

		BenchMark::debugStdOut(string("messagePropagation"));
	}

	return infEngine->getTabularFromJTree(bnet, *pJTree, queryset, evidset);
	//unfinished hugin protocal
	//return infEngine->messagePassing()
}