#ifndef JSMN_HELPER_H
#define JSMN_HELPER_H

#include <stdbool.h>


#define JSMN_HEADER
#include "jsmn.h"

/**
 * @brief find the index of a property value
 *
 * @param json the original json string
 * @param tokens parsed tokens
 * @param num_tokens number of parsed tokens
 * @param property_name property name to search for
 * @param type jsmn property type to look for
 * @return int index of the value of the property, -1 if not found
 */
int jsmnhelper_find_property_value_idx(
    const char* json,
    const jsmntok_t* tokens,
    int num_tokens,
    const char* property_name,
    jsmntype_t type);

/**
 * @brief Get a boolean property value
 *
 * @param json original json string
 * @param tokens pointer to parsed tokens
 * @param num_tokens number of parsed tokens
 * @param property_name property name to look for
 * @param value pointer to result variable
 * @return int 0 = success, -1 if error or property not found
 */
int jsmnhelper_get_boolean_property_value(
    const char* json,
    const jsmntok_t* tokens,
    int num_tokens,
    const char* property_name,
    bool* value);

/**
 * @brief Get an integer property value
 *
 * @param json original json string
 * @param tokens pointer to parsed tokens
 * @param num_tokens number of parsed tokens
 * @param property_name property name to look for
 * @param value pointer to result variable
 * @return int 0 = success, -1 if error or property not found
 */
int jsmnhelper_get_integer_property_value(
    const char* json,
    const jsmntok_t* tokens,
    int num_tokens,
    const char* property_name,
    int* value);

/**
 * @brief Get a string property value
 *
 * @param json original json string
 * @param tokens pointer to parsed tokens
 * @param num_tokens number of parsed tokens
 * @param property_name property name to look for
 * @param value pointer to result variable
 * @param maxlen maximum length of result string
 * @return int 0 = success, -1 if error or property not found
 */
int jsmnhelper_get_string_property_value(
    const char* json,
    const jsmntok_t* tokens,
    int num_tokens,
    const char* property_name,
    char* value,
    int maxlen);

/**
 * @brief Check if a property name is equals to a string value
 * 
 * @param json oribinal json string
 * @param tokens parsed tokens
 * @param num_tokens Number of tokens
 * @param property_name property name to look for
 * @param value string value to check
 */
bool jsmnhelper_property_value_equals_string(
    const char* json,
    const jsmntok_t* tokens,
    int num_tokens,
    const char* property_name,
    const char* value);

#endif // JSMN_HELPER_H
