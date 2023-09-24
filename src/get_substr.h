#ifndef GET_SUBSTR_H
#define GET_SUBSTR_H

/**
 * @brief Get the substr from string from start to end
 * 
 * @param str the original string
 * @param result result where the substring is to be stored
 * @param start start of the substring
 * @param end wend of the substring
 * @param maxresultlen maximum lenght for the result
 * @return char* the result is returned for convenience
 */
char* get_substr(const char* str, char* result, int start, int end, int maxresultlen);


#endif // GET_SUBSTR_H