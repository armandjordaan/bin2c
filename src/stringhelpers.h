#ifndef STRINGHELPERS_H
#define STRINGHELPERS_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief determine the length of a string, but no more than maxlen
 *
 * @param s - string to measure
 * @param maxlen - maximum length to measure
 * @return size_t - length of string, but no more than maxlen
 */
size_t stringhelpers_strnlen(const char *s, size_t maxlen);

#ifdef __cplusplus
}
#endif


#endif // STRINGHELPERS_H
