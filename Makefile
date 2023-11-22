# TODO compiler..
CC=gcc

SOURCES=$(wildcard src/*.c)
SOURCES_NO_MAIN=$(filter-out src/main.c, $(SOURCES))
TEST_SOURCES=$(wildcard test/*.c)

#-Wpadded 
DEBUG_CFLAGS=-Wall -Wextra -pedantic-errors -Werror -g -O0 
DEBUG_DIR=bin/debug
DEBUG_OBJECTS=$(patsubst src/%.c, $(DEBUG_DIR)/%.o, $(SOURCES))
DEBUG_EXE=$(DEBUG_DIR)/medusa

RELEASE_CFLAGS=-Wall -Wextra -pedantic-errors -Werror -O2 -g
RELEASE_DIR=bin/release
RELEASE_OBJECTS=$(patsubst src/%.c, $(RELEASE_DIR)/%.o, $(SOURCES))
RELEASE_EXE=$(RELEASE_DIR)/medusa

GENERATOR_DIR=bin/generator
GENERATOR_OBJECTS=$(GENERATOR_DIR)/gen.o $(GENERATOR_DIR)/generate.o
GENERATOR_EXE=$(GENERATOR_DIR)/gen

TEST_CFLAGS=-Wall -Wextra -fprofile-arcs -ftest-coverage -g -O0
TEST_DIR=bin/test
TEST_OBJECTS=$(patsubst src/%.c, $(TEST_DIR)/%.o, $(SOURCES_NO_MAIN))
TEST_SRC_OBJECTS=$(patsubst test/%.c, $(TEST_DIR)/%.o, $(TEST_SOURCES))
TEST_EXE=$(TEST_DIR)/test

# Debug build
$(DEBUG_OBJECTS): $(DEBUG_DIR)/%.o : src/%.c
	$(CC) $(DEBUG_CFLAGS) -c $< -o $@

$(DEBUG_EXE): $(DEBUG_OBJECTS)
	$(CC) $(DEBUG_CFLAGS) -o $(DEBUG_EXE) $(DEBUG_OBJECTS)

# Debug build
$(RELEASE_OBJECTS): $(RELEASE_DIR)/%.o : src/%.c
	$(CC) $(RELEASE_CFLAGS) -c $< -o $@

$(RELEASE_EXE): $(RELEASE_OBJECTS)
	$(CC) $(RELEASE_CFLAGS) -o $(RELEASE_EXE) $(RELEASE_OBJECTS)

# Generator build
$(GENERATOR_DIR)/gen.o : generators/gen.c 
	$(CC) $(DEBUG_CFLAGS) -c $< -o $@
$(GENERATOR_DIR)/generate.o : src/generate.c 
	$(CC) $(DEBUG_CFLAGS) -c $< -o $@

$(GENERATOR_EXE): $(GENERATOR_OBJECTS)
	$(CC) $(DEBUG_CFLAGS) -o $(GENERATOR_EXE) $(GENERATOR_OBJECTS)

# Testing with coverage reports
$(TEST_SRC_OBJECTS): $(TEST_DIR)/%.o : test/%.c
	# Don't use coverage flags here.
	$(CC) $(DEBUG_CFLAGS) -c $< -o $@

$(TEST_DIR)/munit.o: munit/munit.c 
	# Don't use coverage flags here.
	$(CC) $(DEBUG_CFLAGS) -c munit/munit.c -o $(TEST_DIR)/munit.o

$(TEST_OBJECTS): $(TEST_DIR)/%.o : src/%.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

$(TEST_EXE): $(TEST_OBJECTS) $(TEST_SRC_OBJECTS) $(TEST_DIR)/munit.o
	$(CC) $(TEST_CFLAGS) -o $(TEST_EXE) $(TEST_OBJECTS) $(TEST_SRC_OBJECTS) $(TEST_DIR)/munit.o

# Directories for builds
$(DEBUG_DIR): 
	@mkdir -p $(DEBUG_DIR)
$(TEST_DIR): 
	@mkdir -p $(TEST_DIR)
$(RELEASE_DIR): 
	@mkdir -p $(RELEASE_DIR)
$(GENERATOR_DIR): 
	@mkdir -p $(GENERATOR_DIR)
$(PERFT_DIR): 
	@mkdir -p $(PERFT_DIR)

clean:
	rm -rf ./bin/
	rm -rf ./cov/

generator: $(GENERATOR_DIR) $(GENERATOR_EXE)

debug: $(DEBUG_DIR) $(DEBUG_EXE)

release: $(RELEASE_DIR) $(RELEASE_EXE)

perft: $(PERFT_DIR) $(PERFT_EXE)

.PHONY: test
test: $(TEST_DIR) $(TEST_EXE)
	# TODO: lcov / gnu
	# LLVM_PROFILE_FILE="cov/medusa.profraw" ./bin/test/test --log-visible info --show-stderr
	# llvm-profdata-14 merge -sparse cov/medusa.profraw -o cov/medusa.profdata
	# llvm-cov-14 show $(TEST_DIR)/test -instr-profile=cov/medusa.profdata > cov/line.report
	# llvm-cov-14 report $(TEST_DIR)/test -instr-profile=cov/medusa.profdata > cov/summary.report
	# llvm-cov-14 show $(TEST_DIR)/test -instr-profile=cov/medusa.profdata -format=html > cov/line.report.html

all: test debug
