# Public API

Public functions are declared in headers under `main/inc/`. Maven copies these
headers into the distribution. The rules that keep the API compatible with JCC
are in [`../architecture/public-api.md`](../architecture/public-api.md).

## Symbol naming

Every public function follows the scheme below. The garbage collector is the
one exception. It keeps its `jcc_gc_*` names, see
[`garbage-collection.md`](garbage-collection.md).

**The `bas_` prefix.** Every exported function starts with `bas_`. A static
archive's symbols are visible to the whole link. Plain names like `pos`,
`timer` or `read_line` could collide with libc, or with the user's own C code,
in the final link of a BASIC program.

**Signature mangling.** BASIC functions such as `INSTR` and `MID$` take a
varying number of arguments, so one BASIC name can need several exported
functions. The exported name is:

```
bas_<name>[_<type>...]
```

`<name>` is the BASIC function name in lower case, without the `$` suffix.
Each `<type>` is a token for one parameter type, in declaration order:

| BASIC type | C type | Token |
| --- | --- | --- |
| string | `const char *` | `str` |
| integer | `int64_t` | `i64` |
| floating point | `double` | `f64` |

These rules make the mapping mechanical:

- The name encodes parameter types only, in declaration order. It does not
  encode the return type.
- Every function with BASIC-typed parameters is mangled, also when it has only
  one form: `bas_chr_i64`, not `bas_chr`.
- A function with no parameters has no suffix: `bas_timer`, not
  `bas_timer_void`.
- A function whose parameters are not BASIC types has the prefix only. JCC
  calls these functions to support the generated code. `bas_gosub_push` takes a
  `void *` and `bas_init_command_line` takes `int argc, char *argv[]`.

Examples:

| BASIC function | Exported symbol |
| --- | --- |
| `CHR$(integer)` | `bas_chr_i64` |
| `DATE$` | `bas_date` |
| `INSTR(string, string)` | `bas_instr_str_str` |
| `INSTR(integer, string, string)` | `bas_instr_i64_str_str` |
| `MID$(string, integer, integer)` | `bas_mid_str_i64_i64` |
| `RND` | `bas_rnd` |
| `RND(float)` | `bas_rnd_f64` |
| `STRING$(integer, string)` | `bas_string_i64_str` |
| `+` on two strings | `bas_add_str_str` |

The scheme is the one libjcccol uses, with `bas_` in place of `col_`.
[ADR 0003](../adr/0003-bas-symbol-naming-scheme.md) records the decision and
the alternatives.

## Other naming conventions

- Header filenames are snake_case, without `$` and without the `bas_` prefix,
  for example `mid.h` and `mki_cvi.h`.
- Header guards use the `JCCBAS_<MODULE>_H` pattern, for example
  `JCCBAS_MKI_CVI_H`. `jcc_gc.h` keeps `JCC_GC_H_`, because libjcccol vendors
  an identical copy.
- Internal and `static` functions have no `bas_` prefix. The prefix marks the
  exported API only.
- The internal helpers are not `static`, so `libjccbas.a` still exports them
  without a prefix. On macOS and Linux these are `Sleep`, `_kbhit`, `_getch`
  and `_getsystime` from `kbd_util` and `time_util`, and
  `get_cursor_position` from `screen_util`. On Windows only
  `get_cursor_position` is exported. User C code that defines one of these
  names can collide with libjccbas at link time.
- The test assertions in `test/inc/assert.h` use the same lower-case type
  tokens, for example `assert_equals_i64_i64` and `assert_equals_str_str`.

## Adding a public function

1. Derive the exported name from the scheme above. JCC emits calls to this
   name directly, so the name is the whole contract.
2. Follow the steps in [`adding-functions.md`](adding-functions.md).
