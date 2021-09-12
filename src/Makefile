##
# alg-dstruct
#
# @file
# @version 0.1

.PHONY: all clean debug release test
all: debug release

PROJECT_DIR = ~/Documents/C/Uni/Sem3/alg-dstruct
BUILD_DIR = $(PROJECT_DIR)/build
SOURCE_DIR = $(PROJECT_DIR)/src
TESTS_DIR = $(PROJECT_DIR)/tests
BIN_DIR = $(PROJECT_DIR)/bin

# basically a glob
SRCS := $(notdir $(wildcard $(SOURCE_DIR)/*.c))
TESTS := $(notdir $(wildcard $(TESTS_DIR)/*.c))

# multiple sets of object files
OBJS := $(patsubst %.c, %.o, $(SRCS))
DEBUG_OBJS := $(addprefix $(BUILD_DIR)/Debug/, $(OBJS))
RELEASE_OBJS := $(addprefix $(BUILD_DIR)/Release/, $(OBJS))
TESTBINS := $(addprefix $(BIN_DIR)/tests/, $(basename $(TESTS)))

# compile with clang
CC = clang

CFLAGS =
CFLAGS_DEBUG = -Og
CGLAGS_RELEASE = -O2 -DNDEBUG -march=native

debug: $(DEBUG_OBJS) | $(BIN_DIR)
	$(CC) $^ $(LFLAGS) -o $(BIN_DIR)/lab

$(BUILD_DIR)/Debug/%.o: $(SOURCE_DIR)/%.c | $(BUILD_DIR)/Debug
	$(CC) -c $(CFLAGS) $(CFLAGS_DEBUG) $< -o $@ $(addprefix -I , $(INCLUDE_DIRS))

release: $(RELEASE_OBJS) | $(BIN_DIR)
	$(CC) $^ $(LFLAGS) -o $(BIN_DIR)/lab-release

$(BUILD_DIR)/Release/%.o: $(SOURCE_DIR)/%.c | $(BUILD_DIR)/Release
	$(CC) -c $(CFLAGS) $(CFLAGS_RELEASE) $< -o $@ $(addprefix -I , $(INCLUDE_DIRS))

test: debug $(TESTBINS)
	for test in $(TESTBINS) ; do $$test ; done

$(BIN_DIR)/tests/%: $(TESTS_DIR)/%.c | $(BIN_DIR)/tests
	$(CC) $(CFLAGS) $< $(filter-out $(BUILD_DIR)/Debug/main.o, $(DEBUG_OBJS)) -o $@

$(BIN_DIR):
	mkdir -p $@
$(BIN_DIR)/tests:
	mkdir -p $@
$(BUILD_DIR)/Debug:
	mkdir -p $@
$(BUILD_DIR)/Release:
	mkdir -p $@

clean:
	$(RM) $(addprefix $(BUILD_DIR)/Debug/, *.o $(EXEC_NAME)) $(addprefix $(BUILD_DIR)/Release/, *.o $(EXEC_NAME))

# end
