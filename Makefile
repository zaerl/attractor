# This is the Makefile used to generate the attractor.h and attractor.c
# files and run tests. Please ignore it.
CC = gcc
CFLAGS = -Wall -Wextra -Wno-unused-parameter -std=c11

SRC_DIR = .
TEST_DIR = tests
BIN_DIR = bin

# Source files
ATTRACTOR_H = $(SRC_DIR)/attractor.h
ATTRACTOR_C = $(SRC_DIR)/attractor.c
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c, $(BIN_DIR)/%.o, $(TEST_SRCS))

# Executable name
TEST_EXECUTABLE = $(BIN_DIR)/test

all: generate $(TEST_EXECUTABLE)

generate:
	node scripts/generate-decl.mjs && node scripts/generate-decl.mjs c

$(BIN_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(TEST_EXECUTABLE): $(ATTRACTOR_C) $(TEST_OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

test: $(TEST_EXECUTABLE)
	$(TEST_EXECUTABLE)

clean:
	rm -f $(TEST_OBJS) $(TEST_EXECUTABLE)

.PHONY: all clean generate test
