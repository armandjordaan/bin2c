#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <errno.h>

#include "fileutils.h"

#define LOG_LEVEL   DEBUG
#include "seethe.h"

int fileutils_get_filesize(const char *filename, long* result_filesize)
{
    // determine the file length in bytes of "filename"
    FILE* fp = NULL;
    long filesize;
    int result;

    debug("fileutils_get_filesize: %s", filename);

    if (filename == NULL)
    {
        error("fileutils_get_filesize: filename is NULL");
        result = -EINVAL;
        goto cleanup;
    }
    if ((fp = fopen(filename,"rb")) == NULL) // flawfinder: ignore
    {
        error("fileutils_get_filesize: fopen failed");
        result = -EIO;
        goto cleanup;
    }
    if (fseek(fp, 0, SEEK_END) != 0)
    {
        error("fileutils_get_filesize: fseek failed");
        result = -EIO;
        goto cleanup;
    }
    filesize = ftell(fp);

    if (fclose(fp) != 0)
    {
        error("fileutils_get_filesize: fclose failed");
        result = -EIO;
        goto cleanup;
    }
    fp = NULL;
    result = 0;

cleanup:
    if (fp != NULL)
    {
        warning("fileutils_get_filesize: fclose cleanup");
        fclose(fp);
        fp = NULL;
    }

    debug("fileutils_get_filesize: filesize: %ld", filesize);
    *result_filesize = filesize;

    debug("fileutils_get_filesize: result: %d", result);
    return result;
}

int fileutils_read_into_buffer(const char *filename, void** result_buffer, size_t maxlen)
{
    long filesize;
    void* buffer;
    int result=0;
    size_t numread;
    FILE* fp = NULL;

    if (filename == NULL)
    {
        error("fileutils_read_into_buffer: filename is NULL");
        result = -EINVAL;
        goto cleanup;
    }
    if (result_buffer == NULL)
    {
        error("fileutils_read_into_buffer: result_buffer is NULL");
        result = -EINVAL;
        goto cleanup;
    }
    if (maxlen == 0)
    {
        error("fileutils_read_into_buffer: maxlen is 0");
        result = -EINVAL;
        goto cleanup;
    }

    buffer = *result_buffer;
    if (fileutils_get_filesize(filename,&filesize) != 0)
    {
        error("fileutils_read_into_buffer: fileutils_get_filesize failed");
        result = -EIO;
        goto cleanup;
    }
    debug("fileutils_read_into_buffer: filesize: %ld", filesize);

    if ((size_t)filesize > maxlen)
    {
        error("fileutils_read_into_buffer: filesize too large");
        result = -EFBIG;
        goto cleanup;
    }

    buffer = malloc((size_t)filesize);
    if (buffer == NULL)
    {
        error("fileutils_read_into_buffer: malloc failed");
        result = -ENOMEM;
        goto cleanup;
    }

    if ((fp = fopen(filename,"rb")) == NULL) // flawfinder: ignore
    {
        error("fileutils_read_into_buffer: fopen failed");
        result = -EIO;
        goto cleanup;
    }
    if (fseek(fp, 0, SEEK_SET) != 0)
    {
        error("fileutils_read_into_buffer: fseek failed");
        result = -EIO;
        goto cleanup;
    }
    numread = fread(buffer,sizeof(uint8_t),filesize,fp);
    if (numread != (size_t)filesize)
    {
        error("fileutils_read_into_buffer: fread failed read: %zu vs filesize: %ld", numread, filesize);
        result = -EIO;
        goto cleanup;
    }

    if (fclose(fp) != 0)
    {
        error("fileutils_read_into_buffer: fclose failed");
        result = -EIO;
        goto cleanup;
    }
    fp = NULL;
    *result_buffer = buffer;

cleanup:
    if ((buffer != NULL) && (result != 0))
    {
        warning("fileutils_read_into_buffer: buffer cleanup");
        free(buffer);
        buffer = NULL;
    }
    if (fp != NULL)
    {
        warning("fileutils_read_into_buffer: fclose cleanup");
        fclose(fp);
        fp = NULL;
    }

    return result;
}
