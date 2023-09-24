#include "get_substr.h"

char* get_substr(const char* str, char* result, int start, int end, int maxresultlen)
{
    int tot=0;
    int i;
    for (i = start; (i < end) && (tot < (maxresultlen-1)); i++)
    {
        result[i - start] = str[i];
        tot++;
    }
    result[i - start] = '\0';

    return result;
}