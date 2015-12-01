#include "LrnEngine.h"

LrnEngine::~LrnEngine()
{
}

Hypothesis* LrnEngine::initParameter(const BNet & bnet) {
	Hypothesis * hy = new Hypothesis();

	for (int i = 0; i < bnet.vertexSize(); i++) {	
		hy->insertParameter(bnet.getPaVarsRange(i), bnet.getVarRange(i));
	}
	return hy;
}

void LrnEngine::setProbability(BNet & bnet, Hypothesis * hy) {
	std::set<Factor> cpts;
	for (int i = 0; i < bnet.vertexSize(); i++) {
		vector<int> paIndex = bnet.getPaVars(i);
		vector< vector <double> > & theta = hy->getCPT(i);
		vector<RandVar> vars;
		vector<string> paName;
		vector<double> probs;
		
		for (int nodeIdx = 0; nodeIdx < paIndex.size(); nodeIdx++) {
			
			vars.push_back(bnet.vertex(paIndex[i]));
			paName.push_back(bnet.vertex(paIndex[i]).name);
		}
		vars.push_back(bnet.vertex(i));

		for (int k = 0; k < theta.size(); k++) {
			for (int w = 0; w < theta[k].size(); w++) {
				probs.push_back(theta[k][w]);
			}
		}
		cpts.insert(Factor(vars).setParents(paName).setProb(probs));
	}
	bnet.setCPTs(cpts);
	delete hy;
}

Hypothesis * LrnEngine::sampling(Hypothesis * hy, const LearningData & data) {
	return NULL;
}
