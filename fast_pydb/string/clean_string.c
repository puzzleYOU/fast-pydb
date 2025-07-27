#include <stdbool.h>

#include "../_common/macros.h"
#include "clean_string.h"


/*
 * Data struct for reflecting the algorithm loop state.
 * Each field description relates to "this is the variable's state when
 * entering the loop".
 */
typedef struct LoopState
{
    /*
     * The original string position which will be read from next.
     */
    int originalOffset;

    /*
     * The destination string position which will be written to next.
     */
    int destinationOffset;

    /*
     * The original string size. Immutable.
     */
    const Py_ssize_t originalSize;

    /*
     * Indicates that not all leading whitespaces have been visited.
     */
    bool leadingWhitespacesLeft;

    /*
     * Shorthand: Is the current character a whitespace?
     * This flag is meant to be set first when re-entering the loop.
     */
    bool isCurrentCharWhitespace;
} LoopState;


inline void fillDestinationString(
    const char *original,
    char *destination,
    LoopState *loopState) 
{
    /*
     * Strips duplicate whitespaces and writes them into 'destination'.
     * 
     * The steps are on intent not flowing in order, e.g. (4) following (2).
     */
    while (loopState->originalOffset < loopState->originalSize)
    {
        // (0): initialize shorthand 'isCurrentCharWhitespace'
        loopState->isCurrentCharWhitespace =
            original[loopState->originalOffset] == ' ';

        // (1): We walk through the original string without writing anything to
        // destination to just skip all leading whitespaces.
        if (loopState->isCurrentCharWhitespace
            && loopState->leadingWhitespacesLeft)
        {
            loopState->originalOffset++;
            continue;
        }

        // (2): We have visited all leading whitespaces and update the loop state
        // accordingly to write the destination string.
        loopState->leadingWhitespacesLeft = false;

        // (4): We have visited all leading whitespaces and the current character
        // is an intermediate whitespace.
        // - If it is followed by another one, SKIP. The following scenario will
        //   apply to the next iteration step(s).
        // - If it is not followed by another one, we have either deduplicated
        //   multiple intermediate whitespaces or are just visiting a single
        //   one. In this case, the condition does NOT SKIP so the single
        //   whitespace is emitted into destination.
        if (loopState->isCurrentCharWhitespace
            && (loopState->originalOffset + 1) < loopState->originalSize
            && original[loopState->originalOffset + 1] == ' ')
        {
            loopState->originalOffset++;
            continue;
        }

        // (5): This applies to a trailing whitespace. Because part (4) has
        // already deduplicated possible multiple whitespaces, we are done
        // here and break out the loop.
        if (loopState->isCurrentCharWhitespace
            && loopState->originalOffset + 1 == loopState->originalSize)
        {
            loopState->originalOffset++;
            break;
        }

        // (3): We are emitting a single whitespace or any other character and
        // walk further.
        destination[loopState->destinationOffset] =
            original[loopState->originalOffset];
        loopState->originalOffset++;
        loopState->destinationOffset++;
    }

    // We finalize the string with the null character to determine the string
    // ends here.
    destination[loopState->destinationOffset] = 0;
}

PyObject *processWhitespaces(const char *original, const Py_ssize_t size)
{
    char *destination = calloc(size + 1, sizeof(char));

    if (!destination)
    {
        PyErr_SetString(PyExc_SystemError, "could not allocate buffer");
        return NULL;
    }

    LoopState loopState = {
        .originalOffset = 0,
        .destinationOffset = 0,
        .originalSize = size,
        .leadingWhitespacesLeft = true,
        .isCurrentCharWhitespace = false,
    };

    fillDestinationString(original, destination, &loopState);

    return PyUnicode_FromString(destination);
}


PyObject *clean_string(const PyObject *module, const PyObject *args)
{
    UNUSED(module);

    const char* original;
    const Py_ssize_t bufferSize = -1;

    // Python runtime turns this into 'TypeError'
    if (!PyArg_ParseTuple((PyObject *) args,
                          "z#",
                          &original,
                          &bufferSize))
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