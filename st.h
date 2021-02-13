#ifndef ST_H
#define ST_H

#include <stdio.h>

typedef enum stcode_e {
  ok_e = 0,
  null_ptr_e,
  inv_arg_e,
  inv_size_e,
  too_large_e,
  too_small_e,
  out_of_range_e,
  not_implemented_e,
  not_found_e,
  internal_e,
  generic_e,
  not_initialized_e,
  last_elem_e,
} stcode_t;

void st_print(char* msg, stcode_t st);

const char* st_tostr(stcode_t st);

#endif
