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
	printf("\t%s²âÊÔ:\t\n", this->benchMark.empty() ? "»ù×¼" : this->benchMark.c_str());
	printf("ºÄÊ±£º %d clicks (%f seconds).\n", tmp, ((float)tmp) / CLOCKS_PER_SEC);
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

//Box-mulller
double gaussianDist(double mu, double sigma) {
	
	const double epsilon = std::numeric_limits<double>::min();
	const double two_pi = 2.0*3.14159265358979323846;

	static double z0, z1;
	static bool generate;
	generate = !generate;

	if (!generate)
		return z1 * sigma + mu;

	double u1, u2;
	do
	{
		u1 = rand() * (1.0 / RAND_MAX);
		u2 = rand() * (1.0 / RAND_MAX);
	} while (u1 <= epsilon);

	z0 = sqrt(-2.0 * log(u1)) * cos(two_pi * u2);
	z1 = sqrt(-2.0 * log(u1)) * sin(two_pi * u2);
	return z0 * sigma + mu;
}

}