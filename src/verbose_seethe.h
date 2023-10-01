#ifndef VERBOSE_SEETH_H
#define VERBOSE_SEETH_H

#include <stdbool.h>
#include <stdio.h>
#include "seethe.h"

extern bool verbose_enabled_flag;

/* DEBUG LOG */
#define verbose_debug(...) do {                                                     \
    if (verbose_enabled_flag) {                                                     \
        debug(__VA_ARGS__);                                                         \
    }                                                                               \
} while (0)

/* INFO LOG */
#define verbose_info(...) do {                                                      \
    if (verbose_enabled_flag) {                                                     \
        info(__VA_ARGS__);                                                          \
    } else {                                                                        \
        printf("info:     ");                                                       \
        printf(__VA_ARGS__);                                                        \
        printf("\n");                                                               \
    }                                                                               \
} while (0)

/* NOTICE LOG */
#define verbose_notice(...) do {                                                    \
    if (verbose_enabled_flag) {                                                     \
        notice(__VA_ARGS__);                                                        \
    } else {                                                                        \
        printf("notice:   ");                                                       \
        printf(__VA_ARGS__);                                                        \
        printf("\n");                                                               \
    }                                                                               \
} while (0)

/* WARNING LOG */
#define verbose_warning(...) do {                                                   \
    if (verbose_enabled_flag) {                                                     \
        warning(__VA_ARGS__);                                                       \
    } else {                                                                        \
        printf("warning:  ");                                                        \
        printf(__VA_ARGS__);                                                        \
        printf("\n");                                                               \
    }                                                                               \
} while (0)

/* ERROR LOG */
#define verbose_error(...) do {                                                     \
    if (verbose_enabled_flag) {                                                     \
        error(__VA_ARGS__);                                                         \
    } else {                                                                        \
        printf("error:    ");                                                          \
        printf(__VA_ARGS__);                                                        \
        printf("\n");                                                               \
    }                                                                               \
} while (0)

/* CRITICAL LOG */
#define verbose_critical(...) do {                                                  \
    if (verbose_enabled_flag) {                                                     \
        critical(__VA_ARGS__);                                                      \
    } else {                                                                        \
        printf("critical: ");                                                          \
        printf(__VA_ARGS__);                                                        \
        printf("\n");                                                               \
    }                                                                               \
} while (0)


#endif // VERBOSE_SEETH_H