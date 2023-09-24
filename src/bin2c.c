#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <malloc.h>
#include <time.h>

#include "bin2c.h"
#include "config.h"
#include "fileutils.h"

#define LOG_LEVEL   DEBUG
#include "verbose_seethe.h"

static int write_auto_generated_header(FILE* fp, const config_st* cfg, const char* filename)
{
    int result;

    if (fp == NULL)
    {
        return -EINVAL;
    }
    if (filename == NULL)
    {
        return -EINVAL;
    }

    result = 0;

    fprintf(fp,"\n// Auto generated file - please do not edit\n");
    fprintf(fp,"// Generated from %s\n", cfg->input_filename);
    fprintf(fp,"// Filename: %s \n", filename);

    // write the current date and time to file fp
    time_t now = time(0);
    struct tm *t = localtime(&now);
    fprintf(fp,"// Date: %d-%02d-%02d %02d:%02d:%02d\n\n", (t->tm_year + 1900), (t->tm_mon + 1), t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

    return result;
}

int bin2c_write_buffer_c_file(const uint8_t* buffer, size_t buffer_size, const config_st* cfg)
{
    FILE* fpout;
    int errcode;
    size_t i,j;
    size_t offs;
    size_t max_line_length;

    char* temp;   // flawfinder: Ignore

    if (cfg == NULL)
    {
        return -EINVAL;
    }
    if (buffer == NULL)
    {
        return -EINVAL;
    }
    if (cfg->c_filename == NULL)
    {
        return -EINVAL;
    }
    if (buffer_size == 0)
    {
        return -EINVAL;
    }

    max_line_length = 80 + 6*cfg->bytes_per_line;
    temp = (char*)malloc(max_line_length); // flawfinder: ignore

    if ((fpout = fopen(cfg->c_filename,"wt")) == NULL) // flawfinder: ignore
    {
        verbose_error("Cannot open output file for writing: %s",cfg->c_filename);
        errcode = -EIO;
        goto cleanup;
    }

    fprintf(fpout,"#include <stdint.h>\n");
    fprintf(fpout,"#include <stdbool.h>\n");
    fprintf(fpout,"#include \"%s\"\n\n", cfg->h_filename);

    write_auto_generated_header(fpout, cfg, cfg->c_filename);

    if (cfg->array_static)
    {
        fprintf(fpout,"static ");
    }
    if (cfg->array_const)
    {
        fprintf(fpout,"const ");
    }

    if (cfg->array_size > 0)
    {
        fprintf(fpout,"%s %s[%d] = {\n", cfg->array_type, cfg->array_name, cfg->array_size);
    }
    else
    {
        fprintf(fpout,"%s %s[] = {\n", cfg->array_type, cfg->array_name);
    }

    for(i=0; i<buffer_size; i+=cfg->bytes_per_line)
    {
        temp[0] = 0;
        offs = 0;
        offs += snprintf(temp+offs, max_line_length-offs, "    /* %08x */ ", (unsigned int)i);
        for(j=i; (j<(i+cfg->bytes_per_line)) && (j<buffer_size); j++)
        {
            offs += snprintf(temp+offs, max_line_length-offs, "0x%02x", buffer[j]);

            if ((j+1) < buffer_size)
            {
                offs += snprintf(temp+offs, max_line_length-offs, ",");
            }
        }
        fprintf(fpout,"%s\n", temp);
    }
    fprintf(fpout,"\n};\n");

    errcode = 0;

cleanup:
    if (fpout != NULL)
    {
        fclose(fpout);
    }
    if (temp != NULL)
    {
        free(temp);
    }

    return errcode;
}

int bin2c_write_buffer_h_file(const uint8_t* buffer, size_t buffer_size, const config_st* cfg)
{
    FILE* fpout;
    int errcode;
    size_t max_line_length;

    char* temp;   // flawfinder: Ignore

    if (cfg == NULL)
    {
        return -EINVAL;
    }
    if (buffer == NULL)
    {
        return -EINVAL;
    }
    if (cfg->c_filename == NULL)
    {
        return -EINVAL;
    }
    if (buffer_size == 0)
    {
        return -EINVAL;
    }

    max_line_length = 80 + 6*cfg->bytes_per_line;
    temp = (char*)malloc(max_line_length); // flawfinder: ignore

    if ((fpout = fopen(cfg->h_filename,"wt")) == NULL) // flawfinder: ignore
    {
        verbose_error("Cannot open output file for writing: %s",cfg->h_filename);
        errcode = -EIO;
        goto cleanup;
    }

    fprintf(fpout,"#ifndef %s\n", cfg->header_guard);
    fprintf(fpout,"#define %s\n", cfg->header_guard);

    write_auto_generated_header(fpout, cfg, cfg->h_filename);

    fprintf(fpout,"#include <stdint.h>\n");
    fprintf(fpout,"#include <stdbool.h>\n");


    if (cfg->cpp_extern)
    {
        fprintf(fpout,"\n#ifdef __cplusplus\n");
        fprintf(fpout,"extern \"C\" {\n");
        fprintf(fpout,"#endif\n\n");
    }

    if (!cfg->array_static)
    {
        fprintf(fpout,"extern ");
        if (cfg->array_const)
        {
            fprintf(fpout,"const ");
        }

        if (cfg->array_size > 0)
        {
            fprintf(fpout,"%s %s[%d];\n", cfg->array_type, cfg->array_name, cfg->array_size);
        }
        else
        {
            fprintf(fpout,"%s %s[];\n", cfg->array_type, cfg->array_name);
        }
    }

    if (cfg->cpp_extern)
    {
        fprintf(fpout,"\n#ifdef __cplusplus\n");
        fprintf(fpout,"}\n");
        fprintf(fpout,"#endif\n\n");
    }
    fprintf(fpout,"#endif // %s\n", cfg->header_guard);

    errcode = 0;

cleanup:
    if (fpout != NULL)
    {
        fclose(fpout);
    }
    if (temp != NULL)
    {
        free(temp);
    }

    return errcode;
}

int bin2c_write(const config_st* cfg)
{
    uint8_t* input_buffer = NULL;
    FILE* fpin = NULL;
    int errcode;
    long filesize = 0;
    size_t numread;

    if (cfg == NULL)
    {
        return -EINVAL;
    }
    if (cfg->input_filename == NULL)
    {
        return -EINVAL;
    }
    if (cfg->c_filename == NULL)
    {
        return - EINVAL;
    }
    if (cfg->h_filename == NULL)
    {
        return - EINVAL;
    }
    if (cfg->array_name == NULL)
    {
        return - EINVAL;
    }
    if (cfg->array_type == NULL)
    {
        return - EINVAL;
    }
    if (cfg->header_guard == NULL)
    {
        return - EINVAL;
    }

    if (fileutils_get_filesize(cfg->input_filename,&filesize) != 0)
    {
        verbose_error("Cannot get file size of %s",cfg->input_filename);
        errcode = -EIO;
        goto cleanup;
    }
    input_buffer = (uint8_t *)malloc((size_t)filesize);
    if (input_buffer == NULL)
    {
        verbose_error("Cannot allocate memory for input buffer");
        errcode = -ENOMEM;
        goto cleanup;
    }

    if ((fpin = fopen(cfg->input_filename,"rb")) == NULL) // flawfinder: ignore
    {
        verbose_error("Cannot open file %s",cfg->input_filename);
        return -EIO;
    }

    numread = fread(input_buffer,sizeof(uint8_t),filesize,fpin);
    if (numread != (size_t)filesize)
    {
        verbose_error("Cannot seek on file %s",cfg->input_filename);
        errcode = -EIO;
        goto cleanup;
    }

    errcode = bin2c_write_buffer_c_file(input_buffer,numread,cfg);
    errcode += bin2c_write_buffer_h_file(input_buffer,numread,cfg);

cleanup:
    if (fpin != NULL)
    {
        fclose(fpin);
    }

    if (input_buffer != NULL)
    {
        free(input_buffer);
    }

    return errcode;
}
