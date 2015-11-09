#pragma once
//#pragma comment(lib,"pthreadVCE2.lib")
//#pragma comment(lib,"libdbn.lib")
#include "GraphMatrix.h"
#include "Factor.h"
#include "BNet.h"
#include "Metric.h"
#include "InfEngine.h"
#include "InferStrategy.h"
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
	//vv.push_back("C");
	//vv.push_back("D");

	vector<std::string> v1;
	v1.push_back("B");
	v1.push_back("D");

	Factor fact1(vv);
	Factor fact2(v1);

	//set<Factor> sfs;
	//sfs.insert(fact1);
	//sfs.insert(fact2);
	//printf("fact1 < fact1 ? %d\n",fact1 < fact1);
	printf("fact1 < fact2 ? %d\n", fact1 < fact2);
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

	//InOutUtils::stdPrintDBnet(bn);

	//jtree test
	InfEngine* pInf = InfEngine::getInstance();
	//max cardinality
	//vector<int> pi = pInf->maxCardinalitySearch(bn);
	vector<int> pi = pInf->greedyOrdering(bn, MinFill());
	//for (uint32_t i = 0; i < pi.size(); i++)
	//	std::cout << bn.vertex(pi[i]).name << std::endl;
	JTree jtree;
	pInf->buildJTree(jtree, bn, pi);
	//InOutUtils::stdPrintJTree(jtree);
}

void templatefooTest() {
	vector<string> v1 = { "aaa", "bbb", "ccc", "dd" };
	vector<string> v2 = { "aaa", "bbb" };
	vector<string> ss = libdbn::get2VectorSubstract(v1, v2);
	for (unsigned int i = 0; i < ss.size(); i++)
		std::cout << ss[i] << std::endl;
}

void inferenceTest() {
	BNet bn;
	bn.insert(RandVar(0, std::string("A")));
	bn.insert(RandVar(1, std::string("B")));
	bn.insert(RandVar(2, std::string("C")));
	bn.insert(RandVar(3, std::string("D")));
	bn.insert(RandVar(4, std::string("E")));
	bn.insert(0, 0, 0, 2);
	bn.insert(0, 0, 0, 3);
	bn.insert(0, 0, 2, 4);
	bn.insert(0, 0, 1, 3);

	vector<std::string> pa;
	pa.push_back("A");
	vector<std::string> pb;
	pb.push_back("B");
	vector<std::string> pc;
	pc.push_back("A");
	pc.push_back("C");
	vector<std::string> pd;
	pd.push_back("A");
	pd.push_back("B");
	pd.push_back("D");
	vector<std::string> pe;
	pe.push_back("C");
	pe.push_back("E");
	Factor fa(pa);
	Factor fb(pb);
	Factor fc(pc);
	Factor fd(pd);
	Factor fe(pe);
	
	vector<double> vp1;
	vp1.push_back(0.7);
	vp1.push_back(0.3);
	vector<double> vp2;
	vp2.push_back(0.4);
	vp2.push_back(0.6);
	vector<double> vp3;
	vp3.push_back(0.6);
	vp3.push_back(0.4);
	vp3.push_back(0.2);
	vp3.push_back(0.8);
	vector<double> vp4;
	vp4.push_back(0.8);
	vp4.push_back(0.2);
	vp4.push_back(0.9);
	vp4.push_back(0.1);
	vp4.push_back(0.2);
	vp4.push_back(0.8);
	vp4.push_back(0.3);
	vp4.push_back(0.7);
	vector<double> vp5;
	vp5.push_back(0.8);
	vp5.push_back(0.2);
	vp5.push_back(0.3);
	vp5.push_back(0.7);
	
	fa.setProb(vp1);
	fb.setProb(vp2);
	fc.setProb(vp3);
	fd.setProb(vp4);
	fe.setProb(vp5);

	set<Factor> sfs;
	sfs.insert(fa);
	sfs.insert(fb);
	sfs.insert(fc);
	sfs.insert(fd);
	sfs.insert(fe);

	bn.setCPTs(sfs);

	InfEngine* pInf = InfEngine::getInstance();

	vector<string> q;
	q.push_back("A");
	q.push_back("D");
	unordered_map<string, double> e;
	e.insert({"B", 0});


	// VE
	//Factor result = pInf->inference(bn, q, e, VE);
	//InOutUtils::stdPrint(result);
	
	//pInf->eliminate(sfs,vector<string>(1,"B"));
	//for (auto it = sfs.begin(); it != sfs.end(); it++) {
	//	InOutUtils::stdPrint(*it);
	//}



}


int main() {
	BenchMark bm;
	//graphmatrixTest();
	//factorTest();
	//bnetTest();
	inferenceTest();
	
	bm.timeTest();
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