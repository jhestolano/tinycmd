## TinyCmd: 
Execute commands on your micrcontroller using strings.

### How it works
It works by parsing a string (the actual interface does not matter),
and extracting the command name (first token in string), and then searching for
arguments that may be contained in the string.

#### Example:

> pwm f123 d10

TinyCmd will treat the first token (separator is a blank character by default)
as the command the user wants executed, and the next tokens are treated as
arguments. It will then search for such command in the command table provided
by the user at initialization. If the command name matches one from the table,
the callback is executed, and the arguments are available as typed (`int32`,
`uint8`, `int16`, etc) values inside the callback. The user is responsible to
implement the callbacks, that will roughly look something like this:

``` C
stcode_t set_pwm_callback(arg_t* args, void* user_args) {
  /* args is a buffer that contains the parsed arguments (if any) */
  /* user_args is a pointer to whatever data structure the user may need */
  /* inside the callback function, in this case Pwm_t object. */
  Pwm_t* obj = (Pwm_t)user_args;
  
  /* Grab the 1st argument, and cast as uint16_t. */
  Pwm_SetFrq(obj, TINYCMD_ARG(args, 0, uint16_t));  
  
  /* Grab the 2nd argument, and cast as uint8_t. */
  Pwm_SetDC(obj, TINYCMD_ARG(args, 1, uint8_t));  
  
  return ok_e;
}

```

#### Command Definition:

For this command to get executed, first TinyCmd needs to know about it. The
way this is done is defining a command table somewhere in your application code
(it could be main). Example:

``` C
#include <stdio.h>
#include "tinycmd.h"

/* Defined somewhere else. */
extern stcode_t set_pwm_callback(args_t* args, void* user_args);

extern Pwm_t* obj;

int main(void) {

  const tinycmd_t command_table[] = 
  {
    {
       /* Command name. */
       .name = "pwmfreq",

       /* Command handle. */
       .callback = set_pwm_callback,

       /* Argument definition. */
       .args = {
         {arg_u16_e, 'f'},
         {arg_u8_e,  'd'},
       },

       /* User argument. */
       .user_data = (void*)obj,

       /* Help message */
       .help = "Updates PWM (f)requency, and (d)uty-cycle."
    },
  };

  /* This is a string example asking TinyCmd to show the help message for the */
  /* pwmfreq command. Usually this string would be received via Uart; something */
  /* alone the lines of: Uart_ReadLine(&uart_obj, raw_command, sizeof(raw_command)); */
  const char raw_command[] = "pwmfreq ?";

  /* TinyCmd is initialized with the command table and its size. */
  tinycmd_init(command_table, TINYCMD_TABLE_SIZE(command_table));

  /* Ask TinyCmd to parse the raw string received and check if we can do a callback */
  /* on it. */
  tinycmd_exec(raw_command);
}

```

Executing the above code either on target or on your laptop would print the
following message back to the user:

```
********** TINYCMD HELP MSG **********

Updates PWM (f)requency, and (d)uty-cycle.

**************************************
```
#### Print Function

As you probably have already noticed, TinyCmd has the ability to print messages
back to you. For this to be possible, you need to provide TinyCmd the ability
to do so with the `tinycmd_printf` function implementation. The reason for this
is to make the library flexible enough to print back to you no matter what
the interface you are using. If you are just trying this library on your laptop
a possible implementation looks like this:

``` C
#include <stdio.h>

void tinycmd_printf(const char* msg) {
  printf("%s\n\r", msg);
}

```

So essentially, the `tinycmd_printf` function is just a wrapper around the
interface you are using to print information to a sreen. An implementation
around a UART interface would look like this:

``` C
#include "uart.h"

void tinycmd_printf(const char* msg) {
  Uart_Puts(msg);
}
```

If you'd rather not worry about this, or you do not need the "help message"
functionality, you can just disable it by setting the following macro inside
the `tinycmd.h` file.
```C
#define TINYCMD_ENABLE_HELP (0)
```
* Note: TinyCmd will still parse the help flag and message correctly. It will
just skip the call to the `tinycmd_printf` function.

#### Build

TinyCmd is contained in a few source files: the actual code and some utilities.
A Makefile is provided to compile and generate `libtinycmd.a` library file that
can be linked to your application; then all you have to do is to include
`tinycmd.h` in your source code.

To do this, you would clone the `tinycmd` repo into your project structure. Here
is an example with a simple Hello World type project (single main.c and Makefile):

```
.
├── main.c <--- Your application.
├── Makefile <--- Your Makefile.
└── tinycmd
    ├── dbg.h
    ├── Makefile <--- tinycmd Makefile. Just run `make` inside this folder.
    ├── README.md
    ├── staticdef.h
    ├── st.c
    ├── st.h
    ├── string <--- String Utils.
    │   ├── memcpy.c
    │   ├── memset.c
    │   ├── strcmp.c
    │   └── strtok.c
    ├── tests <--- Tests and testing framework (Unity).
    │   ├── test_cmd.c
    │   ├── test_integration.c
    │   ├── test_main.c
    │   └── Unity
    │       ├── unity.c
    │       ├── unity.h
    │       └── unity_internals.h
    ├── tinycmd.c <--- Actual code.
    ├── tinycmd.h
    └── utils <--- Other Utils.
        ├── utils.c
        └── utils.h
```

To clone and build from your application project folder, run the following
command:

```
cd path_to_your_project
git clone https://github.com/pickman22/tinycmd
cd tinycmd && make && cd ..
```

A build folder will be created inside the tinycmd folder. The output file that
will be linked by your application is called `libtinycmd.a`.

All you have to do now is compile your code and link against the tinycmd lib
file. Here's a simple build command that shows how to do it:

```
gcc main.c -Itinycmd -Ltinycmd/build/ -ltinycmd -o tinycmd-example.out
```


