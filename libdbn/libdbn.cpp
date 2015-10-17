#include "libdbn.h"

//DLLEXPORT int foo(int a, int b) {
//	printf("Your input %d and %d\n", a, b);
//	return a + b;
//}
//
//DLLEXPORT char * yell() {
//	char *s = "yelling here";
//	return s;
//}


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

// function list for registion
static PyMethodDef libdbnMethods[] = {
	{ "foo", wrap_my_c_func, METH_VARARGS },
	{NULL, NULL}
};

// module init function for regist the function list into module
// the name of module , function and some other thing  must be same
PyMODINIT_FUNC
initlibdbn(void) {
	PyObject *m = Py_InitModule("libdbn", libdbnMethods);
	if (m == NULL)
		return;
	//libdbnError = PyErr_NewException("libdbn.error", NULL, NULL);
	//Py_INCREF(libdbnError);
	//PyModule_AddObject(m, "error", libdbnError);
}

