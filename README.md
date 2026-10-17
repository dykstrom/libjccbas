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

The JCC BASIC standard library is designed to be used with the [JCC compiler](https://github.com/dykstrom/jcc). It is distributed as a static library, `libjccbas.a`, for Windows, macOS, and Linux.

## Building

### Automated Builds (GitHub Actions)

The library is automatically built and tested on multiple platforms using GitHub Actions:

- Windows x86_64
- Windows ARM64
- macOS ARM64 (Apple Silicon)
- macOS x86_64 (Intel)
- Linux x86_64
- Linux ARM64

Build artifacts are available from the Actions tab after each workflow run.

For information on creating releases, see [`docs/system/release-process.md`](docs/system/release-process.md).

### Local Development

To build the JCC BASIC standard library locally, you need Clang/LLVM and GNU Make.

**Windows:**

- [MSYS2](https://www.msys2.org) with Clang/LLVM and GNU Make. Use the CLANG64 environment on x86_64 and the CLANGARM64 environment on ARM64.

**macOS:**

- Clang/LLVM (included with Xcode Command Line Tools)
- GNU Make (included with Xcode Command Line Tools)

**Linux:**

- Clang/LLVM
- GNU Make

### Build Commands

```bash
# Build library and tests
make

# Build and run tests
make test

# Package with Maven
mvn package
```

## Documentation

- [`AGENTS.md`](AGENTS.md) - Project overview, stack, directory index, and commands
- [`docs/`](docs/) - Durable project context: build system, module layout, and how to add functions
- [`docs/system/release-process.md`](docs/system/release-process.md) - Release process using the Maven Release Plugin
