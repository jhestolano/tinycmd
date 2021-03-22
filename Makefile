PROJ_NAME=tinycmd
BUILD_DIR=./build
LIB_PREFIX=lib

ARCH=x86_64

LIBC=./libc
LIBC_BUILD_DIR=$(LIBC)/build

TESTS_DIR=./tests

INC_DIRS=. \
	utils \
	$(TESTS_DIR) \
	$(TESTS_DIR)/Unity \
	$(LIBC)/include \
	$(LIBC)/printf \
	$(LIBC)/arch/$(ARCH)/include \

SRCS=tinycmd.c \
	st.c \
	putchar.c \
	utils/utils.c \

LIB_SRCS=libc/src/string/memcpy.c \
	libc/src/string/strcmp.c \
	libc/src/string/strtok.c \
	libc/src/stdlib/strtoul.c \
	libc/printf/printf.c \

TEST_SRCS=$(TESTS_DIR)/test_main.c \
	$(TESTS_DIR)/test_cmd.c \
	$(TESTS_DIR)/Unity/unity.c \
 
OBJS:=$(SRCS:%=$(BUILD_DIR)/%.o)
OBJS+=$(LIB_SRCS:%=$(BUILD_DIR)/%.o)

TEST_OBJS:=$(TEST_SRCS:%=$(BUILD_DIR)/%.o)

CC=gcc
OBJCOPY=objcopy
OBJDUMP=objdump
GDB=gdb
AR=ar

# Project Defines.
DEFS= \
	-D__DBG__ \
	-DUNITY_EXCLUDE_MATH_H

TARGET_FLAGS=

CFLAGS=$(TARGET_FLAGS) \
	-g \
	-O0 \
	-Wall \
	-fno-builtin \

LFLAGS=$(TARGET_FLAGS) \
	# -L$(LIBC_BUILD_DIR) \
	# -lc \

INCLUDE=$(addprefix -I,$(INC_DIRS))

$(BUILD_DIR)/$(LIB_PREFIX)$(PROJ_NAME): $(OBJS)
	$(AR) rcs $@.a $^

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(INCLUDE) $(DEFS) $(CFLAGS) $(EXTRA_FLAGS) -c -o $@ $<

$(BUILD_DIR)/%.s.o: %.s
	mkdir -p $(dir $@)
	$(CC) $(INCLUDE) $(DEFS) -c -o $@ $< $(CFLAGS) $(EXTRA_FLAGS)

clean:
	rm -rf $(BUILD_DIR)

debug: $(BUILD_DIR)/$(PROJ_NAME)
	$(GDB) $^.out

tests:DEFS+=-DUNIT_TEST
tests: $(OBJS) $(TEST_OBJS) 
	$(CC) $(INCLUDE) $(DEFS) $(CFLAGS) $^ $(LFLAGS) -o $(BUILD_DIR)/$@.out
	./$(BUILD_DIR)/$@.out

.PHONY: clean $(PROJ_NAME) debug tests
