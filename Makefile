# TODO compiler..
CC=gcc

SOURCES=$(wildcard src/*.c)
MEDUSA_SOURCE=medusa/main.c
TEST_SOURCES=$(wildcard test/*.c)
PERFT_SOURCE=perft/main.c

#-Wpadded 
DEBUG_CFLAGS=-Wall -Wextra -pedantic-errors -Werror -g -O0 
DEBUG_DIR=bin/debug
DEBUG_OBJECTS=$(patsubst src/%.c, $(DEBUG_DIR)/%.o, $(SOURCES))
DEBUG_MAIN_OBJECT=$(DEBUG_DIR)/main.o
DEBUG_EXE=$(DEBUG_DIR)/medusa

RELEASE_CFLAGS=-Wall -Wextra -pedantic-errors -Werror -O2 -g
RELEASE_DIR=bin/release
RELEASE_OBJECTS=$(patsubst src/%.c, $(RELEASE_DIR)/%.o, $(SOURCES))
RELEASE_MAIN_OBJECT=$(RELEASE_DIR)/main.o
RELEASE_EXE=$(RELEASE_DIR)/medusa

PERFT_DIR=bin/perft
PERFT_MAIN_OBJECT=$(PERFT_DIR)/main.o
PERFT_EXE=$(PERFT_DIR)/perft

GENERATOR_DIR=bin/generator
GENERATOR_OBJECTS=$(GENERATOR_DIR)/gen.o $(GENERATOR_DIR)/generate.o
GENERATOR_EXE=$(GENERATOR_DIR)/gen

TEST_CFLAGS=-Wall -Wextra -fprofile-arcs -ftest-coverage -g -O0
TEST_DIR=bin/test
TEST_OBJECTS=$(patsubst src/%.c, $(TEST_DIR)/%.o, $(SOURCES))
TEST_SRC_OBJECTS=$(patsubst test/%.c, $(TEST_DIR)/%.o, $(TEST_SOURCES))
TEST_EXE=$(TEST_DIR)/test

# Debug build
$(DEBUG_MAIN_OBJECT): $(MEDUSA_SOURCE)
	$(CC) $(DEBUG_CFLAGS) -c $(MEDUSA_SOURCE) -o $(DEBUG_MAIN_OBJECT)

$(DEBUG_OBJECTS): $(DEBUG_DIR)/%.o : src/%.c
	$(CC) $(DEBUG_CFLAGS) -c $< -o $@

$(DEBUG_EXE): $(DEBUG_OBJECTS) $(DEBUG_MAIN_OBJECT)
	$(CC) $(DEBUG_CFLAGS) -o $(DEBUG_EXE) $(DEBUG_OBJECTS) $(DEBUG_MAIN_OBJECT)

# Release build
$(RELEASE_MAIN_OBJECT): $(MEDUSA_SOURCE)
	$(CC) $(RELEASE_CFLAGS) -c $(MEDUSA_SOURCE) -o $(RELEASE_MAIN_OBJECT)

$(RELEASE_OBJECTS): $(RELEASE_DIR)/%.o : src/%.c
	$(CC) $(RELEASE_CFLAGS) -c $< -o $@

$(RELEASE_EXE): $(RELEASE_OBJECTS) $(RELEASE_MAIN_OBJECT)
	$(CC) $(RELEASE_CFLAGS) -o $(RELEASE_EXE) $(RELEASE_OBJECTS) $(RELEASE_MAIN_OBJECT)
	
# Perft build
$(PERFT_MAIN_OBJECT): $(PERFT_SOURCE)
	$(CC) $(RELEASE_CFLAGS) -c $(PERFT_SOURCE) -o $(PERFT_MAIN_OBJECT)

$(PERFT_EXE): $(RELEASE_OBJECTS) $(PERFT_MAIN_OBJECT)
	$(CC) $(RELEASE_CFLAGS) -o $(PERFT_EXE) $(RELEASE_OBJECTS) $(PERFT_MAIN_OBJECT)

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
	rm -f report.info

.PHONY: generator
generator: $(GENERATOR_DIR) $(GENERATOR_EXE)

.PHONY: debug
debug: $(DEBUG_DIR) $(DEBUG_EXE)

.PHONY: release
release: $(RELEASE_DIR) $(RELEASE_EXE)

.PHONY: perft
perft: $(PERFT_DIR) $(RELEASE_DIR) $(PERFT_EXE)

.PHONY: test
test: $(TEST_DIR) $(TEST_EXE)
	# run tests
	./bin/test/test --log-visible info --show-stderr
	# generate coverage report
	lcov --capture --directory bin/test --output-file report.info
	# generate html report, open with: make cov-browse
	genhtml report.info --output-directory cov

.PHONY: cov-browse
cov-browse:
	xdg-open cov/index.html

.PHONY: bench
bench: perft
	# TODO perf stat ..?
	./bin/perft/perft 7 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
	

all: test debug
