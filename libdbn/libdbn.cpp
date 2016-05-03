#include "libdbn.h"

// function list for registion
static PyMethodDef libdbnMethods[] = {
	{ "resetBNet", resetBNet, METH_VARARGS, "���ñ�Ҷ˹����" },
	{ "clearEvid", clearEvid, METH_VARARGS, "�����ѯ֤��" },
	{ "clearQuery", clearQuery, METH_VARARGS, "�����ѯ����" },
	{ "insertVar", insertVar, METH_VARARGS, "�������" },
	{ "insertEdge", insertEdge, METH_VARARGS, "�����" },
	{ "insertTabular", insertTabular, METH_VARARGS, "�������������" },
	{ "setProbs", setProbs, METH_VARARGS, "���ñ��������" },
	{ "removeTabular", removeTabular, METH_VARARGS, "ɾ�����������" },
	{ "showEvid", showEvid, METH_VARARGS, "��ʾ����֤�ݱ���ֵ" },
	{ "showQuery", showQuery, METH_VARARGS, "��ʾ���в�ѯ����" },
	{ "setEvid", setEvid, METH_VARARGS, "����֤��ֵ��name��value��" },
	{ "setQuery", setQuery, METH_VARARGS, "���ò�ѯ����(name)" },
	{ "jtreeQuery", varElim, METH_VARARGS, "��������ȷ����" },
	{ "varElim", varElim, METH_VARARGS, "������Ԫ����ȷ����" }, 
	{ "paramLearning", paramLearning, METH_VARARGS, "����ѧϰ" },
	{ "readTrainDataToBN", readTrainDataToBN, METH_VARARGS, "��ȡѵ��������" },
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

