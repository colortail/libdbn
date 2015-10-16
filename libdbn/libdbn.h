#pragma once
#include <stdio.h>

#ifndef BUILD_DLL
#define DLLEXPORT extern "C" __declspec(dllexport) 
#else
#define EXPORT __declspec(dllimport)
#endif

//DLLEXPORT int foo(int a, int b);
//
//DLLEXPORT char * yell();

#include "Python.h"