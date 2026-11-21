# 0003. Name exported functions with a bas_ prefix and type tokens

*2026-10-26*

## Context

JCC-emitted LLVM IR calls libjccbas functions by symbol name. Before 3.0.0, most exported functions had plain BASIC names such as `pos`, `timer` and `chr$`. These names can collide with libc or with the user's own C code in the final link. Only functions with several forms had type suffixes, and these used upper-case tokens such as `instr_I64_Str_Str`. `string$_I64` encoded only one of its two parameters. libjcccol already uses a `col_` prefix with lower-case type tokens (libjcccol ADR 0001). The alternatives were to add type suffixes only to functions with several forms, to keep the `$` in names (`bas_chr$_i64`), and to give the support functions `bas_gosub_push` and `bas_init_command_line` their own tokens (`ptr`, `i32`).

## Decision

We will name every exported function `bas_<name>[_<type>...]`, with the `$` removed and the libjcccol tokens `str`, `i64` and `f64`. Every function with BASIC-typed parameters gets type tokens, so that one simple rule gives the name. Support functions whose parameters are not BASIC types get the prefix only. The `jcc_gc_*` functions keep their names.

## Consequences

JCC can derive every libjccbas name from the BASIC signature, and libjccbas and libjcccol now follow the same rule. Without the `$`, all names are plain C identifiers. The trade-off is an ABI break: version 3.0.0 renames every exported function, and JCC must update every `ExternalFunction` name in `LibJccBasBuiltIns.java` when it moves to 3.0.0. Two exported functions cannot differ only in return type, because the name does not encode it. A new support function must also take only the prefix, and a new BASIC function must always get type tokens.
