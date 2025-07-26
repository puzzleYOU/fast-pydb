#include "clean_string.h"

static PyMethodDef string_methods[] = {
    {
        "clean_string",
        clean_string,
        METH_VARARGS,
        "Trims unnecessary whitespaces from a string. \n\
         >>> clean_string() \n\
         Traceback (most recent call last):\n\
         ...\n\
         TypeError: function takes exactly 1 argument (0 given)\n\
         >>> clean_string(1) \n\
         Traceback (most recent call last):\n\
         ...\n\
         TypeError: a bytes-like object is required, not 'int'\n\
         >>> clean_string(1,2,3)\n\
         Traceback (most recent call last):\n\
         ...\n\
         TypeError: function takes exactly 1 argument (3 given)\n\
         >>> clean_string(None) \n\
         >>> clean_string('')\n\
         ''\n\
         >>> clean_string('   ')\n\
         ''\n\
         >>> clean_string('abc')\n\
         'abc'\n\
         >>> clean_string('   abc') \n\
         'abc'\n\
         >>> clean_string('abc    ') \n\
         'abc'\n\
         >>> clean_string('   abc    ') \n\
         'abc'\n\
         >>> clean_string('   a b c    ') \n\
         'a b c'\n\
         >>> clean_string('   a    b c    ') \n\
         'a b c'\n\
         >>> clean_string('   a    b       c    ') \n\
         'a b c'"
    },
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef string = {
    .m_base = PyObject_HEAD_INIT(_Py_NULL)
    .m_name = "fast_pydb.string",
    .m_doc = "various string utility functions",
    .m_size = -1,
    .m_methods = string_methods,
};