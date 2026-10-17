# Random numbers

- `randomize`, `rnd`, and `rnd_F64` MUST return exactly the same values as
  QuickBASIC 4.5 for the same sequence of calls, including the low byte that
  `RANDOMIZE` keeps. ([ADR 0002](../adr/0002-copy-quickbasic-random-number-generator.md))
- The module MUST NOT use the C library's `rand` or `srand`.
  ([ADR 0002](../adr/0002-copy-quickbasic-random-number-generator.md))
- A change to expected values in `test/src/test_randomize_rnd.c` MUST be
  confirmed against QuickBASIC 4.5. See [system/random-numbers.md](../system/random-numbers.md).
