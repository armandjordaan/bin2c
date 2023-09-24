#include "stringhelpers.h"

// determine the length of a string, but no more than maxlen
// maxlen can be zero, in which case the function returns 0
size_t stringhelpers_strnlen(const char *s, size_t maxlen)
{
    size_t len = 0;

    // Check for NULL pointer, and return 0 if NULL
    if (s == NULL)
        return 0;

    // Its OK that the function does not guarantee that a null character will
    // be found before the maxlen number of characters. That is exactly what we are trying to achieve
    while (len < maxlen && s[len])
        len++;

    return len;
}
