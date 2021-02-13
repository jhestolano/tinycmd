#ifndef __TINYCMD_H_
#define __TINYCMD_H_
#include <stdint.h>
#include <stddef.h>
#include "st.h"

/* Max. number of raw data per argument. */
#define TINYCMD_ARG_BYTES_MAX_SIZE (sizeof(uint32_t))

/* Max. length for a command name. */
#define TINYCMD_NAME_MAX_SIZE (16)

/* Max. number of arguments a comand may have. */
#define TINYCMD_ARG_MAX_SIZE (4)

/* Max. string length for a raw command. */
#define TINYCMD_RAW_STR_MAX_SIZE (64)

typedef enum argtype_e {
  arg_u8_e = 0,
  arg_u16_e,
  arg_u32_e,
  arg_i8_e,
  arg_i16_e,
  arg_i32_e,
  arg_str_e,
  arg_none_type_e = 254,
  arg_inv_type_e = 255,
} argtype_t;

typedef struct argdef_s {
  argtype_t type;
  char name;
} argdef_t;

typedef struct arg_s {
  const argdef_t* def;
  uint8_t data[TINYCMD_ARG_BYTES_MAX_SIZE];
  /* tinycmd_base_t data; */
  uint8_t is_valid;
} arg_t;

typedef enum stcode_e (*tinycmd_cb_t)(arg_t* arg, void* usrdata);

typedef struct cmddef_s {
  const char name[TINYCMD_NAME_MAX_SIZE];
  const tinycmd_cb_t callback;
  const argdef_t argdef[TINYCMD_ARG_MAX_SIZE];
  void* usrdata;
} cmddef_t;

typedef struct cmd_s {
  tinycmd_cb_t callback;
  arg_t args[TINYCMD_ARG_MAX_SIZE];
  void* usrdata;
} cmd_t;

typedef struct tinycmd_s {
  const cmddef_t* cmdtab;
  size_t size;
} tinycmd_t;

stcode_t tinycmd_init(const cmddef_t* table, size_t size);

stcode_t tinycmd_exec(char* str);

stcode_t tinycmd_loop(void);

#endif //__TINYCMD_H_
