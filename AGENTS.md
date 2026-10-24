# AGENTS.md

## What is this

libjccbas is the JCC BASIC standard library — the runtime library for BASIC programs compiled with the [JCC compiler](https://github.com/dykstrom/jcc). It implements ~30 standard BASIC functions (string, math, type-conversion, formatting, I/O, screen/cursor, date/time, and GOSUB/RETURN control-flow support). It also provides a runtime garbage collector (`jcc_gc`, a precise mark-and-sweep collector) for JCC's LLVM backend. It's distributed as the static library `libjccbas.a` on Windows, macOS, and Linux.

## Stack

| Piece | Choice |
|-------|--------|
| Language | C |
| Build (native) | GNU Make with a single `Makefile` |
| Compiler | Clang/LLVM on all platforms (MSYS2 on Windows) |
| Packaging/release | Maven (assembly + release plugin) |
| Testing | Custom C harness (`test/inc/assert.h`), one executable per function |

## Directory index

| Path | What's there |
|------|-------------|
| `main/inc/` | Public C headers (`.h`) for the BASIC API functions — packaged for distribution |
| `main/src/` | C implementations (`.c`) plus internal utility headers (`*_util.h`, not distributed) |
| `test/inc/` | Test infrastructure — `assert.h` with colored-output assertion macros |
| `test/src/` | Unit tests, one `test_*.c` per function |
| `main/assembly/` | Maven assembly descriptors for platform-specific packaging |
| `bin/` | Build output (created by `make`): `libjccbas.a`, the test executables, and objects in `bin/obj/` |
| `docs/` | Durable project context. Sub-folder layout below shows where each kind of doc goes. |

```
docs/
├── system/         ← what the code does today (updated as code changes)
├── architecture/   ← what the system must do (updated when rules change)
├── adr/            ← architecture decisions (immutable once shipped)
├── reference/      ← long-form rationale (append-only)
└── working-notes/  ← research; NOT authoritative — rules live in architecture/ + adr/
```

## Commands

| What | Command |
|------|---------|
| Build library + tests | `make` (Clang builds for the host platform and architecture) |
| Run all tests | `make test` (runs every test executable; fails if a test exits non-zero or prints FAIL) |
| Run one test | `./bin/test_chr` (macOS/Linux) or `./bin/test_chr.exe` (Windows) |
| Clean | `make clean` (removes `bin/`) |
| Package | `mvn package` (distribution zip/tar.gz in `target/`) |

Terminal-dependent tests (INKEY$, POS, CSRLIN, read_line) check `isatty()` and skip gracefully when not run in a real terminal, so they pass silently under CI or piped output rather than failing. `make test` captures test output, so POS and CSRLIN always skip there. Run their executables directly in a terminal to exercise them.
