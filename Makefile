PROJ_NAME=tinycmd
BUILD_DIR=./build


INC_DIRS+=.

SRCS=main.c \
		 tinycmd.c \
		 st.c \
		 utils.c \
 
OBJS:=$(SRCS:%=$(BUILD_DIR)/%.o)

CC=gcc
OBJCOPY=objcopy
OBJDUMP=objdump
GDB=gdb

# Project Defines.
DEFS=-D__DBG__

TARGET_FLAGS=

CFLAGS=$(TARGET_FLAGS) \
	-g \
	-Os \
	-Wall \

LINKER_FILE=./linker/stm32f30_flash.ld

LFLAGS=$(TARGET_FLAGS) \

# Dependencies and object file definitions,
INCLUDE=$(addprefix -I,$(INC_DIRS))

# Use compiler flags w/ optimizations turned for libraries, as it most likely
# this will not need to be debugged. Use the same LIB_FLAGS for all libraries.
# $(HAL_OBJS): CFLAGS:=$(LIB_FLAGS)
$(BUILD_DIR)/$(PROJ_NAME): $(OBJS)
	$(CC) $(INCLUDE) $(DEFS) $(CFLAGS) $(OBJS) -o $@.out $(LFLAGS)

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

.PHONY: clean $(PROJ_NAME) debug
