# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

jccbasic is the JCC BASIC standard library, a component of the [JCC compiler project](https://github.com/dykstrom/jcc). It provides runtime library functions for BASIC programs compiled with JCC, distributed as `jccbasic.dll`. The library implements standard BASIC functions like ASC, CHR$, STR$, and various mathematical, string manipulation, and I/O functions.

## Architecture

The codebase is organized into:

- `main/inc/` - Public C header files (.h) for each library function
- `main/src/` - C implementation files (.c) for each library function
- `test/inc/` - Test infrastructure (assert.h with colored output assertions)
- `test/src/` - Unit test executables (test_*.c) - one per function
- `bin/` - Build output directory (created during build)
- `main/assembly/` - Maven assembly descriptor for distribution packaging

Each BASIC function is implemented as a separate module with its own .h/.c pair in main/ and a corresponding test_*.c in test/. The library currently contains ~30 functions including:
- String functions: ASC, CHR$, COMMAND$, INSTR, LCASE$, LEFT$, LTRIM$, MID$, RIGHT$, RTRIM$, SPACE$, STR$, STRING$, UCASE$
- Type conversion: CDBL, CINT, FIX, INT, MKD$, CVD, MKI$, CVI
- Math: SGN, RANDOMIZE, RND
- Formatting: HEX$, OCT$
- I/O: INKEY$
- Screen/Cursor: POS, CSRLIN
- System: DATE$, TIME$, SLEEP, TIMER, JCCBASIC_VERSION
- Array: LBOUND, UBOUND

### Platform Utility Modules

The library includes utility modules that provide cross-platform abstractions:
- `kbd_util` - Keyboard utilities for Unix-like systems (provides _kbhit, _getch, Sleep to match Windows conio.h)
- `time_util` - Time utilities for Unix-like systems (provides _getsystime)
- `screen_util` - Screen/cursor utilities for all platforms (provides get_cursor_position for POS/CSRLIN)

## Build System

The project uses a hybrid build system: GNU Make for native compilation and Maven for packaging/release.

The build system supports multiple platforms:
- **Windows (x86_64)**: Builds `jccbasic.dll` using GCC
- **macOS (arm64/x86_64)**: Builds `libjccbas.a` using Clang/LLVM
- **Linux (arm64/x86_64)**: Builds `libjccbas.a` using Clang/LLVM

Platform and architecture detection is automatic based on `uname -s` and `uname -m`. The build uses the native architecture without cross-compilation.

### Makefile Structure

The build system is split into multiple files to avoid repetition:
- `Makefile` - Main entry point with platform detection
- `Makefile.common` - Shared variables and file lists
- `Makefile.windows` - Windows-specific configuration
- `Makefile.macos` - macOS-specific configuration
- `Makefile.linux` - Linux-specific configuration

### Building the Library

```bash
make
```

On Windows, this compiles all C sources, creates `bin/jccbasic.dll`, and builds all test executables.

On macOS and Linux, this compiles all C sources, creates `bin/libjccbas.a`, and builds all test executables.

Build process:
1. Platform and architecture are detected automatically (Darwin → macOS, Linux → Linux, otherwise → Windows)
2. Creates bin/ directory
3. Compiles main/src/*.c to bin/*.o using platform-specific compiler and flags
4. Links/archives object files into the library (DLL on Windows, static lib on macOS/Linux)
5. Compiles test/src/*.c to bin/*.o with test includes
6. Links test executables against the library

### Running Tests

Run all tests:
```bash
./run_all_tests.sh
```

This executes all test executables in bin/ and displays colored output (green OK / red FAIL).

Run a single test:
```bash
# Windows
./bin/test_asc.exe

# macOS and Linux
./bin/test_asc
```

### Clean Build

```bash
make clean
```

Removes the entire bin/ directory.

### Maven Packaging

```bash
mvn package
```

Creates distribution archives (zip/tar.gz) in target/ containing:
- The DLL (lib/jccbasic.dll)
- Header files (inc/)
- README and LICENSE

The Maven assembly plugin copies resources from bin/ and main/inc/ into target/ during the build process.

## Development Requirements

Platform-specific requirements:

**Windows (x86_64):**
- MinGW-w64 (64-bit GCC for Windows)
- GNU Make
- Standard Unix tools: mkdir, rm (available via GnuWin32 CoreUtils or Git Bash)

**macOS (arm64 or x86_64):**
- Clang and LLVM toolchain (assumed to be in PATH)
- GNU Make (available via Xcode Command Line Tools or Homebrew)
- Standard Unix tools (built-in)

**Linux (arm64 or x86_64):**
- Clang and LLVM toolchain (assumed to be in PATH)
- GNU Make
- Standard Unix tools (built-in)

## Adding New Functions

When adding a new BASIC function to the library:

1. Create main/inc/function_name.h with function declaration
2. Create main/src/function_name.c with implementation
3. Create test/src/test_function_name.c with unit tests using assert.h macros
4. Add the header to HEADERS list in Makefile.common
5. Add the object file to MAIN_OBJS list in Makefile.common
6. Add the test executable to TEST_EXES list in Makefile.common
7. Run `make` to build and `./run_all_tests.sh` to verify

### Platform-Specific Implementations

For functions requiring platform-specific code:
- Use `#ifdef _WIN32` for Windows-specific code, `#else` for Unix-like systems (macOS/Linux)
- Consider creating shared utilities (like kbd_util, time_util, screen_util) for reusable platform-specific code
- Tests should check `isatty()` and skip gracefully when not running in appropriate environments (e.g., terminal-dependent functions)

## Code Conventions

- Functions use int64_t for integer returns (matches JCC's integer type)
- String parameters are const char*
- Test assertions are inline functions in test/inc/assert.h
- Assert functions named as assert_equals_TYPE1_TYPE2(expected, actual)
- Available assertions: I64, F64, Str, Bool variants plus array comparisons
- Version encoding: MAJOR * 1000000 + MINOR * 1000 + PATCH (e.g., 1.12.7 = 1012007)
