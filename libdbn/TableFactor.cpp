#pragma once
#include "TableFactor.h"


TableFactor::TableFactor() {

}

TableFactor::TableFactor(int) {

}

TableFactor::~TableFactor() {

}

void TableFactor::pushItem(std::vector<double> rowItem) {
	cpt.push_back(rowItem);
}

TableFactor TableFactor::operator*(TableFactor &) {

}

TableFactor TableFactor::marginalize(std::vector<int>&) {

}

TableFactor TableFactor::operator/(TableFactor &) {

}

TableFactor& TableFactor::absorbEvidence(std::unordered_map<std::string, double>&) {

}

void tableFactorTest() {

}