#include <string.h>
#include <errno.h>
#include <malloc.h>

#include "config.h"
#include "fileutils.h"
#include "stringhelpers.h"

#define JSMN_HEADER
#include "jsmn.h"
#include "jsmnhelper.h"

#define LOG_LEVEL   DEBUG
#include "verbose_seethe.h"

#define MAX_TOKENS      (128)    /* We expect no more than 128 JSON tokens */
#define MAX_CFG_FILELEN (4096)

int config_read(const char* filename, config_st* cfg)
{
    jsmn_parser parser;
    jsmntok_t tokens[MAX_TOKENS];
    int result;
    char* str = NULL;

    verbose_debug("filename: %s",filename);

    if (cfg == NULL)
    {
        verbose_error("config_read: cfg is NULL");
        result = -EINVAL;
        goto cleanup;
    }
    if (filename == NULL)
    {
        verbose_error("config_read: filename is NULL");
        result = -EINVAL;
        goto cleanup;
    }

    result = fileutils_read_into_buffer(filename,(void**)&str,MAX_CFG_FILELEN);
    if (result != 0)
    {
        verbose_error("config_read: fileutils_read_into_buffer failed (code %d)",result);
        goto cleanup;
    }

    jsmn_init(&parser);
    result = jsmn_parse(&parser, str, stringhelpers_strnlen(str,MAX_CFG_FILELEN), tokens, MAX_TOKENS);
    if (result == 0)
    {
        verbose_error("config_read: jsmn_parse failed (not tokens parsed)");
        result = -EIO;
        goto cleanup;
    }

    result  = jsmnhelper_get_string_property_value(str,tokens,MAX_TOKENS,"input_filename",cfg->input_filename, sizeof(cfg->input_filename));
    result += jsmnhelper_get_integer_property_value(str,tokens,MAX_TOKENS,"bytes_per_line",&(cfg->bytes_per_line));
    result += jsmnhelper_get_string_property_value(str,tokens,MAX_TOKENS,"array_name",cfg->array_name, sizeof(cfg->array_name));
    result += jsmnhelper_get_integer_property_value(str,tokens,MAX_TOKENS,"array_size",&(cfg->array_size));
    result += jsmnhelper_get_string_property_value(str,tokens,MAX_TOKENS,"array_type",cfg->array_type, sizeof(cfg->array_type));
    result += jsmnhelper_get_boolean_property_value(str,tokens,MAX_TOKENS,"array_static",&(cfg->array_static));
    result += jsmnhelper_get_boolean_property_value(str,tokens,MAX_TOKENS,"array_const",&(cfg->array_const));
    result += jsmnhelper_get_string_property_value(str,tokens,MAX_TOKENS,"c",cfg->c_filename, sizeof(cfg->c_filename));
    result += jsmnhelper_get_string_property_value(str,tokens,MAX_TOKENS,"h",cfg->h_filename, sizeof(cfg->h_filename));
    result += jsmnhelper_get_boolean_property_value(str,tokens,MAX_TOKENS,"cpp_extern",&(cfg->cpp_extern));
    result += jsmnhelper_get_string_property_value(str,tokens,MAX_TOKENS,"header_guard",cfg->header_guard, sizeof(cfg->header_guard));

    verbose_debug("config input_filename: %s",cfg->input_filename);
    verbose_debug("config bytes_per_line: %d",cfg->bytes_per_line);
    verbose_debug("config array_name: %s",cfg->array_name);
    verbose_debug("config array_size: %d",cfg->array_size);
    verbose_debug("config array_type: %s",cfg->array_type);
    verbose_debug("config array_static: %d",cfg->array_static);
    verbose_debug("config array_const: %d",cfg->array_const);
    verbose_debug("config c_filename: %s",cfg->c_filename);
    verbose_debug("config h_filename: %s",cfg->h_filename);
    verbose_debug("config cpp_extern: %d",cfg->cpp_extern);
    verbose_debug("config header_guard: %s",cfg->header_guard);

cleanup:
    if (str != NULL)
    {
        verbose_debug("config_read: fileutils_read_into_buffer cleanup");
        free(str);
        str = NULL;
    }

    return result;
}
