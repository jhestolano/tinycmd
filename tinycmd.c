#include "tinycmd.h"
#include <string.h>
#include "dbg.h"
#include <stdlib.h>
#include "utils.h"

const char* TinyCmdDelimStr = " ";

static tinycmd_t _cmdtab_ps = {0};

typedef stcode_t (*strtonum_t)(const char* rawstr, void* buf);

/* Holds the conversion functions from string to ints. The function call index */
/* must match that of the enumeration, as it is used as the actual index. */
static strtonum_t _strtonum_fp[] = {
  utils_strtou8,
  utils_strtou16,
  utils_strtou32,
  utils_strtoi8,
  utils_strtoi16,
  utils_strtoi32,
  NULL,
};

stcode_t _get_arg(char *rawstr, const argdef_t* argdef_a, arg_t* arg) {
  stcode_t ret = generic_e;
  char argname = rawstr[0];
  size_t i;
  if(rawstr && argdef_a && arg) {
    ret = not_found_e;
    DBG_DEBUG("Searching param %s\n", rawstr);
    for(i = 0; (i < (TINYCMD_ARG_MAX_SIZE)) && (ret == not_found_e); i++) {
      DBG_DEBUG("Trying: %c\n", argdef_a[i].name);
      if(argname == argdef_a[i].name) {
        arg[i].def = &argdef_a[i];
        arg[i].is_valid = 1;
        DBG_DEBUG("Arg valid: %c\n", arg[i].def->name);
        if(_strtonum_fp[arg[i].def->type]) {
          ret = _strtonum_fp[arg[i].def->type](rawstr + 1, arg[i].data);
        } 
        if(ret == ok_e) {
          DBG_DEBUG("Conversion ok\n");
        } else {
          DBG_ERR("Conversion failed: %s!\n", st_tostr(ret));
        }
      }
    }
  } else {
    DBG_ERR("Null ptr catch");
    ret = null_ptr_e;
  }
  return ret;
}

stcode_t _get_cmddef(char* cmdstr, const tinycmd_t * table, const cmddef_t** cmddef) {
  stcode_t ret = generic_e;
  uint8_t i;
  size_t table_sz;
  const cmddef_t* cmd_a;
  if(cmdstr && table && table->size && table->cmdtab && cmddef) {
    cmd_a = table->cmdtab;
    table_sz = table->size;
    *cmddef = NULL;
    ret = ok_e;
    for(i = 0; i < table_sz; i++) {
      if(strcmp(cmdstr, cmd_a[i].name) == 0) {
        *cmddef = &cmd_a[i];
        break;
      }
    }
  } else {
    ret = (cmdstr && table && table->cmdtab && cmddef) ? inv_size_e : null_ptr_e;
  }
  return ret;
}

stcode_t _parse_str(char* str, const tinycmd_t* table, cmd_t* handle) {
  stcode_t ret = generic_e;
  char* ctxptr = str;
  char* tok;
  const cmddef_t* cmddef;
  if(str && table && table->size && handle) {
    memset(handle->args, 0, sizeof(arg_t) * (TINYCMD_ARG_MAX_SIZE));
    tok = strtok_r(ctxptr, " ", &ctxptr);
    ret = _get_cmddef(tok, table, &cmddef);
    if(!cmddef) {
      ret = internal_e;
      DBG_ERR("Command not found: %s\n", tok);
    } if (ret == ok_e) {
      DBG_DEBUG("Command: %s\n", tok);
      while(tok) {
        tok = strtok_r(ctxptr, " ", &ctxptr);
        _get_arg(tok, cmddef->argdef, handle->args);
      }
       
    }

  } else {
    DBG_ERR("Invalid input arguments\n");
  }
  return ret;
}

stcode_t tinycmd_init(cmddef_t* table, size_t size) {
   stcode_t ret = inv_arg_e;
  _cmdtab_ps.cmdtab = NULL;
  _cmdtab_ps.size = 0;
  if(table && size) {
    _cmdtab_ps.cmdtab = table;
    _cmdtab_ps.size = size;
    ret = ok_e;
  } else {
    ret = table ? inv_arg_e : null_ptr_e;
  }
  return ret;
}

stcode_t tinycmd_exec(const char* str) {
  return ok_e;
}

stcode_t tinycmd_loop(void) {
  return ok_e;
}

