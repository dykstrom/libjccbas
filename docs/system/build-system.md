# Build system

libjccbas uses GNU Make to compile and test, and Maven to package and release. Clang builds the static library `libjccbas.a` on every platform.

## Makefile

A single `Makefile` builds the library and the tests. It has no file lists:

- Every `.c` file in `main/src/` goes into the library.
- Every `.c` file in `test/src/` becomes one test executable.
- Clang writes a `.d` file next to each object (`-MMD -MP`), and `make` includes these files. A changed header therefore recompiles every object that includes it.

`CC` defaults to `clang` and `AR` defaults to `ar`. You can override both on the command line, for example `make CC=clang-21`.

Clang builds for the host platform and architecture. There is no cross-compilation and no architecture detection. On Windows, the MSYS2 environment decides the target: MINGW64 builds x86_64 and CLANGARM64 builds ARM64.

Platform differences in the `Makefile`:

| Platform | Difference |
|----------|------------|
| Windows | Test executables get the `.exe` suffix. The `Makefile` detects Windows when `uname -s` contains `MINGW` or when `OS` is `Windows_NT`. |
| Linux | Test executables link with `-lm`. |

## Targets

| Target | What it does |
|--------|--------------|
| `make` (`all`) | Builds `bin/libjccbas.a` and all test executables. |
| `make test` | Builds, then runs every test executable and prints its output. Exits non-zero if any test exits non-zero or prints `FAIL`. |
| `make clean` | Removes `bin/`. |
| `make help` | Lists the targets. |

The assertions in `test/inc/assert.h` print `FAIL` but do not set an exit code. This is why `make test` also searches the test output for `FAIL`. It runs all tests, even after a failure, and lists the failed tests at the end.

`make test` captures test output, so stdout is not a terminal. The POS and CSRLIN tests then skip. Run `./bin/test_pos` or `./bin/test_csrlin` directly in a terminal to exercise them.

## Output layout

| Path | Contents |
|------|----------|
| `bin/libjccbas.a` | The library |
| `bin/test_*` (`.exe` on Windows) | Test executables |
| `bin/obj/` | Library objects and their `.d` files |
| `bin/obj/test/` | Test objects and their `.d` files |

## Maven packaging (`mvn package`)

`mvn package` builds a distribution archive under `target/`, named `libjccbas-<version>-<platform>.zip` on Windows and `libjccbas-<version>-<platform>.tar.gz` elsewhere. A Maven profile selects the platform from the OS and architecture, and each platform has an assembly descriptor in `main/assembly/`.

The archive contains `lib/libjccbas.a`, the public headers in `inc/`, `README.md`, and `LICENSE`. Run `make` before `mvn package`, because Maven copies the library from `bin/`. Maven copies headers only from `main/inc/`, so the internal `*_util.h` headers in `main/src/` are never distributed.

JCC downloads the archive from GitHub Releases by this file name and copies out `libjccbas.a`. The file name pattern must not change.

## Continuous integration

- `.github/workflows/build.yml` runs on pushes and pull requests for all branches.
- `.github/workflows/release.yml` runs when a version tag (e.g. `v3.0.0`) is pushed from master. It creates a GitHub Release with the distribution archives.

Both workflows build on six platforms: Windows x86_64 and ARM64, macOS ARM64 and x86_64, and Linux x86_64 and ARM64. Each job runs `make`, `make test`, and `mvn package`. The Windows jobs use MSYS2 with the MINGW64 environment on x86_64 and the CLANGARM64 environment on ARM64.

`release-process.md` describes the release procedure.

## Development requirements

- **Windows (x86_64 or ARM64):** Clang/LLVM, GNU Make, and a POSIX shell with `uname`. CI uses MSYS2, in the MINGW64 environment on x86_64 or the CLANGARM64 environment on ARM64. The llvm-mingw toolchain with GnuWin32 make 3.81, run from Git Bash, also works.
- **macOS (ARM64 or x86_64):** Clang/LLVM and GNU Make from the Xcode Command Line Tools or Homebrew.
- **Linux (ARM64 or x86_64):** Clang/LLVM and GNU Make.
- **Packaging:** A JDK and Maven, to run `mvn package`.
