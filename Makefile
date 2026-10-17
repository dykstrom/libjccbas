# Makefile for libjccbas - JCC BASIC standard library
#
# Builds the static library bin/libjccbas.a and its tests with Clang on
# macOS, Linux, and Windows (MSYS2). Clang targets the host platform and
# architecture; there is no cross-compilation.

# Compiler and tools. We use LLVM Clang by default on all platforms.
# Override from the command line (e.g., `make CC=clang-21`) or environment.
#
# The `origin` check is required because make pre-defines CC=cc and AR=ar as
# built-in implicit variables; a plain `CC ?= clang` would be a no-op.
ifeq ($(origin CC),default)
    CC := clang
endif
ifeq ($(origin AR),default)
    AR := ar
endif

# Directories
MAIN_INC := main/inc
MAIN_SRC := main/src
TEST_INC := test/inc
TEST_SRC := test/src
BIN := bin
OBJ := $(BIN)/obj
TEST_OBJ := $(OBJ)/test

# Output
LIB := $(BIN)/libjccbas.a

# Compiler flags. -MMD -MP emits .d files alongside each .o as a side-effect
# of compilation, so header dependencies are tracked without a header list.
CFLAGS := -g -Wall -MMD -MP
ARFLAGS := rcs
LDLIBS :=

# Platform specifics. Executables get .exe on Windows (MSYS2/MinGW or native
# OS=Windows_NT), and the tests need libm on Linux.
EXE :=
UNAME_S := $(shell uname -s)
ifneq (,$(findstring MINGW,$(UNAME_S)))
    EXE := .exe
endif
ifeq ($(OS),Windows_NT)
    EXE := .exe
endif
ifeq ($(UNAME_S),Linux)
    LDLIBS += -lm
endif

# Source files. Every .c file in main/src goes into the library, and every
# .c file in test/src becomes one test executable.
MAIN_SOURCES := $(wildcard $(MAIN_SRC)/*.c)
MAIN_OBJECTS := $(patsubst $(MAIN_SRC)/%.c,$(OBJ)/%.o,$(MAIN_SOURCES))
TEST_SOURCES := $(wildcard $(TEST_SRC)/*.c)
TEST_OBJECTS := $(patsubst $(TEST_SRC)/%.c,$(TEST_OBJ)/%.o,$(TEST_SOURCES))
TEST_BINARIES := $(patsubst $(TEST_SRC)/%.c,$(BIN)/%$(EXE),$(TEST_SOURCES))

.PHONY: all test clean help

.PRECIOUS: $(OBJ)/%.o $(TEST_OBJ)/%.o

# Default target: the library and all test executables
all: $(LIB) $(TEST_BINARIES)

help:
	@echo "libjccbas - JCC BASIC standard library"
	@echo ""
	@echo "Available targets:"
	@echo "  all    - Build the library and the tests (default)"
	@echo "  test   - Build and run all tests"
	@echo "  clean  - Remove all build artifacts"
	@echo "  help   - Show this help message"

$(LIB): $(MAIN_OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

$(OBJ)/%.o: $(MAIN_SRC)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(MAIN_INC) -c $< -o $@

$(TEST_OBJ)/%.o: $(TEST_SRC)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(MAIN_INC) -I$(TEST_INC) -c $< -o $@

$(BIN)/%$(EXE): $(TEST_OBJ)/%.o $(LIB)
	$(CC) $< $(LIB) -o $@ $(LDLIBS)

# Build and run all tests. The assertions in test/inc/assert.h print FAIL but
# do not set an exit code, so a test fails if it exits non-zero or if its
# output contains FAIL. All tests run, even after a failure. Output is
# captured, so tests that need a terminal on stdout skip themselves.
test: all
	@failed=""; \
	for test in $(TEST_BINARIES); do \
		echo "$$test"; \
		output=$$(./$$test 2>&1); \
		status=$$?; \
		echo "$$output"; \
		if [ $$status -ne 0 ] || echo "$$output" | grep -q FAIL; then \
			failed="$$failed $$test"; \
		fi; \
	done; \
	echo ""; \
	if [ -n "$$failed" ]; then \
		echo "Failed tests:$$failed"; \
		exit 1; \
	fi; \
	echo "All tests passed"

clean:
	rm -rf $(BIN)

# Dependency tracking. .d files are produced as a side-effect of compilation
# via -MMD -MP in CFLAGS, so no dedicated rule is needed.
-include $(MAIN_OBJECTS:.o=.d)
-include $(TEST_OBJECTS:.o=.d)
