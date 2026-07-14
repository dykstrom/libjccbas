# Build system

libjccbas uses a hybrid build: GNU Make for native compilation, Maven for packaging and release.

## Platform and architecture detection

Detection is automatic. The top-level `Makefile` selects the platform: Darwin → macOS, Linux → Linux, otherwise → Windows. It then includes the matching per-platform makefile.

On Windows the build detects ARM64 vs x86_64 by checking `PROCESSOR_IDENTIFIER` and `uname -a` output, because emulation layers (e.g. Git Bash x86_64 on ARM64) can mask the real hardware architecture. The build always targets the native architecture — no cross-compilation.

## Makefile structure

Split across files to avoid repetition:

| File | Role |
|------|------|
| `Makefile` | Main entry point; platform detection |
| `Makefile.common` | Shared variables and the `HEADERS`, `MAIN_OBJS`, `TEST_NAMES` file lists |
| `Makefile.windows` | Windows configuration (with `Makefile.windows.gcc` and `Makefile.windows.clang`) |
| `Makefile.macos` | macOS configuration |
| `Makefile.linux` | Linux configuration |

## Build process (`make`)

1. Platform and architecture are detected.
2. `bin/` subdirectories are created — Windows makes both `bin/gcc` and `bin/clang`; macOS/Linux make only `bin/clang`.
3. `main/src/*.c` is compiled to `bin/gcc/*.o` or `bin/clang/*.o` with the platform compiler and flags.
4. Object files are linked/archived into the library:
   - Windows GCC → `bin/gcc/libjccbas.dll`
   - Windows/macOS/Linux Clang → `bin/clang/libjccbas.a`
5. `test/src/*.c` is compiled with the test includes.
6. Test executables are linked against the library.

`make clean` removes the entire `bin/` directory.

## Windows C runtime split

`libjccbas.dll` (GCC/MinGW build) links against the legacy `msvcrt.dll`, for compatibility with FASM-generated executables. `libjccbas.a` (Clang build) links against UCRT, for modern LLVM/Clang toolchains. This split is intentional; do not unify the two without checking both consumers.

## Maven packaging (`mvn package`)

Produces distribution archives (zip/tar.gz) under `target/` containing the libraries (`lib/libjccbas.dll` and/or `lib/libjccbas.a`, depending on what was built), the public headers (`inc/`), README, and LICENSE. The assembly plugin copies resources from `bin/` and `main/inc/` into `target/` during the build. Because Maven copies only from `main/inc/`, internal `*_util.h` headers in `main/src/` are never distributed.

## Continuous integration

- `.github/workflows/build.yml` — runs on pushes to master/dev and on pull requests. Builds Windows x86_64, macOS ARM64/x86_64, and Linux x86_64.
- `.github/workflows/release.yml` — runs when a version tag (e.g. `v1.5.1`) is pushed from master. Builds on all platforms, runs tests, and creates a GitHub Release with the distribution artifacts.

Release procedure is documented in `release-process.md` (Maven Release Plugin, with a manual fallback).

## Development requirements

- **Windows (x86_64 or arm64):** MinGW-w64 (64-bit GCC) for the DLL build; LLVM-MinGW toolchain (e.g. llvm-mingw-ucrt) for the Clang `.a` build; GNU Make; Unix tools `mkdir`, `rm` (via GnuWin32 CoreUtils or Git Bash).
- **macOS (arm64 or x86_64):** Clang/LLVM and GNU Make (Xcode Command Line Tools or Homebrew); built-in Unix tools.
- **Linux (arm64 or x86_64):** Clang/LLVM, GNU Make, built-in Unix tools.
