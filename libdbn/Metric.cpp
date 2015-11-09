#include "Metric.h"

int MinFill::operator()(const BNet & bn, std::vector<bool> & marked) {
	BNet& tbn = const_cast<BNet&>(bn);
	std::vector<int> nbrs;
	int cnt = 0;
	int min[2] = { -1, INT_MAX };

	for (uint32_t x = 0; x < tbn.vertexSize(); x++) {
		if (marked[x])
			continue;

		cnt = 0;
		nbrs.clear();

		for (int k = tbn.firstNbr(x); -1 < k; k = tbn.nextNbr(x, k))
			if (!marked[k])
				nbrs.push_back(k);

		for (std::vector<int>::iterator iIt = nbrs.begin();
			iIt != nbrs.end();
			iIt++){
			for (std::vector<int>::iterator jIt = nbrs.begin();
				jIt != nbrs.end();
				jIt++) {
				if (*iIt != *jIt && !tbn.exists(*iIt, *jIt))
					cnt++;
			}
		}

		if (cnt < min[1]) {
			min[0] = x;
			min[1] = cnt;
		}
	}
	return min[0];
}

/*========================unfinished=================================*/
int MinNeighbors::operator()(const BNet & bn, std::vector<bool> & marked) {
	return 0;
}

int MinWeight::operator()(const BNet & bn, std::vector<bool> & marked) {
	return 0;
}

int WeightedMinFill::operator()(const BNet & bn, std::vector<bool> & marked) {
	return 0;
}

