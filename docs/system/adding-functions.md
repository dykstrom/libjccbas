# Adding functions and code conventions

## Adding a public BASIC function

1. Create `main/inc/function_name.h` with the function declaration (public API).
2. Create `main/src/function_name.c` with the implementation.
3. Create `test/src/test_function_name.c` with unit tests using the `assert.h` macros.
4. Add the header to `HEADERS` in `Makefile.common`.
5. Add the object file to `MAIN_OBJS` in `Makefile.common`.
6. Add the test executable to `TEST_NAMES` in `Makefile.common`.
7. Run `make`, then `./run_all_tests.sh` to verify.

## Adding an internal utility function

For helpers that are not part of the public BASIC API:

1. Create `main/src/util_name.h` with the internal declarations.
2. Create `main/src/util_name.c` with the implementation.
3. Add the header to `HEADERS` in `Makefile.common` (at the end, before `assert.h`).
4. Add the object file to `MAIN_OBJS` in `Makefile.common`.
5. The header stays in `main/src/`, so Maven never distributes it (it copies public headers only from `main/inc/`).

## Platform-specific code

- Use `#ifdef _WIN32` for Windows-specific code, `#else` for Unix-like systems (macOS/Linux).
- For reusable platform-specific code, add to or create a shared utility module (see `kbd_util`, `time_util`, `screen_util`).
- Terminal-dependent tests must check `isatty()` and skip gracefully when not run in an appropriate environment.

## Code conventions

- Integer returns use `int64_t` (matches JCC's integer type).
- String parameters are `const char *`.
- Test assertions are inline functions in `test/inc/assert.h`, named `assert_equals_TYPE1_TYPE2(expected, actual)`. Available variants: I64, F64, Str, Bool, plus array comparisons.
- Version encoding: `MAJOR * 1000000 + MINOR * 1000 + PATCH` (e.g. 1.12.7 = 1012007).
