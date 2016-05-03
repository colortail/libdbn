#include "libdbn.h"

// function list for registion
static PyMethodDef libdbnMethods[] = {
	{ "resetBNet", resetBNet, METH_VARARGS, "重置贝叶斯网络" },
	{ "clearEvid", clearEvid, METH_VARARGS, "清除查询证据" },
	{ "clearQuery", clearQuery, METH_VARARGS, "清除查询变量" },
	{ "insertVar", insertVar, METH_VARARGS, "插入变量" },
	{ "insertEdge", insertEdge, METH_VARARGS, "插入边" },
	{ "insertTabular", insertTabular, METH_VARARGS, "单个表格型因子" },
	{ "setProbs", setProbs, METH_VARARGS, "设置表格型因子" },
	{ "removeTabular", removeTabular, METH_VARARGS, "删除表格型因子" },
	{ "showEvid", showEvid, METH_VARARGS, "显示所有证据变量值" },
	{ "showQuery", showQuery, METH_VARARGS, "显示所有查询变量" },
	{ "setEvid", setEvid, METH_VARARGS, "设置证据值（name，value）" },
	{ "setQuery", setQuery, METH_VARARGS, "设置查询变量(name)" },
	{ "jtreeQuery", varElim, METH_VARARGS, "联合树精确推理" },
	{ "varElim", varElim, METH_VARARGS, "变量消元法精确推理" }, 
	{ "paramLearning", paramLearning, METH_VARARGS, "参数学习" },
	{ "readTrainDataToBN", readTrainDataToBN, METH_VARARGS, "读取训练集数据" },
	{ "help", help, METH_VARARGS },
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

