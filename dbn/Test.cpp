#pragma once
//#pragma comment(lib,"pthreadVCE2.lib")
#pragma comment(lib,"libdbn.lib")
#include "GraphMatrix.h"
//#include <stdint.h>
//#include <pthread.h>
//#include <math.h>
//#include "libdbn.h"

extern void graphmatrixTest();

int main() {
	//graphmatrixTest();
	
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
