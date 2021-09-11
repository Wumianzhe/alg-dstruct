##
# alg-dstruct
#
# @file
# @version 0.1

.PHONY: all clean debug release
all: debug release

PROJECT_DIR = ~/Documents/C/Uni/Sem3/alg-dstruct
BUILD_DIR = $(PROJECT_DIR)/build
SOURCE_DIR = $(PROJECT_DIR)/src
EXEC_NAME = lab
# basically a glob
SRCS := $(notdir $(wildcard $(SOURCE_DIR)/*.c))
OBJS := $(patsubst %.c, %.o, $(SRCS))

# compile with clang
CC = clang

CFLAGS =
CFLAGS_DEBUG = -Og
CGLAGS_RELEASE = -O2 -DNDEBUG -march=native

# i should have inserted comments when writing this 4 months ago. Works on my laptop though
# most of "magic" is turning INCLUDE_DIRS into list of include flags and cleaning up
# TODO add directory dependencies
debug: $(addprefix $(BUILD_DIR)/Debug/, $(OBJS))
	$(CC) $^ $(LFLAGS) -o $(BUILD_DIR)/Debug/$(EXEC_NAME)

$(BUILD_DIR)/Debug/%.o: $(SOURCE_DIR)/%.c
	$(CC) -c $(CFLAGS) $(CFLAGS_DEBUG) $< -o $@ $(addprefix -I , $(INCLUDE_DIRS))

release: $(addprefix $(BUILD_DIR)/Release/, $(OBJS))
	$(CC) $^ $(LFLAGS) -o $(BUILD_DIR)/Release/$(EXEC_NAME)

$(BUILD_DIR)/Release/%.o: $(SOURCE_DIR)/%.c
	$(CC) -c $(CFLAGS) $(CFLAGS_RELEASE) $< -o $@ $(addprefix -I , $(INCLUDE_DIRS))

clean:
	$(RM) $(addprefix $(BUILD_DIR)/Debug/, *.o $(EXEC_NAME)) $(addprefix $(BUILD_DIR)/Release/, *.o $(EXEC_NAME))

# end
