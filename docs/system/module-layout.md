# Module layout

Each public BASIC function is a separate module with its own `.h`/`.c` pair: the public header in `main/inc/`, the implementation in `main/src/`. Internal utility modules (`*_util`) keep their headers in `main/src/` because they are implementation details, not part of the public API, and are therefore never distributed.

## Function catalog

The library implements ~30 functions:

| Category | Functions |
|----------|-----------|
| String | CHR$, COMMAND$, INSTR, LCASE$, LEFT$, LTRIM$, MID$, RIGHT$, RTRIM$, SPACE$, STR$, STRING$, UCASE$ |
| Type conversion | MKD$, CVD, MKI$, CVI |
| Math | RANDOMIZE, RND |
| Formatting | HEX$, OCT$ |
| I/O | INKEY$ |
| Screen/cursor | POS, CSRLIN |
| System | DATE$, TIME$, SLEEP, TIMER |
| Control flow | GOSUB/RETURN stack management (`gosub_push`, `gosub_pop`, `gosub_init`, `gosub_depth`) |

JCC's LLVM backend generates some BASIC built-ins inline, so it does not call a library version of them. These built-ins are ASC, CDBL, CINT, FIX, INT, SGN, LBOUND, and UBOUND. Version 3.0.0 removed them from the library, see [#9](https://github.com/dykstrom/libjccbas/issues/9).

## Internal platform utility modules

Located in `main/src/`, these provide cross-platform abstractions and are not distributed:

- `kbd_util` — keyboard utilities for Unix-like systems (provides `_kbhit`, `_getch`, `Sleep` to match Windows `conio.h`).
- `time_util` — time utilities for Unix-like systems (provides `_getsystime`).
- `screen_util` — screen/cursor utilities for all platforms (provides `get_cursor_position` for POS/CSRLIN).

## Special (non-intrinsic) functions

Some modules exist to support the compiler rather than to implement a BASIC intrinsic:

- **String concatenation** (`add.h`/`add.c`): `add_Str_Str` implements the `+` operator on strings. Called by generated code, not a BASIC intrinsic.
- **Line input** (`read_line.h`/`read_line.c`): `read_line` reads a line from standard input, dynamically allocating memory for variable-length input and returning a null-terminated string. Used by the compiler to implement BASIC's INPUT statement.
- **GOSUB/RETURN stack** (`gosub.h`/`gosub.c`): runtime support for BASIC's GOSUB/RETURN control flow, called directly by LLVM IR the JCC compiler generates using `blockaddress` and `indirectbr` instructions. See [gosub.md](gosub.md).
- **GC support** (`jcc_gc.h`/`jcc_gc.c`): the runtime garbage collector — a precise mark-and-sweep collector with compiler-managed roots (a shadow stack). Called by JCC-generated code, not a BASIC intrinsic. The `jcc_gc.h` API is fixed by [jcc#63](https://github.com/dykstrom/jcc/issues/63) §3.2; this is the canonical copy (libjcccol vendors an identical one). See [garbage-collection.md](garbage-collection.md).
