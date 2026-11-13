# The Attractor Unit Test library
#
# This file is distributed under the MIT License. See LICENSE for details.

CC = gcc
CFLAGS = -Wall -Wextra -Wno-unused-parameter -std=c11
CXX = g++
CXXFLAGS = -Wall -Wextra -Wno-unused-parameter -std=c++17

# Warnings the header must stay free of, for users who build with strict flags
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wformat=2 \
	-Wcast-qual -Wdouble-promotion -Wundef -Wno-unused-parameter -Werror

SRC_DIR = .
TEST_DIR = tests
BIN_DIR = bin
SCRIPT_DIR = scripts

# Source files
ATTRACTOR_H = $(SRC_DIR)/attractor.h
GEN_SCRIPT = $(SCRIPT_DIR)/generate-decl.mjs
GEN_STAMP = $(BIN_DIR)/.generated
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c, $(BIN_DIR)/%.o, $(TEST_SRCS))
TEST_OBJS_CPP = $(patsubst $(TEST_DIR)/%.c, $(BIN_DIR)/%.cpp.o, $(TEST_SRCS))

# Executable name
TEST_EXECUTABLE = $(BIN_DIR)/test
TEST_EXECUTABLE_CPP = $(BIN_DIR)/test-cpp

# Golden test: a program whose assertions all fail, and its expected output
GOLDEN_SRC = $(TEST_DIR)/golden/failures.c
GOLDEN_EXPECTED = $(TEST_DIR)/golden/failures.expected
GOLDEN_EXECUTABLE = $(BIN_DIR)/failures
GOLDEN_ACTUAL = $(BIN_DIR)/failures.actual

all: $(TEST_EXECUTABLE)

# Generate attractor.h from the script
# Only regenerates if the generation script is newer than the stamp file
$(GEN_STAMP): $(GEN_SCRIPT)
	@mkdir -p $(BIN_DIR)
	node $(GEN_SCRIPT)
	@touch $(GEN_STAMP)

# Ensure the generated header exists before building
$(ATTRACTOR_H): $(GEN_STAMP)

# C compilation of test files
$(BIN_DIR)/%.o: $(TEST_DIR)/%.c $(ATTRACTOR_H)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# C++ compilation of test files
$(BIN_DIR)/%.cpp.o: $(TEST_DIR)/%.c $(ATTRACTOR_H)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -x c++ $(INCLUDES) -c $< -o $@

# Link C test executable
$(TEST_EXECUTABLE): $(TEST_OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

# Link C++ test executable
$(TEST_EXECUTABLE_CPP): $(TEST_OBJS_CPP)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

$(GOLDEN_EXECUTABLE): $(GOLDEN_SRC) $(ATTRACTOR_H)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

test: $(TEST_EXECUTABLE) test-golden
	$(TEST_EXECUTABLE)

# The failing program must exit with 1 and print exactly the expected output
test-golden: $(GOLDEN_EXECUTABLE)
	$(GOLDEN_EXECUTABLE) > $(GOLDEN_ACTUAL); test $$? -eq 1
	diff -u $(GOLDEN_EXPECTED) $(GOLDEN_ACTUAL)

test-cpp: $(TEST_EXECUTABLE_CPP)
	$(TEST_EXECUTABLE_CPP)

# Compile the header (implementation and macros) with strict warnings as errors, in C and C++
strict: $(ATTRACTOR_H)
	$(CC) -std=c11 $(STRICT_FLAGS) -Wstrict-prototypes -Wmissing-prototypes -fsyntax-only $(GOLDEN_SRC)
	$(CXX) -std=c++17 -x c++ $(STRICT_FLAGS) -fsyntax-only $(GOLDEN_SRC)

# Force regeneration
generate:
	@mkdir -p $(BIN_DIR)
	node $(GEN_SCRIPT)
	@touch $(GEN_STAMP)

clean:
	rm -f $(TEST_OBJS) $(TEST_OBJS_CPP) $(TEST_EXECUTABLE) $(TEST_EXECUTABLE_CPP) $(GOLDEN_EXECUTABLE) $(GOLDEN_ACTUAL) $(GEN_STAMP)

.PHONY: all clean generate strict test test-cpp test-golden
