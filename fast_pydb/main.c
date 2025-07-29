#include "string/module.h"

static PyMethodDef fast_pydb_methods[] = {
    {NULL, NULL, 0, NULL}  // Sentinel
};

static struct PyModuleDef fast_pydb = {
    .m_base = PyObject_HEAD_INIT(_Py_NULL)
    .m_name = "fast_pydb",
    .m_doc = "TODO fast_pydb docstring",
    .m_size = -1,
    .m_methods = fast_pydb_methods,
};


PyMODINIT_FUNC PyInit_fast_pydb(void) {
    PyObject *root = PyModule_Create(&fast_pydb);
    PyModule_AddObject(root, "string", PyModule_Create(&string));
    return root;
}