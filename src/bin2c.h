#ifndef BIN2C_H
#define BIN2C_H

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

int bin2c_write(const config_st* cfg);
int bin2c_write_buffer_c_file(const uint8_t* buffer, size_t buffer_size, const config_st* cfg);

#ifdef __cplusplus
}
#endif

#endif // BIN2C_H
