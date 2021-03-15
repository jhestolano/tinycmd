#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdint.h>
#include "st.h"

#define min(x, y) ((x) < (y)) ? (x) : (y)
#define max(x, y) ((x) > (y)) ? (x) : (y)

stcode_t utils_to_bytes(uint8_t* buf, size_t bufsz, void* data, size_t datasz); 

stcode_t utils_from_bytes(uint8_t* buf, size_t bufsz, void* data, size_t datasz); 

stcode_t utils_strtou8(const char* rawstr, void* buf);
stcode_t utils_strtou16(const char* rawstr, void* buf);
stcode_t utils_strtou32(const char* rawstr, void* buf);
stcode_t utils_strtoi8(const char* rawstr, void* buf);
stcode_t utils_strtoi16(const char* rawstr, void* buf);
stcode_t utils_strtoi32(const char* rawstr, void* buf);

#endif
