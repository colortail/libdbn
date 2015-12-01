#include "utils.h"

namespace libdbn {

uint32_t hashCode(std::string & str) {
	uint32_t seed = 31;
	uint32_t hash = 0;
	for (std::string::iterator p = str.begin();
		p != str.end();
		p++)
		hash = hash * seed + (*p);
	return (hash & 0x7fffffff);
}

void BenchMark::timeTest() {
	this->end = clock();
	clock_t tmp = end - start;
	printf("==================================\n");
	printf("\t%s����:\t\n", this->benchMark.empty() ? "��׼" : this->benchMark.c_str());
	printf("��ʱ�� %d clicks (%f seconds).\n", tmp, ((float)tmp) / CLOCKS_PER_SEC);
	printf("==================================\n");
}

void BenchMark::debugStdOut(std::string & msg, bool mem, const char* extra) {

	if (debug) {

		if (mem) {
			std::map<std::string, int>::iterator idx = msgCnts.find(msg);
			if (idx != msgCnts.end()) {
				idx->second++;
				printf("%s: run %d round(s) %s\n", msg.c_str(), idx->second, extra);
			}
			else {
				msgCnts.insert({ msg, 1 });
				printf("%s: run 1 round(s) %s\n", msg.c_str(), extra);
			}
		}
		else {
			printf("%s %s\n", msg.c_str(), extra);
		}

	}
}


bool BenchMark::debug = false;

std::map<std::string, int> BenchMark::msgCnts;

std::vector<double> range(double end, double start, double step) {
	if (end <= start)
		return std::vector<double>();

	size_t size = (int)((end - start) / step);
	if (size == 0) size = 1;

	std::vector<double> rang(size);
	for (int i = 0; abs(end - start) < 0.0001; i++) {
		rang[i] = start;
		start += step;
	}
	return rang;
}

std::vector<int> range(int end, int start, int step) {
	if (end <= start)
		return std::vector<int>();

	size_t size = (end - start) / step;
	if (size == 0) size = 1;

	std::vector<int> rang(size);
	for (int i = 0; start < end; i++) {
		rang[i] = start;
		start += step;
	}
	return rang;
}

double uniformDist(int start, int end) {
	return rand();
}

double gaussianDist(double mu, double sigma) {
	return rand();
}

}