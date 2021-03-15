#include "utils.h"
#include "st.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define _BUFOP_TO   0
#define _BUFOP_FROM 1

static stcode_t _bufop(uint8_t* buf, size_t bufsz, void* data, size_t datasz, uint8_t toorfrom) {
  stcode_t ret = generic_e;
  assert(buf && data && datasz && bufsz);
  assert(bufsz >= datasz);
  if(buf && data && datasz && bufsz && (bufsz >= datasz)) {
    if(toorfrom == _BUFOP_TO) {
      /* Handle storage into buffer. */
      memset(buf, 0, bufsz);
      memcpy(buf, data, datasz);
      ret = ok_e;
    } else if (toorfrom == _BUFOP_FROM) {
      /* Handle data restore from buffer. */
      memcpy(data, buf, datasz);
      ret = ok_e;
    } else {
      ret = internal_e;
    }
  } else {
    if(!buf || !data) {
      ret = null_ptr_e;
    } else {
      ret = inv_size_e;
    }
  }
  return ret;
}

stcode_t utils_to_bytes(uint8_t* buf, size_t bufsz, void* data, size_t datasz) {
  assert(buf && data);
  assert(bufsz && datasz);
  assert(bufsz >= datasz);
  return _bufop(buf, bufsz, data, datasz, _BUFOP_TO);
}

stcode_t utils_from_bytes(uint8_t* buf, size_t bufsz, void* data, size_t datasz) {
  assert(buf && data);
  assert(bufsz && datasz);
  assert(bufsz >= datasz);
  return _bufop(buf, bufsz, data, datasz, _BUFOP_FROM);
}

stcode_t utils_strtoi32(const char* rawstr, void* buf) {
  stcode_t ret = generic_e;
  int32_t tmp = 0;
  char* ptr;
  if(rawstr && buf) {
    tmp = strtol(rawstr, &ptr, 10);
    if(!ptr) {
      /* Complete error checking requires testing errno against a zero value, */
      /* but checking the contex pointer ptr will do for now. */
      ret = inv_arg_e;
    } else {
      ret = ok_e;
    }
  } else {
    ret = null_ptr_e;
  }
  if(ret == ok_e) {
    ret = utils_to_bytes(buf, sizeof(int32_t), &tmp, sizeof(int32_t));
  }
  return ret;
}

stcode_t utils_strtou32(const char* rawstr, void* buf) {
  stcode_t ret = generic_e;
  uint32_t tmp = 0;
  char* ptr;
  if(rawstr && buf) {
    tmp = strtoul(rawstr, &ptr, 10);
    if(!ptr) {
      /* Complete error checking requires testing errno against a zero value, */
      /* but checking the contex pointer ptr will do for now. */
      ret = inv_arg_e;
    } else {
      ret = ok_e;
    }
  } else {
    ret = null_ptr_e;
  }
  if(ret == ok_e) {
    ret = utils_to_bytes(buf, sizeof(uint32_t), &tmp, sizeof(uint32_t));
  }
  return ret;
}

stcode_t utils_strtou8(const char* rawstr, void* buf) {
  int32_t tmp;
  stcode_t ret = utils_strtou32(rawstr, &tmp);
  if((ret == ok_e) && (tmp >= 0) && (tmp <= UINT8_MAX)) {
    ret = utils_to_bytes(buf, sizeof(uint8_t), &tmp, sizeof(uint8_t));
  } else {
    ret = (ret != ok_e) ? out_of_range_e : ret;
  }
  return ret;
}

stcode_t utils_strtou16(const char* rawstr, void* buf) {
  int32_t tmp;
  stcode_t ret = utils_strtou32(rawstr, &tmp);
  if((ret == ok_e) && (tmp >= 0) && (tmp <= UINT16_MAX)) {
    ret = utils_to_bytes(buf, sizeof(uint16_t), &tmp, sizeof(uint16_t));
  } else {
    ret = (ret != ok_e) ? out_of_range_e : ret;
  }
  return ret;
}

stcode_t utils_strtoi8(const char* rawstr, void* buf) {
  int32_t tmp;
  stcode_t ret = utils_strtoi32(rawstr, &tmp);
  if((ret == ok_e) && (tmp >= INT8_MIN) && (tmp <= INT8_MAX)) {
    ret = utils_to_bytes(buf, sizeof(int8_t), &tmp, sizeof(int8_t));
  } else {
    ret = (ret != ok_e) ? out_of_range_e : ret;
  }
  return ret;
}

stcode_t utils_strtoi16(const char* rawstr, void* buf) {
  int32_t tmp;
  stcode_t ret = utils_strtoi32(rawstr, &tmp);
  if((ret == ok_e) && (tmp >= INT16_MIN) && (tmp <= INT16_MAX)) {
    ret = utils_to_bytes(buf, sizeof(int16_t), &tmp, sizeof(int16_t));
  } else {
    ret = (ret != ok_e) ? out_of_range_e : ret;
  }
  return ret;
}

stcode_t utils_strlow(char* str) {
  stcode_t ret = null_ptr_e;
  size_t i = 0;
  if(str) {
    while(str[i] != (char)0) {
      str[i] = tolower(str[i]);
      i++;
    }
    ret = ok_e;
  }
  return ret;
}
