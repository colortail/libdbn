#include "Factor.h"

//Factor::Factor() : varSize(0), factorType(CPD){}

Factor::~Factor() {
	if (pName != NULL) {
		delete pName;
		pName = NULL;
	}
	if (pLocation != NULL) {
		delete pLocation;
		pLocation = NULL;
	}
	if (pTable != NULL) {
		delete pTable;
		pTable = NULL;
	}
	if (pParents != NULL) {
		delete pParents;
		pParents = NULL;
	}
	if (pElemSize != NULL) {
		delete pElemSize;
		pElemSize = NULL;
	}
}

Factor::Factor(const Factor& copy)
	: factorType(copy.factorType),
	varSize(copy.varSize),
	rows(copy.rows),
	hash(copy.hash) {

	this->pName = new vector<string>(*copy.pName);
	this->pLocation = new unordered_map<string, int>(*copy.pLocation);

	this->pElemSize = new vector<size_t>(*copy.pElemSize);
	this->pTable = new vector<vector<double>>(*copy.pTable);
	if (copy.pParents != NULL)
		this->pParents = new vector<bool>(*copy.pParents);
	else
		this->pParents = NULL;
}

Factor& Factor::operator=(const Factor& assign) {

	if (this != &assign) {
		if (pName != NULL) {
			delete pName;
			pName = NULL;
		}
		if (pLocation != NULL) {
			delete pLocation;
			pLocation = NULL;
		}
		if (pTable != NULL) {
			delete pTable;
			pTable = NULL;
		}
		if (pParents != NULL) {
			delete pParents;
			pParents = NULL;
		}
		if (pElemSize != NULL) {
			delete pElemSize;
			pElemSize = NULL;
		}

		this->factorType = assign.factorType;
		this->varSize = assign.varSize;
		this->rows = assign.rows;
		this->hash = assign.hash;

		this->pName = new vector<string>(*assign.pName);
		this->pLocation = new unordered_map<string, int>(*assign.pLocation);

		this->pElemSize = new vector<size_t>(*assign.pElemSize);
		this->pTable = new vector<vector<double>>(*assign.pTable);
		if (assign.pParents != NULL)
			this->pParents = new vector<bool>(*assign.pParents);
		else
			this->pParents = NULL;
	}
	return *this;
}

//默认变量为2值
Factor::Factor()
	: factorType(POT), rows(0), varSize(0), pParents(NULL), hash(0) {
	newFactor(vector<string>(), vector<size_t>(0, 2));
}

Factor::Factor(const vector<string> & elems)
	: factorType(POT), rows(1), varSize(elems.size()), pParents(NULL), hash(0) {

	int varSize = elems.size();
	newFactor(elems, vector<size_t>(varSize, 2));
}

Factor::Factor(const vector<string> & elems, const vector<size_t> & elemSize)
	: factorType(POT), rows(1), varSize(elems.size()), pParents(NULL),hash(0) {
	newFactor(elems, elemSize);
}
Factor::Factor(const vector<RandVar> & randVars)
	: factorType(POT), rows(1), varSize(randVars.size()), pParents(NULL), hash(0){
	
	vector<string> elems;
	vector<size_t> elemSize;
	for (vector<RandVar>::const_iterator varIt = randVars.begin();
		varIt != randVars.end();
		varIt++) {

		elems.push_back(varIt->name);
		elemSize.push_back(varIt->range ? varIt->range : 2);
	}

	newFactor(elems, elemSize);
}
void Factor::newFactor(const vector<string> & elems, const vector<size_t> & elemSize) {

	if (elems.size() != elemSize.size()) {
		throw exception("exception on constructing the factor, arguments can not be matched!");
	}

	this->pLocation = new unordered_map<string, int>();
	this->pTable = new vector<vector<double>>();
	
	this->pName = new vector<string>(elems);
	this->pElemSize = new vector<size_t>(elemSize);

	for (uint32_t i = 0; i < varSize; i++) {
		this->pLocation->insert({ (*pName)[i], i });
		this->rows *= elemSize[i];
	}
	for (uint32_t j = 0; j < rows; j++) {
		this->pTable->push_back(vector<double>(varSize + 1, 0));
	}
	//填充表格
	int offset = 1;
	for (int i = varSize - 1; i >= 0; i--) {
		for (uint32_t j = 0; j < rows; j++) {
			(*pTable)[j][i] = (j / offset) % (*pElemSize)[i];
		}
		offset *= elemSize[i];
	}
	hashCode();
}

Factor& Factor::setParents(const vector<string> & parents) {
	this->factorType = CPD;
	pParents = new vector<bool>(varSize, false);
	assert(pLocation != NULL);

	for (uint32_t i = 0; i < parents.size(); i++) {
		unordered_map<string, int>::iterator loc = pLocation->find(parents[i]);
		if (loc != pLocation->end()) {
			(*pParents)[loc->second] = true;
		}
	}
	return *this;
}

uint32_t Factor::indexOf(string & name) const {
	
	unordered_map<string, int>::iterator it = pLocation->find(name);
	if (it != pLocation->end())
		return it->second;
	else
		return -1;
}

size_t Factor::getElemSize(string & name) {
	uint32_t idx = this->indexOf(name);
	if (0 <= idx && idx < pElemSize->size())
		return (*pElemSize)[idx];
	else
		return -1;
}

vector<size_t> Factor::getElemSize(vector<string> & names) {
	vector<size_t> sizes;
	size_t idx = -1;
	for (uint32_t i = 0; i < names.size(); i++) {
		idx = this->indexOf(names[i]);
		if (0 <= idx && idx < pElemSize->size())
			sizes.push_back((*pElemSize)[idx]);
		else
			sizes.push_back(-1);
	}
	return sizes;
}

Factor& Factor::setProb(const vector<double> & probVector) {
	int size = probVector.size();
	for (uint32_t i = 0; i < this->pTable->size(); i++) {
		(*this->pTable)[i][this->varSize] = probVector[i % size];
	}
	return *this;
}

unordered_map<string, pair<int, int>> Factor::getIntersection(const Factor* a, const Factor* b) {
	if (a == NULL || b == NULL)
		return unordered_map<string, pair<int, int>>();
	
	unordered_map<string, pair<int, int>> unionSet;
	
	if (a == b) {
		const vector<string>* pNames = a->getElementsName();
		for (uint32_t i = 0; i < pNames->size(); i++) {
			unionSet.insert({ (*pNames)[i], pair<int, int>(i, i) });
		}
	}
	else {
		const vector<string>* pNameLeft = a->getElementsName();
		const vector<string>* pNameRight = b->getElementsName();
		for (uint32_t i = 0; i < pNameLeft->size(); i++)
			for (uint32_t j = 0; j < pNameRight->size(); j++) {
				if ((*pNameLeft)[i] == (*pNameRight)[j]) {
					unionSet.insert({ (*pNameLeft)[i], pair<int, int>(i, j) });
				}
			}
	}
	return unionSet;
}

//乘
Factor Factor::multiply(const Factor & b) const {

	if (this->varSize == 0 && b.varSize == 0)
		return Factor(vector<string>());
	if (this->varSize == 0)
		return b;
	if (b.varSize == 0)
		return *this;

	vector<size_t> dimLeft(this->getElementsName()->size(),2);
	vector<size_t> dimRight(b.getElementsName()->size(), 2);

	vector<string> elemNames;
	vector<size_t> elemSize;
	
	size_t cnt;
	size_t size;
	int leftIdx, rightIdx;

	unordered_map<string, pair<int, int>> unionSet = getIntersection(this, &b);

	//get union variable
	for (uint32_t i = 0; i < this->pName->size(); i++) {
		elemNames.push_back((*this->pName)[i]);
		elemSize.push_back((*this->pElemSize)[i]);
	}
	for (uint32_t i = 0; i < b.pName->size(); i++) {
		if (unionSet.find((*b.pName)[i]) == unionSet.end()) {
			elemNames.push_back((*b.pName)[i]);
			elemSize.push_back((*b.pElemSize)[i]);
		}
	}
	//get two dimension mark
	cnt = 1;
	size = this->getElementsName()->size();
	for (int i = size - 1; i >= 0; i--) {
		dimLeft[i] = cnt;
		cnt *= (*this->pElemSize)[i];
	}
	cnt = 1; 
	size = b.getElementsName()->size();
	for (int i = size - 1; i >= 0; i--) {
		dimRight[i] = cnt;
		cnt *= (*b.pElemSize)[i];
	}

	//core multiply
	Factor result(elemNames, elemSize);
	for (uint32_t k = 0; k < result.rows; k++) {
		leftIdx = rightIdx = 0;
		unordered_map<string, int>::iterator pLoc;
		for (uint32_t w = 0; w < elemNames.size(); w++) {
			pLoc = this->pLocation->find(elemNames[w]);
			if (pLoc != this->pLocation->end()) {
				leftIdx += (int)(*result.pTable)[k][w] * dimLeft[(pLoc->second)];
			}
		}
		for (uint32_t w = 0; w < elemNames.size(); w++) {
			pLoc = b.pLocation->find(elemNames[w]);
			if (pLoc != b.pLocation->end()) {
				rightIdx += (int)(*result.pTable)[k][w] * dimRight[(pLoc->second)];
			}
		}
		(*result.pTable)[k][result.varSize] = (*this->pTable)[leftIdx][this->varSize] * (*b.pTable)[rightIdx][b.varSize];
	}

	//for CPT
	if (this->getFactorType() == CPD && b.getFactorType() == CPD) {
		set<string> childrenBag;
		set<string> parentBag;
		set<string>::iterator it;
		for (uint32_t i = 0; i < this->varSize; i++) {
			if ((*this->pParents)[i]) {
				parentBag.insert((*this->pName)[i]);
			}
			else
				childrenBag.insert((*this->pName)[i]);
		}
		for (uint32_t i = 0; i < b.varSize; i++) {
			if ((*b.pParents)[i]) {
				parentBag.insert((*b.pName)[i]);
			}
			else
				childrenBag.insert((*b.pName)[i]);
		}

		it = parentBag.begin();
		while (it != parentBag.end()) {
			if (childrenBag.find(*it) != childrenBag.end()) {
				it = parentBag.erase(it);
			}
			else 
				it++;
		}
		//It is a CPT
		if (parentBag.empty()) {
			result.setParents(vector<string>());
		}
	}
	return result;
}

//边缘化
Factor Factor::summation(vector<string> & varset) const {
	vector<string> elemNames(*this->pName);
	vector<size_t> elemSize;
	vector<string>::iterator it;
	unordered_map<string, int>::iterator pLoc;
	int idx = 0;
	for (uint32_t i = 0; i < varset.size(); i++) {
		it = elemNames.begin();
		while (it != elemNames.end()) {
			if (varset[i] == *it) {
				it = elemNames.erase(it);
			}
			else
				it++;
		}
	}

	for (it = elemNames.begin(); it != elemNames.end(); it++) {
		pLoc = this->pLocation->find(*it);
		if (pLoc != this->pLocation->end())
			elemSize.push_back((*this->pElemSize)[pLoc->second]);
		else
			elemSize.push_back(0); //pass it, it can not happen
	}

	Factor result(elemNames, elemSize);

	if (elemNames.size() == 0)
		return result;
	
	//radix
	for (int i = elemSize.size() - 2; i >= 0; i--) {
		elemSize[i] *= elemSize[i + 1];
	}
	for (int i = elemSize.size() - 2; i >= 0; i--) {
		elemSize[i] = elemSize[i + 1];
	}
	elemSize[elemSize.size() - 1] = 1;

	//core marginalization
	for (uint32_t i = 0; i < this->rows; i++) {
		idx = 0;
		for (uint32_t j = 0; j < this->pName->size(); j++) {
			pLoc = result.pLocation->find((*this->pName)[j]);
			if (pLoc != result.pLocation->end()) {
				idx += (int)(*this->pTable)[i][j] * elemSize[pLoc->second];
			} 
		}
		(*result.pTable)[idx][result.varSize] += (*this->pTable)[i][this->varSize];
	}
	return result;
}

//设置证据
Factor Factor::setEvidence(unordered_map<string, double>& evidset) const {
	Factor result = *this;
	int probLoc = result.varSize;
	for (unordered_map<string, double>::iterator evidIt = evidset.begin();
		evidIt != evidset.end();
		evidIt++) {
		
		unordered_map<string, int>::iterator locIt = result.pLocation->find(evidIt->first);
		if (locIt != result.pLocation->end()) {

			for (vector<vector<double>>::iterator cptIt = result.pTable->begin();
				cptIt != result.pTable->end(); cptIt++) {
				if (abs((*cptIt)[locIt->second] - evidIt->second) > 0.0001) {
					//cptIt = result.pTable->erase(cptIt);
					(*cptIt)[probLoc] = 0;
				}
			}
		}
	}
	result.normalize();
	return result;
}

uint32_t Factor::hashCode() {
	
	if (hash == 0) {
		uint32_t hashCode = 0;
		vector<string>* pNames = this->getElementsName();
		for (uint32_t i = 0; i < this->getVarSize(); i++)
			hashCode += libdbn::hashCode((*pNames)[i]);
		this->hash = hashCode;
	}
	return this->hash;
}

//set operator:参数为对象，不可为reference
bool operator<(const Factor & lhs, const Factor & rhs) {

	vector<string>* leftNames = lhs.getElementsName();
	vector<string>* rightNames = rhs.getElementsName();
	if (lhs.getVarSize() != rhs.getVarSize())
		return lhs.getVarSize() < rhs.getVarSize();
	
	return lhs.hash < rhs.hash;

}

bool Factor::exists(const string& var) const {

	if (pLocation != NULL) {
		return pLocation->find(var) != pLocation->end();
	}
	return false;
}

vector<string>& Factor::exists(vector<string>& commonvars, const vector<string>& varset) const {
	
	commonvars.clear();

	if (pLocation != NULL) {
		for (uint32_t i = 0; i < varset.size(); i++) {
			if (pLocation->find(varset[i]) != pLocation->end()) {
				commonvars.push_back(varset[i]);
			}
		}
	}
	return commonvars;
}

Factor& Factor::normalize() {
	double norm = 0;
	for (uint32_t i = 0; i < rows; i++) {
		norm += (*this->pTable)[i][varSize];
	}
	for (uint32_t i = 0; i < rows; i++) {
		(*this->pTable)[i][varSize] /= norm;
	}
	return *this;
}

vector<double> Factor::getProbVector() const {
	
	if (this->pTable == NULL)
		return vector<double>();

	vector<double> prob;
	for (uint32_t i = 0; i < this->getRowsSize(); i++) {
		prob.push_back((*this->pTable)[i][this->varSize]);
	}
	return prob;
}