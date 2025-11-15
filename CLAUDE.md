# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

libjccbas is the JCC BASIC standard library, a component of the [JCC compiler project](https://github.com/dykstrom/jcc). It provides runtime library functions for BASIC programs compiled with JCC, distributed as `libjccbas.dll` on Windows or `libjccbas.a` on macOS/Linux. The library implements standard BASIC functions like ASC, CHR$, STR$, and various mathematical, string manipulation, and I/O functions.

## Architecture

The codebase is organized into:

- `main/inc/` - Public C header files (.h) for the BASIC API functions
- `main/src/` - C implementation files (.c) and internal header files (\*\_util.h)
- `test/inc/` - Test infrastructure (assert.h with colored output assertions)
- `test/src/` - Unit test executables (test\_\*.c) - one per function
- `bin/` - Build output directory (created during build)
- `main/assembly/` - Maven assembly descriptors for platform-specific distribution packaging

Each public BASIC function is implemented as a separate module with its own .h/.c pair. Internal utility modules (\*\_util) have their headers located in main/src/ as they are implementation details, not part of the public API. The library currently contains ~30 functions including:

- String functions: ASC, CHR$, COMMAND$, INSTR, LCASE$, LEFT$, LTRIM$, MID$, RIGHT$, RTRIM$, SPACE$, STR$, STRING$, UCASE$
- Type conversion: CDBL, CINT, FIX, INT, MKD$, CVD, MKI$, CVI
- Math: SGN, RANDOMIZE, RND
- Formatting: HEX$, OCT$
- I/O: INKEY$
- Screen/Cursor: POS, CSRLIN
- System: DATE$, TIME$, SLEEP, TIMER, JCCBASIC_VERSION
- Array: LBOUND, UBOUND
- Control Flow: GOSUB/RETURN stack management (gosub_push, gosub_pop, gosub_init, gosub_depth)

### Internal Platform Utility Modules

The library includes internal utility modules (located in `main/src/`) that provide cross-platform abstractions:

- `kbd_util` - Keyboard utilities for Unix-like systems (provides \_kbhit, \_getch, Sleep to match Windows conio.h)
- `time_util` - Time utilities for Unix-like systems (provides \_getsystime)
- `screen_util` - Screen/cursor utilities for all platforms (provides get_cursor_position for POS/CSRLIN)

These utilities are implementation details and their headers are not included in the distributed packages.

## Build System

The project uses a hybrid build system: GNU Make for native compilation and Maven for packaging/release.

### Continuous Integration

GitHub Actions automatically builds and tests the library on multiple platforms. See `.github/workflows/build.yml` for the complete workflow configuration. The CI builds for Windows x86_64, macOS ARM64/x86_64, and Linux x86_64.

The build system supports multiple platforms:

- **Windows (x86_64/arm64)**: Builds `libjccbas.dll` using GCC and `libjccbas.a` using Clang
- **macOS (arm64/x86_64)**: Builds `libjccbas.a` using Clang/LLVM
- **Linux (arm64/x86_64)**: Builds `libjccbas.a` using Clang/LLVM

Platform and architecture detection is automatic. On Windows, the build detects ARM64 vs x86_64 by checking `PROCESSOR_IDENTIFIER` and `uname -a` output, since emulation layers (like Git Bash x86_64 on ARM64) can mask the actual hardware architecture. The build uses the native architecture without cross-compilation.

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

On Windows, this compiles all C sources, creates `bin/gcc/libjccbas.dll` and `bin/clang/libjccbas.a`, and builds all test executables.

On macOS and Linux, this compiles all C sources, creates `bin/clang/libjccbas.a`, and builds all test executables.

Build process:

1. Platform and architecture are detected automatically (Darwin → macOS, Linux → Linux, otherwise → Windows)
2. Creates bin/gcc and bin/clang subdirectories (Windows creates both, others create only bin/clang)
3. Compiles main/src/_.c to bin/gcc/_.o or bin/clang/\*.o using platform-specific compiler and flags
4. Links/archives object files into the library:
   - Windows GCC: bin/gcc/libjccbas.dll
   - Windows/macOS/Linux Clang: bin/clang/libjccbas.a
5. Compiles test/src/_.c to bin/gcc/_.o or bin/clang/\*.o with test includes
6. Links test executables against the library

### Running Tests

Run all tests:

```bash
./run_all_tests.sh
```

This executes all test executables in bin/ and displays colored output (green OK / red FAIL).

Run a single test:

```bash
# Windows GCC
./bin/gcc/test_asc.exe

# Windows Clang
./bin/clang/test_asc.exe

# macOS and Linux
./bin/clang/test_asc
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

- The libraries (lib/libjccbas.dll and/or lib/libjccbas.a depending on what was built)
- Header files (inc/)
- README and LICENSE

The Maven assembly plugin copies resources from bin/ and main/inc/ into target/ during the build process.

## Development Requirements

Platform-specific requirements:

**Windows (x86_64 or arm64):**

- For GCC builds: MinGW-w64 (64-bit GCC for Windows)
- For Clang builds: LLVM-MinGW toolchain (e.g., llvm-mingw-ucrt for the target architecture)
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

1. Create main/inc/function_name.h with function declaration (for public API)
2. Create main/src/function_name.c with implementation
3. Create test/src/test_function_name.c with unit tests using assert.h macros
4. Add the header to HEADERS list in Makefile.common (if public API)
5. Add the object file to MAIN_OBJS list in Makefile.common
6. Add the test executable to TEST_NAMES list in Makefile.common
7. Run `make` to build and `./run_all_tests.sh` to verify

### Adding Internal Utility Functions

For internal utility functions that are not part of the public BASIC API:

1. Create main/src/util_name.h with internal function declarations
2. Create main/src/util_name.c with implementation
3. Add the header to HEADERS list in Makefile.common (at the end, before assert.h)
4. Add the object file to MAIN_OBJS list in Makefile.common
5. The header will not be included in distributed packages (Maven only copies from main/inc/)

### Platform-Specific Implementations

For functions requiring platform-specific code:

- Use `#ifdef _WIN32` for Windows-specific code, `#else` for Unix-like systems (macOS/Linux)
- Consider creating shared utilities (like kbd_util, time_util, screen_util) for reusable platform-specific code
- Tests should check `isatty()` and skip gracefully when not running in appropriate environments (e.g., terminal-dependent functions)

### Special Functions

Some functions serve specialized purposes beyond standard BASIC operations:

- **String concatenation** (`add.h`/`add.c`): Provides `add_Str_Str` for implementing the `+` operator on strings. This is a compiler support function called by generated code rather than a BASIC intrinsic function.
- **GOSUB/RETURN stack** (`gosub.h`/`gosub.c`): Provides runtime support for BASIC's GOSUB/RETURN control flow. These functions are called directly by LLVM IR generated by the JCC compiler using `blockaddress` and `indirectbr` instructions. See `GOSUB_IMPLEMENTATION.md` for detailed integration notes.

## Code Conventions

- Functions use int64_t for integer returns (matches JCC's integer type)
- String parameters are const char\*
- Test assertions are inline functions in test/inc/assert.h
- Assert functions named as assert_equals_TYPE1_TYPE2(expected, actual)
- Available assertions: I64, F64, Str, Bool variants plus array comparisons
- Version encoding: MAJOR _ 1000000 + MINOR _ 1000 + PATCH (e.g., 1.12.7 = 1012007)
