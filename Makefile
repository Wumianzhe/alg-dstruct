##
# alg-dstruct
#
# @file
# @version 0.1

.PHONY: all clean debug release test
all: debug release

PROJECT_DIR = ~/Documents/C/alg-dstruct
LAB = 3-Kopnov-A18
BUILD_DIR = $(PROJECT_DIR)/build
SOURCE_DIR = $(PROJECT_DIR)/$(LAB)/src
TESTS_DIR = $(PROJECT_DIR)/$(LAB)/tests
BIN_DIR = $(PROJECT_DIR)/bin

# basically a glob
SRCS := $(notdir $(wildcard $(SOURCE_DIR)/*.c))
TESTS := $(notdir $(wildcard $(TESTS_DIR)/*.c))

INCLUDE_DIRS = $(SOURCE_DIR)
INC_FLAGS = $(addprefix -I , $(INCLUDE_DIRS))

# multiple sets of object files
OBJS := $(patsubst %.c, %.o, $(SRCS))
DEBUG_OBJS := $(addprefix $(BUILD_DIR)/Debug/, $(OBJS))
RELEASE_OBJS := $(addprefix $(BUILD_DIR)/Release/, $(OBJS))
TEST_OBJS := $(filter-out $(BUILD_DIR)/Debug/main.o, $(DEBUG_OBJS))
TESTBINS := $(addprefix $(BIN_DIR)/tests/, $(basename $(TESTS)))

# compile with clang
CC = clang

CFLAGS =
CFLAGS_DEBUG = -Og -g
CGLAGS_RELEASE = -O2 -DNDEBUG -march=native

debug: $(DEBUG_OBJS) | $(BIN_DIR)
	$(CC) $^ $(LFLAGS) -o $(BIN_DIR)/lab

$(BUILD_DIR)/Debug/%.o: $(SOURCE_DIR)/%.c | $(BUILD_DIR)/Debug
	$(CC) -c $(CFLAGS) $(CFLAGS_DEBUG) $< -o $@ $(INC_FLAGS)

release: $(RELEASE_OBJS) | $(BIN_DIR)
	$(CC) $^ $(LFLAGS) -o $(BIN_DIR)/lab-release

$(BUILD_DIR)/Release/%.o: $(SOURCE_DIR)/%.c | $(BUILD_DIR)/Release
	$(CC) -c $(CFLAGS) $(CFLAGS_RELEASE) $< -o $@ $(INC_FLAGS)

test: debug $(TESTBINS)
	for test in $(TESTBINS) ; do $$test ; done

$(BIN_DIR)/tests/%: $(TESTS_DIR)/%.c | $(BIN_DIR)/tests
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) $< $(TEST_OBJS) -o $@ $(INC_FLAGS) -lcriterion

$(BIN_DIR):
	mkdir -p $@
$(BIN_DIR)/tests:
	mkdir -p $@
$(BUILD_DIR):
	mkdir -p $@
$(BUILD_DIR)/Debug: | $(BUILD_DIR)
	mkdir -p $@
$(BUILD_DIR)/Release: | $(BUILD_DIR)
	mkdir -p $@

clean:
	$(RM) -r $(BUILD_DIR) $(BIN_DIR)

# end
