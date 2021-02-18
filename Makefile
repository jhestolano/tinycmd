PROJ_NAME=tinycmd
BUILD_DIR=./build


TESTS_DIR=./tests

INC_DIRS=. \
	$(TESTS_DIR) \
	$(TESTS_DIR)/Unity \

SRCS=tinycmd.c \
	st.c \
	utils.c \

TEST_SRCS=$(TESTS_DIR)/test_main.c \
	$(TESTS_DIR)/test_cmd.c \
	$(TESTS_DIR)/Unity/unity.c \
 
OBJS:=$(SRCS:%=$(BUILD_DIR)/%.o)

TEST_OBJS:=$(TEST_SRCS:%=$(BUILD_DIR)/%.o)

CC=gcc
OBJCOPY=objcopy
OBJDUMP=objdump
GDB=gdb
AR=ar

# Project Defines.
DEFS=-D__DBG__

TARGET_FLAGS=

CFLAGS=$(TARGET_FLAGS) \
	-g \
	-O0 \
	-Wall \

LFLAGS=$(TARGET_FLAGS) \

# Dependencies and object file definitions,
INCLUDE=$(addprefix -I,$(INC_DIRS))

# Use compiler flags w/ optimizations turned for libraries, as it most likely
# this will not need to be debugged. Use the same LIB_FLAGS for all libraries.
# $(HAL_OBJS): CFLAGS:=$(LIB_FLAGS)
$(BUILD_DIR)/$(PROJ_NAME): $(OBJS)
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

tests: $(TEST_OBJS) $(OBJS)
	$(CC) $(INCLUDE) $(DEFS) $(CFLAGS) $^ -o $(BUILD_DIR)/$@.out $(LFLAGS)

.PHONY: clean $(PROJ_NAME) debug tests
