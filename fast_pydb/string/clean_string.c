#include <stdbool.h>

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


void fillDestinationString(
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
    // We allocate the destination buffer. Note that whenever any heap
    // allocation is done, the buffer must ALWAYS be freed later! Python
    // cannot "know" whether the value transferred to the interpreter is
    // heap- or stack-allocated, so Python won't do anything and we are
    // responsible for freeing up ourselves!
    char *destination = calloc(size + 1, sizeof(char));

    if (!destination)
    {
        PyErr_SetString(
            PyExc_SystemError,
            "insufficient memory to allocate internal buffer");
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

    // Transfer destination value to python and free up our internal buffer.
    PyObject *pythonResult = PyUnicode_FromString(destination);
    free(destination);
    return pythonResult;
}


PyObject *clean_string(PyObject *module, PyObject *args)
{
    const char* original;
    const Py_ssize_t bufferSize = -1;

    // This effectively exposes following signature and raises TypeError in case
    // the argument count and type constraints are not met:
    //
    //   def clean_string(original: str | None) -> str | None: ...
    //
    // 'const char *original' on C side is
    // - a regular '\0'-terminated C string, if Python 'original' is not None
    // - NULL, if Python 'original' is None
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

    return processWhitespaces(original, bufferSize);
}
