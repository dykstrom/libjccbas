# 0004. Build COMMAND$ from argv with QuickBASIC 4.5 rules

*2026-11-07*

## Context

JCC aims to be compatible with QuickBASIC 4.5. There, `COMMAND$` returns the text after the program name, with leading blanks removed and all letters in upper case. Before 3.0.0, `bas_command` returned the program name and the arguments in their original case. On macOS and Linux it joined the argv that JCC passed to `bas_init_command_line`, and on Windows it returned the raw text from `GetCommandLineA()`. JCC issue #111 wants every program to call `bas_init_command_line` on every OS, so that JCC no longer has to check whether a program uses `COMMAND$`. The alternatives were to keep the old result, or to keep `GetCommandLineA()` on Windows and remove the program name from its text.

## Decision

We will build the `COMMAND$` result from argv on every OS, using the QuickBASIC 4.5 rules. `bas_command` leaves out `argv[0]`, joins the other arguments with single spaces, converts ASCII letters to upper case, and removes leading spaces and tabs.

## Consequences

A program that reads `COMMAND$` gets the same text as in QuickBASIC 4.5, and the same text on every OS. libjccbas no longer has Windows-specific code for `COMMAND$`. The trade-off is that Windows loses the original quoting of the command line, because the arguments are joined with spaces. A program can no longer read its own name or the original case of its arguments. When JCC moves to 3.0.0, it must call `bas_init_command_line` from `main` on every OS, or `COMMAND$` returns an empty string. Future changes to `COMMAND$` must keep these rules.
