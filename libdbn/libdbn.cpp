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

// function list for registion
static PyMethodDef libdbnMethods[] = {
	{ "foo", wrap_my_c_func, METH_VARARGS },
	{ "insertstr", insertStr, METH_VARARGS },
	{ "show", showString, METH_VARARGS },
	{NULL, NULL}
};

// module init function for regist the function list into module
// the name of module , function and some other thing  must be same
PyMODINIT_FUNC
initlibdbn(void) {
	PyObject *m = Py_InitModule("libdbn", libdbnMethods);
	if (m == NULL)
		return;
	libdbnError = PyErr_NewException("libdbn.error", NULL, NULL);
	Py_INCREF(libdbnError);
	PyModule_AddObject(m, "error", libdbnError);
}

