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

void inferenceTest();

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
	int sts;
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


