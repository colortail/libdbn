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
