PROJ_NAME=libc
BUILD_DIR=./build
LIB_PREFIX=

ARCH=x86_64

INC_DIRS=. \
	include \
	printf \
	arch/$(ARCH)/include \
	arch/$(ARCH)/include/_types \

SRC_DIRS=src \
	printf \
	arch/$(ARCH)/src \

SRCS:=$(shell find $(SRC_DIRS) -name *.c -or -name *.s)

OBJS:=$(SRCS:%=$(BUILD_DIR)/%.o)

CC=gcc
OBJCOPY=objcopy
OBJDUMP=objdump
GDB=gdb
AR=ar

TARGET_FLAGS=

CFLAGS=$(TARGET_FLAGS) \
	-g \
	-O0 \
	-Wall \
	-H \

LFLAGS=$(TARGET_FLAGS) \
	-nostdlib \

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

.PHONY: clean $(PROJ_NAME) ARM
