#pragma once
#include <stdint.h>
#include <string>
#include <ctime>
#include <vector>

namespace libdbn {
	
uint32_t hashCode(std::string & str);

class BenchMark {
private:
	std::string benchMark;
	clock_t start;
	clock_t end;
public:
	BenchMark() : benchMark("") { start = clock(); }
	BenchMark(std::string & name) : benchMark(name) { start = clock(); }

	inline void clear() { start = clock(); }
	void timeTest();
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


}