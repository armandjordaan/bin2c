#include <string.h>
#include <stdlib.h>

#define JSMN_HEADER
#include "jsmn.h"

#include "jsmnhelper.h"
#include "get_substr.h"

int jsmnhelper_find_property_value_idx(
    const char* json,
    const jsmntok_t* tokens,
    int num_tokens,
    const char* property_name,
    jsmntype_t type)
{
    int i;
    char temp[128];

    for(i=0; i<num_tokens; i++)
    {
        get_substr(json, temp, tokens[i].start, tokens[i].end, sizeof(temp));
        if ((strcmp(temp,property_name) == 0) &&
            (tokens[i].type & type))
        {
            return i+1;
        }
    }

    return -1;
}

int jsmnhelper_get_boolean_property_value(
    const char* json,
    const jsmntok_t* tokens,
    int num_tokens,
    const char* property_name,
    bool* value)
{
    if (value == NULL) return -1;
    if (json == NULL) return -1;
    if (tokens == NULL) return -1;
    if (property_name == NULL) return -1;

    int idx = jsmnhelper_find_property_value_idx(
                    json,
                    tokens,
                    num_tokens,
                    property_name,
                    JSMN_STRING);
    if (idx >= 0)
    {
        char temp[20];

        if (tokens[idx].type & JSMN_PRIMITIVE)
        {
            get_substr(json, temp, tokens[idx].start, tokens[idx].end, sizeof(temp));
            if (strcmp(temp,"true") == 0)
            {
                *value = true;
                return  0;
            }
            else if (strcmp(temp,"false") == 0)
            {
                *value = false;
                return  0;
            }
        }
    }

    return -1;
}

int jsmnhelper_get_integer_property_value(
    const char* json,
    const jsmntok_t* tokens,
    int num_tokens,
    const char* property_name,
    int* value)
{
    if (value == NULL) return -1;
    if (json == NULL) return -1;
    if (tokens == NULL) return -1;
    if (property_name == NULL) return -1;

    int idx = jsmnhelper_find_property_value_idx(
                    json,
                    tokens,
                    num_tokens,
                    property_name,
                    JSMN_STRING);
    if (idx >= 0)
    {
        char temp[40];

        if (tokens[idx].type & JSMN_PRIMITIVE)
        {
            get_substr(json, temp, tokens[idx].start, tokens[idx].end, sizeof(temp));
            *value = atoi(temp);
            return  0;
        }
    }

    return -1;
}

int jsmnhelper_get_string_property_value(
    const char* json,
    const jsmntok_t* tokens,
    int num_tokens,
    const char* property_name,
    char* value,
    int maxlen)
{
    if (value == NULL) return -1;
    if (json == NULL) return -1;
    if (tokens == NULL) return -1;
    if (property_name == NULL) return -1;

    int idx = jsmnhelper_find_property_value_idx(
                    json,
                    tokens,
                    num_tokens,
                    property_name,
                    JSMN_STRING);
    if (idx >= 0)
    {
        if (tokens[idx].type & JSMN_STRING)
        {
            get_substr(json, value, tokens[idx].start, tokens[idx].end, maxlen);
            return  0;
        }
    }

    return -1;
}

bool jsmnhelper_property_value_equals_string(
    const char* json,
    const jsmntok_t* tokens,
    int num_tokens,
    const char* property_name,
    const char* value)
{
    char tempvalue[128];

    int result = jsmnhelper_get_string_property_value(
        json,
        tokens,
        num_tokens,
        property_name,
        tempvalue,
        sizeof(tempvalue));

    if (result == 0)
    {
        if (strcmp(tempvalue, value) == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

