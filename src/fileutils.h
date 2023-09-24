#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

int fileutils_get_filesize(const char *filename, long* result_filesize);
int fileutils_read_into_buffer(const char *filename, void** result_buffer, size_t maxlen);

#ifdef __cplusplus
}
#endif


#endif // FILEUTILS_H