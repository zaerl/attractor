# This is the Makefile used to generate the attractor.h and attractor.c
# files and run tests. Please ignore it.
CC = gcc
CFLAGS = -Wall -Wextra -Wno-unused-parameter -std=c11
CXX = g++
CXXFLAGS = -Wall -Wextra -Wno-unused-parameter -std=c++17

SRC_DIR = .
TEST_DIR = tests
BIN_DIR = bin
SCRIPT_DIR = scripts

# Source files
ATTRACTOR_H = $(SRC_DIR)/attractor.h
ATTRACTOR_C = $(SRC_DIR)/attractor.c
ATTRACTOR_O = $(BIN_DIR)/attractor.o
ATTRACTOR_O_CPP = $(BIN_DIR)/attractor.cpp.o
GEN_SCRIPT = $(SCRIPT_DIR)/generate-decl.mjs
GEN_STAMP = $(BIN_DIR)/.generated
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c, $(BIN_DIR)/%.o, $(TEST_SRCS))
TEST_OBJS_CPP = $(patsubst $(TEST_DIR)/%.c, $(BIN_DIR)/%.cpp.o, $(TEST_SRCS))

# Executable name
TEST_EXECUTABLE = $(BIN_DIR)/test
TEST_EXECUTABLE_CPP = $(BIN_DIR)/test-cpp

all: $(TEST_EXECUTABLE)

# Generate attractor.h and attractor.c from the script
# Only regenerates if the generation script is newer than the stamp file
$(GEN_STAMP): $(GEN_SCRIPT)
	@mkdir -p $(BIN_DIR)
	node $(GEN_SCRIPT) && node $(GEN_SCRIPT) c
	@touch $(GEN_STAMP)

# Ensure generated files exist before building
$(ATTRACTOR_H) $(ATTRACTOR_C): $(GEN_STAMP)

# C compilation of attractor library
$(ATTRACTOR_O): $(ATTRACTOR_C) $(ATTRACTOR_H)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# C++ compilation of attractor library
$(ATTRACTOR_O_CPP): $(ATTRACTOR_C) $(ATTRACTOR_H)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -x c++ $(INCLUDES) -c $< -o $@

# C compilation of test files
$(BIN_DIR)/%.o: $(TEST_DIR)/%.c $(ATTRACTOR_H)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# C++ compilation of test files
$(BIN_DIR)/%.cpp.o: $(TEST_DIR)/%.c $(ATTRACTOR_H)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -x c++ $(INCLUDES) -c $< -o $@

# Link C test executable
$(TEST_EXECUTABLE): $(ATTRACTOR_O) $(TEST_OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

# Link C++ test executable
$(TEST_EXECUTABLE_CPP): $(ATTRACTOR_O_CPP) $(TEST_OBJS_CPP)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

test: $(TEST_EXECUTABLE)
	$(TEST_EXECUTABLE)

test-cpp: $(TEST_EXECUTABLE_CPP)
	$(TEST_EXECUTABLE_CPP)

# Force regeneration
generate:
	@mkdir -p $(BIN_DIR)
	node $(GEN_SCRIPT) && node $(GEN_SCRIPT) c
	@touch $(GEN_STAMP)

clean:
	rm -f $(ATTRACTOR_O) $(ATTRACTOR_O_CPP) $(TEST_OBJS) $(TEST_OBJS_CPP) $(TEST_EXECUTABLE) $(TEST_EXECUTABLE_CPP) $(GEN_STAMP)

.PHONY: all clean generate test test-cpp
