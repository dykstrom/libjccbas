# libjccbas

The JCC BASIC standard library - a part of [JCC](https://github.com/dykstrom/jcc).


## Using

The standard library is not usable on its own. It is distributed with JCC as
libjccbas.dll on Windows or libjccbas.a on macOS/Linux.


## Building

### Automated Builds (GitHub Actions)

The library is automatically built and tested on multiple platforms using GitHub Actions. The workflow builds for:
- Windows x86_64
- macOS ARM64 (Apple Silicon)
- macOS x86_64 (Intel)
- Linux x86_64

Build artifacts are available from the Actions tab after each workflow run.

### Local Development

To build the JCC BASIC standard library locally, you need platform-specific toolchains.

**Windows:**
- 64-bit MinGW-w64 (GCC)
- Clang/LLVM (optional, for static library)
- GNU Make
- Basic Unix tools (mkdir, rm)


**macOS:**
- Clang/LLVM (included with Xcode Command Line Tools)
- GNU Make (included with Xcode Command Line Tools)

**Linux:**
- Clang/LLVM
- GNU Make

### Installation Instructions

**mingw-w64 (Windows):**
The 64-bit version of MinGW can be downloaded from their home page
[mingw-w64](https://mingw-w64.org) or as pre-built binaries from
[MinGW-W64-binaries](https://github.com/niXman/mingw-builds-binaries).

**make (Windows):**
GNU make for Windows can be downloaded from the
[GnuWin32](http://gnuwin32.sourceforge.net/packages/make.htm) project.

**mkdir and rm (Windows):**
The makefile uses the *nix tools mkdir and rm. These can also be downloaded
from the [GnuWin32](http://gnuwin32.sourceforge.net/packages/coreutils.htm)
project as part of the CoreUtils package.

An alternative is to install bash, for example as part of [Git](https://git-scm.com).

### Build Commands

```bash
# Build library
make

# Run tests
./run_all_tests.sh

# Package with Maven
mvn package
```

See `CLAUDE.md` for detailed build system documentation.
