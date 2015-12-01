#pragma once
#include <stdio.h>
#include <vector>
#include <iostream>

#include "BNet.h"
#include "Clique.h"
#include "DBNet.h"
#include "Factor.h"
#include "Graph.h"
#include "GraphMatrix.h"
#include "InfEngine.h"
#include "InferStrategy.h"
#include "InOutUtils.h"
#include "JTree.h"
#include "Metric.h"
#include "utils.h"

#ifndef BUILD_DLL
#define DLLEXPORT extern "C" __declspec(dllexport) 
#else
#define EXPORT __declspec(dllimport)
#endif

//DLLEXPORT int foo(int a, int b);
//
//DLLEXPORT char * yell();

#include "Python.h"

static PyObject * libdbnError;

//static DBNet dbnet;
static std::vector<std::string> strs;

extern void inferenceTest();

// c/c++ native function
int my_c_function(const char * arg) {
	int n = system(arg);
	
	return n;
}

//wrap c function to Python function
static PyObject* wrap_my_c_func(PyObject* self, PyObject * args) {
	const char * command;
	int sts;
	if (!PyArg_ParseTuple(args, "s", &command))
		return NULL;
	sts = my_c_function(command);
	return Py_BuildValue("i", sts);
}


void insertString(const char * arg) {
	strs.push_back(arg);
}

void showString() {
	for (int i = 0; i < strs.size(); i++)
		std::cout << strs[i].c_str() << std::endl;
}

static PyObject* insertStr(PyObject* self, PyObject *args) {
	const char * command;
	//int sts;
	if (!PyArg_ParseTuple(args, "s", &command)) {
		PyErr_SetString(libdbnError, "参数异常");
		return NULL;
	}
	insertString(command);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* showString(PyObject* self, PyObject *args) {
	showString();
	Py_INCREF(Py_None);
	return Py_None;
}


BNet bn;
vector<string> q;
unordered_map<string, double> e;

static PyObject* setEvid(PyObject* self, PyObject *args) {
	const char * evid;
	double eV;

	PyArg_ParseTuple(args, "sd", &evid, &eV);

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

	
	e.insert({ string(evid), eV });

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* setQuery(PyObject* self, PyObject *args) {
	
	const char * query;
	if (!PyArg_ParseTuple(args, "s", &query)) {
		PyErr_SetString(libdbnError, "参数异常");
		return NULL;
	}
	

	q.push_back(string(query));
	
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* jtreeQuery(PyObject* self, PyObject *args){

	InfEngine* pInf = InfEngine::getInstance();
	Factor result = pInf->inference(bn, q, e, JTreeInference());

	InOutUtils::stdPrint(result);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* varElim(PyObject* self, PyObject *args){

	InfEngine* pInf = InfEngine::getInstance();
	Factor result = pInf->inference(bn, q, e, VariableElimination());

	InOutUtils::stdPrint(result);
	Py_INCREF(Py_None);
	return Py_None;
}

