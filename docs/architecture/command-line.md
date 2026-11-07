# Command line

- `bas_command` MUST return what `COMMAND$` returns in QuickBASIC 4.5: the
  arguments after the program name, joined with single spaces, with ASCII
  letters in upper case and leading spaces and tabs removed.
  ([ADR 0004](../adr/0004-build-command-from-argv-with-quickbasic-rules.md))
- `bas_command` MUST build its result only from the argv passed to
  `bas_init_command_line`, on every OS. It MUST NOT read the command line from
  the OS, for example with `GetCommandLineA()`.
  ([ADR 0004](../adr/0004-build-command-from-argv-with-quickbasic-rules.md))
- `bas_init_command_line` MUST be declared and defined on every OS. JCC calls
  it from `main` in every BASIC program.
  ([jcc#111](https://github.com/dykstrom/jcc/issues/111))
