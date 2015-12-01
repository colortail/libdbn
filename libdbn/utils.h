#pragma once
#include <stdint.h>
#include <string>
#include <ctime>
#include <map>
#include <vector>

namespace libdbn {
	
uint32_t hashCode(std::string & str);

std::vector<double> range(double end, double start = 0, double step = 1);

std::vector<int> range(int end, int start = 0, int step = 1);

/**特定概率分布的随机数**/

//均匀分布
double uniformDist(int start = 0, int end = 1);

//正太分布
double gaussianDist(double mu = 0, double sigma = 1);

/**特定的概率分布**/


class BenchMark {
private:
	std::string benchMark;
	clock_t start;
	clock_t end;
	static bool debug;

	static std::map<std::string, int> msgCnts;

public:
	BenchMark() : benchMark(""){ start = clock(); }
	BenchMark(std::string & name) : benchMark(name) { start = clock(); }

	static inline void startDebug() { debug = true; }
	static inline void stopDebug() { debug = false; }
	inline void clear() { start = clock(); }
	
	void timeTest();

	static void debugStdOut(std::string & msg, bool mem = true, const char * extra = "");
};

template< typename T, typename VECTOR >
bool inVector(const VECTOR & v, const T & elem) {
	bool b = false;
	for (uint32_t i = 0; i < v.size(); i++) {
		if (v[i] == elem) {
			b = true;
			break;
		}
	}
	return b;
}

template< typename VECTOR >
VECTOR get2VectorIntersection(const VECTOR & v1, const VECTOR & v2) {
	typename VECTOR intersection;
	for (int i = 0; i < v1.size(); i++) {
		for (int j = 0; j < v2.size(); j++) {
			if (v1[i] == v2[j])
				intersection.push_back(v1[i]);
		}
	}
	return intersection;

}

template<typename VECTOR >
VECTOR get2VectorSubstract(const VECTOR & v1, const VECTOR & v2) {
	typename VECTOR diff;
	for (uint32_t i = 0; i < v1.size(); i++) {
		int exists = false;
		for (uint32_t j = 0; j < v2.size(); j++) {
			if (v1[i] == v2[j]) {
				exists = true;
				break;
			}
		}
		if (exists == false)
			diff.push_back(v1[i]);
	}
	return diff;
}

template<typename VECTOR, typename MAP>
VECTOR getVectorMAPUnion(const VECTOR & v1, const MAP & v2) {
	typename VECTOR addition;
	addition = v1;
	for (auto it = v2.begin();
		it != v2.end();
		it++) {
		addition.push_back(it->first);
	}
	return addition;
}

template<typename VECTOR, typename TWODIMVECTOR>
VECTOR getVectorIntersection(const TWODIMVECTOR & v) {
	typename VECTOR intersection;
	if (v.size() >= 1) {
		intersection = v[0];
		for (int i = 1; i < v.size(); i++) {
			intersection = get2VectorIntersection(intersection, v[1]);
		}
	}
	return result;
}

template<typename VECTOR, typename SET>
VECTOR & removeElements(VECTOR & v, const SET & s) {
	typename VECTOR::iterator iter;
	for (iter = v.begin(); iter != v.end(); ) {
		if (s.find(*iter) != s.end()) {
			iter = v.erase(iter);
		}
		else
			iter++;
	}
	return v;
}

template<typename SET>
SET setIntersection(SET & s1, SET & s2) {
	typename SET::iterator iter;
	SET intersection;
	for (iter = s1.begin(); iter != s1.end(); iter++) {
		if (s2.find(*iter) != s2.end())
			intersection.insert(*iter);
	}
	return intersection;
}

template<typename SET>
SET setSub(SET & s1, SET & s2) {
	typename SET::iterator iter;
	SET sub;
	for (iter = s1.begin(); iter != s1.end(); iter++) {
		if (s2.find(*iter) == s2.end())
			sub.insert(*iter);
	}
	return sub;
}

}