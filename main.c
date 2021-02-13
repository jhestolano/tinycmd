#include <stdio.h>
#include "tinycmd.h"
#include "dbg.h"

extern stcode_t _parse_str(char* str, const tinycmd_t* table, cmd_t* handle);

stcode_t dummy_handle(arg_t* args, void* usrargs) {
  DBG_DEBUG("Dummy command called!\n");
  return ok_e;
}

const cmddef_t cmd_a[] = {

  /*********************************************************************/
  {
     /* Command Name. */
     "pwmfreq",

     /* Command Handle. */
     dummy_handle,

     /* Argument Description. */
     {
        {arg_u8_e, 'r'},
        {arg_i16_e, 'q'},
        {arg_i32_e, 'f'},
     },
     /* User argument. */
     NULL,
  },
  /*********************************************************************/
  {
     /* Command Name. */
     "pid",

     /* Command Handle. */
     dummy_handle,

     /* Argument Description. */
     {
        {arg_i32_e, 'p'},
        {arg_i32_e, 'i'},
        {arg_i32_e, 'd'},
     },
     /* User argument. */
     NULL,
  },
  /*********************************************************************/
  {
     /* Command Name. */
     "ctrlmode",

     /* Command Handle. */
     dummy_handle,

     /* Argument Description. */
     {
        {arg_u8_e, 'x'},
        {arg_i16_e, 'y'},
        {arg_i32_e, 'z'},
     },
     /* User argument. */
     NULL,
  },
};

int main(void) {
  char rawstr[TINYCMD_RAW_STR_MAX_SIZE] = "pwmfreq f233 r10 q-40";
  stcode_t ret = tinycmd_init(cmd_a, sizeof(cmd_a) / sizeof(cmddef_t));
  if(ret == ok_e) {
    DBG_DEBUG("Init ok!\n");
    tinycmd_exec(rawstr);
  }
  return 0;
}
