# GOSUB/RETURN control flow (`gosub`)

`gosub` provides the runtime support for BASIC's `GOSUB`/`RETURN` statements in
programs compiled with JCC's LLVM backend. `GOSUB`/`RETURN` does not map to the
native `call`/`ret` — it is a computed goto with a manually managed return-address
stack. The compiler emits calls into this module and drives the branching itself.

The API (`main/inc/gosub.h`, implemented in `main/src/gosub.c`):

```c
void  gosub_push(void* addr);  // push a return address
void* gosub_pop(void);         // pop the return address to jump back to
void  gosub_init(void);        // reset the stack (called automatically on first use)
int   gosub_depth(void);       // current depth — debugging aid only
```

## LLVM IR integration contract

The JCC compiler generates the control flow using two LLVM features: `blockaddress`
(a `ptr` to a basic-block label) and `indirectbr` (indirect branch to an address).

- **GOSUB** — push the address of the statement *after* the `GOSUB` as the return
  address, then branch to the subroutine:

  ```llvm
  call void @gosub_push(ptr blockaddress(@main, %return_label))
  br label %subroutine_label
  ```

- **RETURN** — pop the return address and `indirectbr` to it:

  ```llvm
  %addr = call ptr @gosub_pop()
  indirectbr ptr %addr, [label %target1, label %target2, ...]
  ```

**The whole-program `indirectbr` target list is the key contract.** `indirectbr`
must enumerate every possible target block at compile time. Because any `RETURN`
can return to any `GOSUB`'s follow-on label, **every** `RETURN`'s `indirectbr` must
list **every** label that follows **any** `GOSUB` in the entire program — not just
those reachable from that particular `RETURN`. The compiler collects the set of all
post-`GOSUB` labels in one pass and emits the complete set at each `RETURN`. The
`gosub.h` header carries a worked BASIC→IR example.

## Stack semantics

- Fixed capacity of 100 entries (`GOSUB_STACK_SIZE` in `gosub.c`).
- The stack is a single global array — one shared call stack, single-threaded, not
  thread-safe.
- Auto-initialises on first `push`/`pop`/`depth`; `gosub_init` is idempotent (only
  resets when not yet initialised).
- Overflow (depth exceeds 100) and underflow (`RETURN` without a matching `GOSUB`)
  each print an error to `stderr` and `exit(1)` — they are not recoverable.
