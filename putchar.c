#include <stdio.h>
#include <stdint.h>

extern void _putchar(char c) {
  putchar((uint8_t)c);
}
