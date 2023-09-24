#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CONFIG_MAX_STR_LEN (80)
#define CONFIG_MAX_FILENAME_LEN (256)

typedef struct
{
    char input_filename[CONFIG_MAX_FILENAME_LEN];
    int bytes_per_line;
    char array_name[CONFIG_MAX_STR_LEN];
    int array_size;
    char array_type[CONFIG_MAX_STR_LEN];
    bool array_static;
    bool array_const;
    char c_filename[CONFIG_MAX_STR_LEN];
    char h_filename[CONFIG_MAX_STR_LEN];
    bool cpp_extern;
    char header_guard[CONFIG_MAX_STR_LEN];

} config_st;

int config_read(const char* filename, config_st* cfg);

#ifdef __cplusplus
}
#endif

#endif // CONFIG_H