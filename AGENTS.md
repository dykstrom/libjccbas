# AGENTS.md

## What is this

libjccbas is the JCC BASIC standard library — the runtime library for BASIC programs compiled with the [JCC compiler](https://github.com/dykstrom/jcc). It implements ~30 standard BASIC functions (string, math, type-conversion, formatting, I/O, screen/cursor, date/time, array, and GOSUB/RETURN control-flow support). It's distributed as `libjccbas.dll` on Windows and `libjccbas.a` (static) on Windows/macOS/Linux.

## Stack

| Piece | Choice |
|-------|--------|
| Language | C |
| Build (native) | GNU Make with per-platform makefiles |
| Compilers | GCC/MinGW-w64 (Windows DLL), Clang/LLVM (static `.a` on all platforms) |
| Packaging/release | Maven (assembly + release plugin) |
| Testing | Custom C harness (`test/inc/assert.h`), one executable per function |

**C runtime (Windows):** `libjccbas.dll` (GCC/MinGW build) links against the legacy `msvcrt.dll`, for FASM-generated executable compatibility; the `libjccbas.a` (Clang build) links against UCRT, for modern LLVM/Clang toolchains.

## Directory index

| Path | What's there |
|------|-------------|
| `main/inc/` | Public C headers (`.h`) for the BASIC API functions — packaged for distribution |
| `main/src/` | C implementations (`.c`) plus internal utility headers (`*_util.h`, not distributed) |
| `test/inc/` | Test infrastructure — `assert.h` with colored-output assertion macros |
| `test/src/` | Unit tests, one `test_*.c` per function |
| `main/assembly/` | Maven assembly descriptors for platform-specific packaging |
| `bin/` | Build output (created by `make`); `bin/gcc/` and `bin/clang/` subdirs |
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
| Build library + tests | `make` (auto-detects platform/arch; builds `.dll`+`.a` on Windows, `.a` elsewhere) |
| Run all tests | `./run_all_tests.sh` (runs every `bin/` test executable, colored OK/FAIL) |
| Run one test | `./bin/clang/test_asc` (macOS/Linux) or `./bin/gcc/test_asc.exe` / `./bin/clang/test_asc.exe` (Windows) |
| Clean | `make clean` (removes `bin/`) |
| Package | `mvn package` (distribution zip/tar.gz in `target/`) |

Terminal-dependent tests (INKEY$, POS, CSRLIN, read_line) check `isatty()` and skip gracefully when not run in a real terminal, so they pass silently under CI or piped output rather than failing.
