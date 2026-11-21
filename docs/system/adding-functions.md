# Adding functions and code conventions

## Adding a public BASIC function

1. Derive the exported name from the naming scheme in [public-api.md](public-api.md), for example `bas_left_str_i64`.
2. Create `main/inc/function_name.h` with the function declaration (public API) and a `JCCBAS_<MODULE>_H` header guard.
3. Create `main/src/function_name.c` with the implementation.
4. Create `test/src/test_function_name.c` with unit tests using the `assert.h` macros.
5. Run `make test` to verify.

The `Makefile` finds new `.c` files and tracks header dependencies by itself, so you do not edit it.

## Adding an internal utility function

For helpers that are not part of the public BASIC API:

1. Create `main/src/util_name.h` with the internal declarations.
2. Create `main/src/util_name.c` with the implementation.
3. The header stays in `main/src/`, so Maven never distributes it (it copies public headers only from `main/inc/`).

## Platform-specific code

- Use `#ifdef _WIN32` for Windows-specific code, `#else` for Unix-like systems (macOS/Linux).
- For reusable platform-specific code, add to or create a shared utility module (see `kbd_util`, `time_util`, `screen_util`).
- Terminal-dependent tests must check `isatty()` and skip gracefully when not run in an appropriate environment.
- On Windows the default stack is 1 MB. A test that declares large local arrays crashes there with exit code `0xC00000FD` (stack overflow) and prints nothing. Declare large arrays `static`, as `test_randomize_rnd.c` does.

## Code conventions

- Integer returns use `int64_t` (matches JCC's integer type).
- String parameters are `const char *`.
- Test assertions are inline functions in `test/inc/assert.h`, named `assert_equals_<type1>_<type2>(expected, actual)` with lower-case type tokens. Available variants: `i32`, `i64`, `f64`, `str`, `ptr`, `bool`, plus array comparisons.
