﻿#pragma once
#include <stdio.h>
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
static BNet bn;
static set<string> q;
static unordered_map<string, double> e;
static InfEngine* pInf = InfEngine::getInstance();

static PyObject* resetBNet(PyObject* self, PyObject* args) {
	for (int i = bn.vertexSize() - 1; 0 <= i; i--) {
		bn.remove(i);
	}
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* clearEvid(PyObject* self, PyObject* args) {
	e.clear();
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* clearQuery(PyObject* self, PyObject* args) {
	q.clear();
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* showEvid(PyObject* self, PyObject* args) {
	printf("证据变量： ");
	for (unordered_map<string, double>::iterator it = e.begin();
		it != e.end();
		it++) {
		printf("%s: %f  \n", it->first.c_str(), it->second);
	}
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* showQuery(PyObject* self, PyObject* args) {
	printf("查询变量集合： ");
	for (set<string>::iterator it = q.begin();
		it != q.end();
		it++) {
		printf("%s  ", it->c_str());
	}
	putchar('\n');
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* setEvid(PyObject* self, PyObject *args) {
	
	const char * evid;
	double eV;

	if (!PyArg_ParseTuple(args, "sd", &evid, &eV)) {
		PyErr_SetString(libdbnError, "参数异常");
		return NULL;
	}

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

	string qstr(query);
	if (q.find(qstr) != q.end()) {
		q.insert(qstr);
	}
	else {
		printf("变量 %s 已存在查询集合中\n", query);
		printf("查询变量集合： ");
		for (set<string>::iterator it = q.begin();
			it != q.end();
			it++) {
			printf("%s  ", it->c_str());
		}
		putchar('\n');
	}
	
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* jtreeQuery(PyObject* self, PyObject *args){

	vector<string> query(q.begin(), q.end());
	Factor result = pInf->inference(bn, query, e, JTreeInference());

	InOutUtils::stdPrint(result);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* varElim(PyObject* self, PyObject *args){

	vector<string> query(q.begin(), q.end());
	Factor result = pInf->inference(bn, query, e, VariableElimination());

	InOutUtils::stdPrint(result);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* insertVar(PyObject* self, PyObject* args) {
	
	int num = bn.vertexSize();
	const char * name;
	if (!PyArg_ParseTuple(args, "s", &name)) {
		PyErr_SetString(libdbnError, "参数异常");
		return NULL;
	}

	bn.insert(RandVar(num, std::string(name),2));
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* insertEdge(PyObject* self, PyObject* args) {

	int start;
	int end;
	if (!PyArg_ParseTuple(args, "ii", &start, &end)) {
		PyErr_SetString(libdbnError, "参数异常");
		return NULL;
	}
	bn.insert(0, 0, start, end);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* insertTabular(PyObject* self, PyObject* args) {

	if (!PyArg_ParseTuple(args, "ii")) {
		PyErr_SetString(libdbnError, "参数异常");
		return NULL;
	}
	vector<std::string> name;
	vector<bool> pa;


	Factor fact(name);
	bn.insertCPT(fact);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* removeTabular(PyObject* self, PyObject* args) {

	if (!PyArg_ParseTuple(args, "ii")) {
		PyErr_SetString(libdbnError, "参数异常");
		return NULL;
	}

	//bn.removeCPT(fact);
	Py_INCREF(Py_None);
	return Py_None;
}

// c/c++ native function

//wrap c function to Python function
static PyObject* help(PyObject* self, PyObject * args) {
	
	printf("==========说明文档===========\n");

	return Py_None;
}