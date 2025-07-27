#include <stdbool.h>

#include "clean_string.h"


struct LoopState
{
    int originalOffset;
    int destinationOffset;
    bool trailingWhitespacesLeft;
    bool isCurrentCharWhitespace;
};


PyObject *processWhitespaces(const char *original, Py_ssize_t size)
{
    char *destination = calloc(size + 1, sizeof(char));

    if (!destination)
    {
        PyErr_SetString(PyExc_SystemError, "could not allocate buffer");
        return NULL;
    }

    struct LoopState loopState = {
        .originalOffset = 0,
        .destinationOffset = 0,
        .trailingWhitespacesLeft = true,
        .isCurrentCharWhitespace = false,
    };

    while (loopState.originalOffset < size)
    {
        loopState.isCurrentCharWhitespace =
            original[loopState.originalOffset] == ' ';

        if (loopState.isCurrentCharWhitespace
            && loopState.trailingWhitespacesLeft)
        {
            loopState.originalOffset++;
            continue;
        }
        loopState.trailingWhitespacesLeft = false;

        if (loopState.isCurrentCharWhitespace
            && (loopState.originalOffset + 1) < size
            && original[loopState.originalOffset + 1] == ' ')
        {
            loopState.originalOffset++;
            continue;
        }

        if (loopState.isCurrentCharWhitespace
            && loopState.originalOffset + 1 == size)
        {
            loopState.originalOffset++;
            continue;
        }

        destination[loopState.destinationOffset] =
            original[loopState.originalOffset];

        loopState.originalOffset++;
        loopState.destinationOffset++;
    }

    destination[loopState.destinationOffset] = 0;

    return PyUnicode_FromString(destination);
}


PyObject *clean_string(PyObject *module, PyObject *args)
{
    const char* original;
    Py_ssize_t bufferSize;

    // Python runtime turns this into 'TypeError'
    if (!PyArg_ParseTuple(args, "z#", &original, &bufferSize))
    {
        return NULL;
    }

    if (original == NULL)
    {
        return Py_None;
    }

    if (bufferSize == 0)
    {
        return PyUnicode_FromString("");
    }

    return processWhitespaces(original, bufferSize);
}