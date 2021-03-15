## TinyCmd: 
Execute commands on your micrcontroller using strings.

### How it works
It works by parsing a string (the actual interface does not matter),
and extracting the command name (first token in string), and then searching for
arguments that may be contained in the string.

Example:

> pwm f123 d10

TinyCmd will treat the first token (separator is a blank character by default)
as the command the user wants executed, and the next tokens are treated as
arguments. It will then search for such command in the command table provided
by the user at initialization. If the command name matches one from the table,
the callback is executed, and the arguments are available as typed (`int32`,
`uint8`, `int16`, etc) values inside the callback. The user is responsible to
implement the callbacks:

``` C

stcode_t set_pwm_callback(arg_t* args, void* user_args) {
  /* Grab the 1st argument, and cast as uint16_t. */
  Pwm_SetFrq(TINYCMD_ARG(args, 0, uint16_t));  
  
  /* Grab the 2nd argument, and cast as uint8_t. */
  Pwm_SetDC(TINYCMD_ARG(args, 1, uint8_t));  
  return ok_e;
}


```

### Setup

TinyCmd is contained in a few source files: the actual code and some utilities.
A Makefile is provided to generate a `tinycmd.a` library file that can be
linked to your application; then all you have to do is to include `tinycmd.h`
in your source code.
