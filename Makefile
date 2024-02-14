# This is the Makefile used to generate the attractor.h and attractor.c
# files and run tests. Please ignore it.
CC = gcc
CFLAGS = -Wall -Wextra -Wno-unused-parameter -std=c11 -DATT_USE_IOCTL=1

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

all: $(TEST_EXECUTABLE)

$(ATTRACTOR_H):
	node scripts/generate-decl.js > $(ATTRACTOR_H)

$(ATTRACTOR_C): $(ATTRACTOR_H)
	node scripts/generate-decl.js c > $(ATTRACTOR_C)

$(BIN_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(TEST_EXECUTABLE): $(ATTRACTOR_C) $(TEST_OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

test: $(TEST_EXECUTABLE)
	$(TEST_EXECUTABLE)

clean:
	rm -f $(ATTRACTOR_H) $(ATTRACTOR_C) $(TEST_OBJS) $(TEST_EXECUTABLE)

.PHONY: all clean
