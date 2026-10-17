# Random numbers (`randomize_rnd`)

`randomize` and `rnd_F64` in `main/src/randomize_rnd.c` copy the QuickBASIC 4.5
random number generator bit for bit. [ADR 0002](../adr/0002-copy-quickbasic-random-number-generator.md)
records why. The module does not use the C library's `rand`, so the same seed
gives the same numbers on every platform.

The generator keeps one global 24-bit seed, which starts at 327680. Calling
`randomize` twice with the same seed does not restart the sequence, because
`randomize` keeps the low byte of the old seed. To restart a sequence, call
`rnd_F64(-1.0)` before `randomize`.

## Tests

The golden tests in `test/src/test_randomize_rnd.c` depend on the initial seed.
They must run first in `main()`, in their current order. Add new tests after them.

The expected values were confirmed in QuickBASIC 4.5 under DOSBox with the
program below. It prints each seed as an integer, in the same order as the
golden tests. To check a new expected value, add the same calls to the program
and run it in DOSBox.

```basic
DEFLNG A-Z
PRINT CLNG(RND(0) * 16777216)
FOR i = 1 TO 5: PRINT CLNG(RND * 16777216): NEXT
PRINT CLNG(RND(-1) * 16777216)
FOR i = 1 TO 2: PRINT CLNG(RND * 16777216): NEXT
x! = RND(-1): RANDOMIZE 10
FOR i = 1 TO 3: PRINT CLNG(RND * 16777216): NEXT
x! = RND(-1): RANDOMIZE -5
FOR i = 1 TO 3: PRINT CLNG(RND * 16777216): NEXT
x! = RND(-1): RANDOMIZE 1234.5
FOR i = 1 TO 3: PRINT CLNG(RND * 16777216): NEXT
PRINT CLNG(RND(-3.5) * 16777216)
RANDOMIZE 10
FOR i = 1 TO 3: PRINT CLNG(RND * 16777216): NEXT
RANDOMIZE 10
FOR i = 1 TO 3: PRINT CLNG(RND * 16777216): NEXT
```
