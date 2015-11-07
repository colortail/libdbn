#pragma once
//#pragma comment(lib,"pthreadVCE2.lib")
//#pragma comment(lib,"libdbn.lib")
#include "GraphMatrix.h"
#include "Factor.h"
#include "BNet.h"
#include "InOutUtils.h"

#include <iostream>
#include <vector>
#include <set>
#include <string>
//#include <stdint.h>
//#include <pthread.h>
//#include <math.h>
//#include "libdbn.h"

//¡⁄Ω”æÿ’Û≤‚ ‘∫Ø ˝
void graphmatrixTest() {

	GraphMatrix<int, int> g;
	g.insert(1);
	g.insert(2);
	g.insert(3);
	g.insert(1, 1, 0, 1);
	g.insert(2, 2, 1, 2);
	g.insert(3, 3, 2, 0);

	g.bfs(0);
	g.dfs(0);
	//g.Graph<int, int>::bfs(0);
	//g.Graph<int, int>::dfs(0);

}

//CPT≤‚ ‘
void factorTest() {

	vector<std::string> vv;
	vv.push_back("A");
	vv.push_back("B");
	vv.push_back("C");
	vv.push_back("D");

	vector<std::string> v1;
	v1.push_back("B");
	v1.push_back("D");

	Factor fact1(vv);
	Factor fact2(v1);

	//set<Factor> sfs;
	//sfs.insert(fact1);
	//sfs.insert(fact2);
	//printf("fact1 < fact1 ? %d\n",fact1 < fact1);
	//printf("fact1 < fact2 ? %d\n", fact1 < fact2);
	//printf("fact2 < fact1 ? %d\n", fact2 < fact1);

	vector<double> vp1;
	vp1.push_back(0.1);
	vp1.push_back(0.2);
	vp1.push_back(0.3);
	vp1.push_back(0.4);

	fact1.setProb(vp1);
	vector<string> pa;
	pa.push_back("A");
	pa.push_back("D");
	fact1.setParents(pa);

	//fact2.setProb(vector<double>(4, 0.5));
	unordered_map<string, double> evidset;
	evidset.insert({ "A", 0 });
	InOutUtils::stdPrint(fact1);
	InOutUtils::stdPrint(fact1.summation(pa));
	//InOutUtils::stdPrint(fact1.setEvidence(evidset));
}

void bnetTest() {
	BNet bn;
	bn.insert(RandVar(0, std::string("A")));
	bn.insert(RandVar(1, std::string("S")));
	bn.insert(RandVar(2, std::string("T")));
	bn.insert(RandVar(3, std::string("R")));
	bn.insert(RandVar(4, std::string("L")));
	bn.insert(RandVar(5, std::string("X")));
	bn.insert(RandVar(6, std::string("D")));
	bn.insert(RandVar(7, std::string("B")));

	bn.insert(0, 0, 0, 2);
	bn.insert(0, 0, 2, 3);
	bn.insert(0, 0, 3, 5);
	bn.insert(0, 0, 3, 6);
	bn.insert(0, 0, 1, 4);
	bn.insert(0, 0, 1, 7);
	bn.insert(0, 0, 4, 3);
	bn.insert(0, 0, 7, 6);

	bn.moralize();

	InOutUtils::stdPrintDBnet(bn);
}

int main() {
	//graphmatrixTest();
	factorTest();
	//bnetTest();
	return 0;
}


//struct data {
//	pthread_t t;
//	uint64_t num;
//	uint64_t min_n;
//	uint64_t max_n;
//};
//
//void * run(void * p) {
//	struct data * d = (struct data *)p;
//	uint64_t sum;
//	if (d->min_n % 2 == 0)
//		sum = (d->min_n / 2) * (d->min_n - 1);
//	else
//		sum = d->min_n * ((d->min_n - 1) / 2);
//
//	for (uint64_t n = d->min_n; n < d->max_n; n++) {
//		if ((d->num - sum) % n == 0) {
//			uint64_t start_num = (d->num - sum) / n;
//			printf("%llu | %llu + ... + %llu \n", n, start_num);
//		}
//		sum += n;
//	}
//	return p;
//}
//
////posix pthread test
//int main(int argc, char *argv[]) {
//	uint64_t num = UINT64_MAX;
//	const int cpu_num = 2;
//
//	uint64_t max_n = sqrtl(num) * sqrtl(2);
//	uint64_t one_cpu_n = max_n / cpu_num;
//	struct data ds[cpu_num];
//
//	for (int i = 0; i < cpu_num; i++) {
//		ds[i].num = num;
//		ds[i].min_n = i * one_cpu_n;
//		ds[i].max_n = ds[i].min_n + one_cpu_n - 1;
//	}
//
//	ds[0].min_n = 2;
//	ds[cpu_num - 1].max_n = max_n;
//
//	for (int i = 0; i < cpu_num; i++)
//		printf("%llu <-> %llu \n", ds[i].min_n, ds[i].max_n);
//
//	for (int i = 0; i < cpu_num; i++)
//		pthread_create(&ds[i].t, NULL, run, &ds[i]);
//
//	for (int i = 0; i < cpu_num; i++) {
//		int p[] = { 1, 2, 3 };
//		pthread_join(ds[i].t, (void**)&p);
//	}
//
//	return 0;
//}