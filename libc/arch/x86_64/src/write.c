#include <sys/syscall.h>
#include <stdint.h>
#include <stdlib.h>

// Syscall functions (write_ and exit_) are linux-only. They have trailing
// underscores to avoid collisions with names in the C headers, which are used
// for size_t and ssize_t.

// Everything we need to make the calls work on both 32 and 64 bit.
#ifdef __x86_64__

#define DO_SYSCALL "syscall"
#define REG1 "D"
#define REG2 "S"
#define REG3 "d"
#define reg_t uint64_t
#define clobbered "memory", "cc", "%rcx", "%r11"

#else

#define DO_SYSCALL "int $0x80"
#define REG1 "b"
#define REG2 "c"
#define REG3 "d"
#define reg_t uint32_t
#define clobbered "memory", "cc"

#endif

ssize_t write(int fd, void *buf, size_t count) {
	ssize_t ret;
	asm volatile (DO_SYSCALL
		: "=a" (ret)
		: "0" (SYS_write), REG1 ((reg_t) fd), REG2 ((reg_t) buf),
		  REG3 ((reg_t) count)
		: clobbered);
	return ret;
}
