## TinyCmd: 
Execute commands on your micrcontroller using strings.
It parses the string (the actual interface does not matter),
and extracts the command name (first token in string), it then searches for
arguments that may be contained in the string.

TinyCmd will treat the first token (separator is a blank character by default)
as the command the user wants executed, and the next tokens are treated as
arguments. It will then search for such command in the command table provided
by you at initialization. If the command name matches one from the table,
the callback is executed, and the arguments are available as typed (`int32`,
`uint8`, `int16`, etc) values inside the callback. You are responsible to
implement the callbacks.

### How it works
Imagine you are testing your microcontroller peripherals code and you may want
to update the PWM duty cycle / frequency on the fly. You pull out a terminal
to your microcontroller serial port and type:

> pwm f123 d10

Which means: set the frequency to 123Khz and duty cycle to 10%.

TinyCmd receives the string, parses it and calls back a function where you have
code to update the frequency and duty cycle of the PWM when executed:

``` C
stcode_t set_pwm_callback(arg_t* args, void* user_args) {
  /* args is a buffer that contains the parsed arguments (if any) */
  /* user_args is a pointer to whatever data structure the user may need */
  /* inside the callback function, in this case Pwm_t object. */
  Pwm_t* obj = (Pwm_t*)user_args;
  
  /* Grab the 1st argument, and cast as uint16_t. */
  Pwm_SetFrq(obj, TINYCMD_ARG(args, 0, uint16_t));  
  
  /* Grab the 2nd argument, and cast as uint8_t. */
  Pwm_SetDC(obj, TINYCMD_ARG(args, 1, uint8_t));  
  
  return ok_e;
}

```

Or maybe you just want to toggle an LED on command:

> led_toggle

``` C
stcode_t led_toggle_callback(arg_t* args, void* user_args) {

  /* Call your code that will toggle the LED */
  LED_toggle();
  
  return ok_e;
}

```

So you can see where this is going: you can connect a callback to turn a motor
on/off; change the gains, or tracking reference of your PID controller, or even
disable it, you name it.

#### The `tinycmd_t` array; AKA the Command Table:

For TinyCmd to execute your commands, first it needs to know about. The way to
do this is to define a command table somwhere in your application code (main,
for example), and initialize TinyCmd with it:

``` C

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
    
    /* This is the second command to update controller gains. */
    {
       /* Command name. */
       .name = "pid_gains",

       /* Command handle. */
       .callback = set_pid_callback,

       /* Argument definition. */
       .args = {
         {arg_i16_e, 'p'},
         {arg_i16_e, 'i'},
         {arg_i16_e, 'd'},
       },

       /* User argument. */
       .user_data = (void*)pid_obj,

       /* Help message */
       .help = "Set PID ccontroller gains"
    },
    /* New commands are defined by new entries to the table. */
};

/* TinyCmd is initialized with the command table and its size. */
tinycmd_init(command_table, TINYCMD_TABLE_SIZE(command_table));

```

As you may have already notice, the command table is just an array of tinycmd_t
structures. The structure has the following members:

* `name`: This is the string you will type to get the command executed.
* `callback`: Function that will be executed when the command name is received.
* `args`: This defines the array of arguments that the command may receive. It
contains two fields: a type field, and a name field. Note that the name field
for an argument is just *one* character.
* `user_data`: Pointer to any other data that may need to be accessed during
command execution.
* `help`: Help message that will be printed when the `?` character is received
on a command.

#### The `args` array
As said before, the `args` array holds the parameters that may have been
received for command execution. TinyCmd parses the string and converts the
text arguments to the appropriate data type, and populates the `args` array. 
When an argument is not receveied, such argument is not populated, and its
`is_valid` flag is set to `False`; this way you know if the command was
actually received before trying to use it. It would look something like this:

````C
uint8_t foo;
if (TINYCMD_ARG_IS_VALID(args, 0)) {
    foo = TINY_CMD_ARG(args, 0, uint8_t);
    do_something(foo);
} else {
    /* Error handling if the argument is not recieved. */
    some_error_handling(INVALID_ARG_ERR);
}
````

The `TINYCMD_ARG_IS_VALID` just makes it easier to grab the `is_valid` flag.
The `0` means that it is the first argument declared in the command table. The
`uint8_t` just defines the cast inside the macro.

The `args` array is of type `args_t` and this structure contains the following
members:

* `argdef_t* def`: Pointer to the `arg` entry in the table. This contains
information of the `arg` name and type.
* `uint8 data[]`: Memory buffer where the arg raw data is stored.
* `uint8_t is_valid`: Flag which gets set to `True` if the `arg` was received.

You do not need to access this fields directly. The following to macros help
you do that:

* `TINYCMD_ARG(args, index_in_table, data_type)`,
* `TINYCMD_ARG_IS_VALID(args, index_in_table)`.


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
  Uart_Puts(&uart_obj, msg);
}
```

If you'd rather not worry about this, or you do not need the "help message"
functionality, you can just disable it by setting the following macro inside
the `tinycmd.h` file.
```C
#define TINYCMD_ENABLE_HELP (0)
```
* Note: TinyCmd will still parse the help flag (?) and message correctly. It will
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

#### Supported data types

Currently only ints, signed and unsigned, from 8 to 32 bits are supported.
32 bit floating numbers will be supported in the future. The enumerations
used to define the argument data type in the command table are the following:

* `arg_u8_e`,
* `arg_i8_e`,
* `arg_u16_e`,
* `arg_i16_e`,
* `arg_u32_e`,
* `arg_i32_e`.

#### The simplest example
