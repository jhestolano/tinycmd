#include "st.h"

static const char *_st_str_a[] = {
   "OK",
   "NULL Pointer",
   "Invalid Argument",
   "Invalid Size",
   "Too Large",
   "Too Small",
   "Out of Range",
   "Not Implemented",
   "Not Found",
   "Internal",
   "Generic",
   "Not Initialized",
   "Unknown?",
};

void st_print(char* msg, stcode_t st) {
   if (st < last_elem_e) {
      printf("%s: %s.\n\r", msg, _st_str_a[st]);
   }
   else {
      printf("%s: Unknown status.\n\r", msg);
   }
}

const char* st_tostr(stcode_t st) {
  if(st > last_elem_e) {
    st = last_elem_e;
  }
  return _st_str_a[st];
}
