# Garbage collection (`jcc_gc`)

`jcc_gc` is the runtime garbage collector for programs compiled with JCC's LLVM
backend. It is a **precise, non-moving mark-and-sweep** collector with
compiler-managed roots (a shadow stack). It is single-threaded and not
async-signal-safe, and uses only ISO C (no platform code beyond `getenv`/`fopen`).

The public API (`main/inc/jcc_gc.h`) is **fixed** by
[jcc#63](https://github.com/dykstrom/jcc/issues/63) §3.2. This repository holds
the canonical `jcc_gc.h`/`jcc_gc.c`; libjcccol vendors identical copies. Do not
change signatures without updating jcc#63 first. The header is kept verbatim to
the spec (no GPL banner) so the vendored copies stay byte-identical.

## Ownership model

`jcc_gc_register(p)` transfers ownership of a `malloc`'d block `p` to the GC and
returns `p`. Unreachable blocks are later reclaimed with `free()` (or the type's
`finalize` callback). The runtime string functions keep allocating with `malloc`
and are unaware of the GC — the compiler wraps their results in
`jcc_gc_register`.

## Roots

The compiler registers the **addresses of pointer slots**, not the pointers
themselves; the collector reads each slot's current value at mark time. A slot
may legally hold `NULL` or a value that is not a registered allocation (e.g. a
string literal in rodata) — such values are ignored.

- **Global roots** — `jcc_gc_set_global_roots` stores a pointer to an array of
  `{slots, count}` ranges terminated by `{NULL, 0}`. A scalar global is
  `{&slot, 1}`; a string array's element region is `{base, count}`. The array is
  read at every collection (not copied) and must live for the program's lifetime.
- **Local roots (shadow stack)** — `jcc_gc_push_frame` records a watermark on
  function entry; `jcc_gc_add_root(&slot)` appends a slot to the current frame;
  `jcc_gc_pop_frame` drops every root added since the matching push. Non-parameter
  slots must be initialised to `NULL` before the first collection can see them.

## Collection

`jcc_gc_collect` performs a full mark-sweep:

1. **Mark** — clear all marks, then for every global-range slot and every local
   root, load the slot's value and hash-look-up the allocation table. If found,
   mark it and, for typed objects, call the type's `trace` to mark interior
   references (recursively).
2. **Sweep** — reclaim every unmarked entry (`finalize` if the type provides one,
   otherwise `free`), then compact the table to the survivors.

The allocation table is an open-addressing hash set keyed on the pointer, with
entries `{ptr, type, mark}`. The sweep rebuilds the table with survivors only, so
no tombstones are needed.

## Trigger and threshold

Registration counts live objects. When `live >= threshold`, `jcc_gc_register`
collects **before** inserting the new pointer — so a registration never sweeps
the very block it is registering. After collecting, while `live > threshold/2`,
the threshold doubles (so a genuinely growing heap is not collected on every
registration). The initial threshold comes from `jcc_gc_init`; `<= 0` selects the
default of 100.

**Caller contract:** after `jcc_gc_register(p)` returns, store `p` into a
registered root slot before the next allocation or registration, or the next
collection may reclaim it.

## Debug output

Debug output is enabled by the `JCC_GC_DEBUG` init flag **or** a non-empty
`JCC_GC_DEBUG` environment variable. It is written to stdout, or appended to the
file named by `JCC_GC_LOG` when that variable is set. The collector logs init,
each collection (live before → after, threshold changes), and — from
`jcc_gc_shutdown`, which `jcc_gc_init` installs via `atexit` — exit statistics:

```
jcc_gc: exit: registered=N collections=M freed=K live=L
```

JCC's integration tests assert on this exact line, so its format is part of the
contract. Because shutdown runs via `atexit`, the line is emitted even when a
BASIC `END`/`SYSTEM` calls `exit()`. The invariant `registered == freed + live`
holds at exit (the line is printed before the final objects are freed).

## Design decisions

- **`finalize` fully reclaims the object.** When a type's `finalize` is non-NULL
  the GC calls it *instead of* `free` — `finalize` is responsible for freeing the
  block. A `NULL` finalize means plain `free(obj)`. This only affects the future
  typed path (closures/records); strings use `NULL`.
- **`jcc_gc_init` is idempotent-reset.** Real programs call it once, but a second
  call frees all tracked objects and resets the collector. This is safe
  (`jcc_gc_shutdown` is idempotent and `atexit` is registered only once) and lets
  the unit tests run isolated scenarios without any test-only API.

## Fresh-block audit

The GC takes ownership at `jcc_gc_register(p)` and later `free()`s `p`, so every
`Str`-returning export must return a *freshly* `malloc`'d block — registering a
static buffer or string literal would later `free()` non-heap memory. All 23
exports were audited and **every one already returns a fresh `malloc`'d block**:

`add_Str_Str`, `chr$`, `command$`, `date$`, `time$`, `hex$`, `inkey$`, `lcase$`,
`left$`, `ltrim$`, `mid$_Str_I64`, `mid$_Str_I64_I64`, `oct$`, `right$`, `rtrim$`,
`space$`, `str_I64`, `str_F64`, `string$_I64`, `string$_Str`, `ucase$`, `mkd$`,
`mki$`.

Notably `command$` returns a `malloc`'d **copy** of its cached command line, not
the cache itself. No offenders were found, so no code changes were needed.
