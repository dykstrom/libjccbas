# libjccbas

[![build](https://github.com/dykstrom/libjccbas/actions/workflows/build.yml/badge.svg)](https://github.com/dykstrom/libjccbas/actions/workflows/build.yml)
[![release](https://github.com/dykstrom/libjccbas/actions/workflows/release.yml/badge.svg)](https://github.com/dykstrom/libjccbas/actions/workflows/release.yml)
[![Latest Release](https://img.shields.io/github/v/release/dykstrom/libjccbas?display_name=release)](https://github.com/dykstrom/libjccbas/releases)
![Downloads](https://img.shields.io/github/downloads/dykstrom/libjccbas/total)
[![Open Issues](https://img.shields.io/github/issues/dykstrom/libjccbas)](https://github.com/dykstrom/libjccbas/issues)
![License](https://img.shields.io/github/license/dykstrom/libjccbas)
![Top Language](https://img.shields.io/github/languages/top/dykstrom/libjccbas)

The JCC BASIC standard library - a part of [JCC](https://github.com/dykstrom/jcc).

## Using

The JCC BASIC standard library is designed to be used with the [JCC compiler](https://github.com/dykstrom/jcc). It is distributed as:

- **Windows**: `libjccbas.dll` (linked with msvcrt.dll for FASM) and `libjccbas.a` (linked with UCRT for LLVM/Clang)
- **macOS/Linux**: `libjccbas.a` (static library)

## Building

### Automated Builds (GitHub Actions)

The library is automatically built and tested on multiple platforms using GitHub Actions:

- Windows x86_64
- macOS ARM64 (Apple Silicon)
- macOS x86_64 (Intel)
- Linux x86_64

Build artifacts are available from the Actions tab after each workflow run.

For information on creating releases, see [`RELEASE_PROCESS.md`](RELEASE_PROCESS.md) or [`MAVEN_RELEASE.md`](MAVEN_RELEASE.md).

### Local Development

To build the JCC BASIC standard library locally, you need platform-specific toolchains.

**Windows:**

- 64-bit MinGW-w64 (GCC) - builds `libjccbas.dll` with msvcrt.dll
- Clang/LLVM - builds `libjccbas.a` with UCRT
- GNU Make
- Basic Unix tools (mkdir, rm)

**Note on Windows C Runtime:** The DLL is intentionally linked with the legacy msvcrt.dll (Microsoft C Runtime) instead of UCRT (Universal CRT) to ensure compatibility with FASM-generated executables. The static library uses UCRT for compatibility with modern LLVM/Clang toolchains.

**macOS:**

- Clang/LLVM (included with Xcode Command Line Tools)
- GNU Make (included with Xcode Command Line Tools)

**Linux:**

- Clang/LLVM
- GNU Make

### Installation Instructions

**mingw-w64 (Windows):**
The 64-bit version of MinGW can be downloaded from [mingw-w64](https://mingw-w64.org) or as pre-built binaries from [MinGW-W64-binaries](https://github.com/niXman/mingw-builds-binaries).

**make (Windows):**
GNU make for Windows can be downloaded from the [GnuWin32](http://gnuwin32.sourceforge.net/packages/make.htm) project.

**mkdir and rm (Windows):**
The makefile uses the \*nix tools mkdir and rm. These can be downloaded from the [GnuWin32](http://gnuwin32.sourceforge.net/packages/coreutils.htm) project as part of the CoreUtils package. Alternatively, install bash as part of [Git](https://git-scm.com).

### Build Commands

```bash
# Build library
make

# Run tests
./run_all_tests.sh

# Package with Maven
mvn package
```

## Documentation

- [`CLAUDE.md`](CLAUDE.md) - Detailed build system documentation and development guidelines
- [`RELEASE_PROCESS.md`](RELEASE_PROCESS.md) - Manual release process using Git tags
- [`MAVEN_RELEASE.md`](MAVEN_RELEASE.md) - Automated release process using Maven Release Plugin
