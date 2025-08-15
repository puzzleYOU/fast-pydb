#include <stdbool.h>

#include "clean_string.h"


/*
 * Data struct for reflecting the algorithm loop state.
 * Each field description relates to "this is the variable's state when
 * entering the loop".
 */
typedef struct {
    /*
     * The original string position which will be read from next.
     */
    unsigned int originalOffset;

    /*
     * The destination string position which will be written to next.
     */
    unsigned int destinationOffset;

    /*
     * The original string size. Immutable.
     */
    const uint originalSize;

    /*
     * Indicates that not all leading whitespaces have been visited.
     */
    bool leadingWhitespacesLeft;

    /*
     * Shorthands for what the current character is.
     * This flag is meant to be set first when re-entering the loop.
     */
    bool isCurrentCharWhitespace;
    bool isCurrentCharLineBreak;
} LoopState;


void fillDestinationStringWithoutLeadingEntities(
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
        // (0): initialize shorthands
        loopState->isCurrentCharWhitespace =
            original[loopState->originalOffset] == ' ';
        loopState->isCurrentCharLineBreak =
            original[loopState->originalOffset] == '\r'
            || original[loopState->originalOffset] == '\n';

        // (1): We walk through the original string without writing anything to
        // destination to just skip all leading whitespaces.
        if (loopState->leadingWhitespacesLeft
            && (loopState->isCurrentCharWhitespace
                || loopState->isCurrentCharLineBreak))
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

void trimAllTrailingNewlines(char *destination, LoopState* loopState)
{
    // We must walk a bit back as there still may be trailing specials.
    // First, trim all trailing newlines...
    while (destination[--loopState->destinationOffset] == '\n') {
      destination[loopState->destinationOffset] = '\0';
    }
}

void trimAllTrailingWhitespaces(char *destination, const LoopState* loopState)
{
    // ... and only then remove the possible remaining trailing whitespace.
    while (destination[loopState->destinationOffset] == ' ') {
      destination[loopState->destinationOffset] = '\0';
    }
}

/*
 * Cleans the source string according following rules:
 * - redundant spaces
 * - trailing and leading whitespaces
 * - trailing and leading newlines
 */
void cleanString(const char *source, char *destination, const uint size)
{
    LoopState state = {
        .originalOffset = 0,
        .destinationOffset = 0,
        .originalSize = size,
        .leadingWhitespacesLeft = true,
        .isCurrentCharWhitespace = false,
    };

    fillDestinationStringWithoutLeadingEntities(source, destination, &state);
    trimAllTrailingNewlines(destination, &state);
    trimAllTrailingWhitespaces(destination, &state);
}