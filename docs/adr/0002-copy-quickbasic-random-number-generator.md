# 0002. Copy the QuickBASIC 4.5 random number generator

*2026-10-12*

## Context

JCC aims to be compatible with QuickBASIC 4.5. `randomize` and `rnd_F64` used the C library's `srand` and `rand`, so a JCC program printed different random numbers than the same program in QuickBASIC 4.5. The numbers also differed between Windows, macOS, and Linux, because each C library implements `rand` differently. One alternative was to keep the C library. Another was to use the QuickBASIC generator but let `RANDOMIZE n` replace the whole seed. That version always restarts the sequence, which is easier to understand, but its output differs from QuickBASIC.

## Decision

We will copy the QuickBASIC 4.5 generator bit for bit, including its quirks. We will keep the existing public API, which uses double, so the JCC compiler needs no changes.

## Consequences

The same program now prints the same random numbers in JCC as in QuickBASIC 4.5, on every platform. Output from real QuickBASIC 4.5 in DOSBox confirmed this. The trade-off is that `RANDOMIZE n` keeps the low byte of the old seed. A program that calls `RANDOMIZE n` twice therefore gets a different sequence the second time. To restart a sequence, a program must call `RND(-1)` before `RANDOMIZE n`, as in QuickBASIC. Future changes to `RND` or `RANDOMIZE` must keep the output identical to QuickBASIC 4.5.
