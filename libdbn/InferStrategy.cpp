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
	
	vector<int> pi = infEngine->maxCardinalitySearch(bnet);
	//vector<int> pi = infEngine->greedyOrdering(bnet);
	
	JTree jtree;

	jtree = infEngine->buildJTree(jtree, bnet, pi);

	return infEngine->messagePropagation(bnet, jtree, queryset, evidset, pi);
	
	//unfinished hugin protocal
	//return infEngine->messagePassing()
}