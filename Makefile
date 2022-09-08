CC=clang

SOURCES=$(wildcard src/*.c)
SOURCES_NO_MAIN=$(filter-out src/main.c, $(SOURCES))

DEBUG_CFLAGS=-Wall
DEBUG_DIR=bin/debug
DEBUG_OBJECTS=$(patsubst src/%.c, $(DEBUG_DIR)/%.o, $(SOURCES))
DEBUG_EXE=$(DEBUG_DIR)/medusa

TEST_CFLAGS=-Wall -fprofile-instr-generate -fcoverage-mapping
TEST_DIR=bin/test
TEST_OBJECTS=$(patsubst src/%.c, $(TEST_DIR)/%.o, $(SOURCES_NO_MAIN))
TEST_EXE=$(TEST_DIR)/test

RELEASE_CFLAGS=-Wall # TODO
RELEASE_DIR=bin/release
RELEASE_EXE=$(RELEASE_DIR)/medusa # TODO

# Debug build
$(DEBUG_OBJECTS): $(DEBUG_DIR)/%.o : src/%.c
	$(CC) $(DEBUG_CFLAGS) -c $< -o $@

$(DEBUG_EXE): $(DEBUG_OBJECTS)
	$(CC) $(DEBUG_CFLAGS) -o $(DEBUG_EXE) $(DEBUG_OBJECTS)

# Testing with coverage reports
$(TEST_DIR)/main.o: test/main.c 
	$(CC) $(DEBUG_CFLAGS) -c test/main.c -o $(TEST_DIR)/main.o

$(TEST_DIR)/munit.o: munit/munit.c 
	# Don't use coverage flags here.
	$(CC) $(DEBUG_CFLAGS) -c munit/munit.c -o $(TEST_DIR)/munit.o

$(TEST_OBJECTS): $(TEST_DIR)/%.o : src/%.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

$(TEST_EXE): $(TEST_OBJECTS) $(TEST_DIR)/main.o $(TEST_DIR)/munit.o
	$(CC) $(TEST_CFLAGS) -o $(TEST_EXE) $(TEST_OBJECTS) $(TEST_DIR)/main.o $(TEST_DIR)/munit.o

# Directories for builds
$(DEBUG_DIR): 
	@mkdir -p $(DEBUG_DIR)
$(TEST_DIR): 
	@mkdir -p $(TEST_DIR)
$(RELEASE_DIR): 
	@mkdir -p $(RELEASE_DIR)

clean:
	rm -rf ./bin/
	rm -rf ./cov/

debug: $(DEBUG_DIR) $(DEBUG_EXE)

test: $(TEST_DIR) $(TEST_EXE)
	# TODO: improve coverage reporting: https://llvm.org/docs/CommandGuide/llvm-cov.html
	LLVM_PROFILE_FILE="cov/medusa.profraw" ./bin/test/test
	llvm-profdata merge -sparse cov/medusa.profraw -o cov/medusa.profdata
	llvm-cov show $(TEST_DIR)/test -instr-profile=cov/medusa.profdata > cov/line.report
	llvm-cov report $(TEST_DIR)/test -instr-profile=cov/medusa.profdata > cov/summary.report

all: test debug
