/*
 * Copyright (C) 2024 Johan Dykstrom
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "command.h"

#define ARGC(argv) ((int) (sizeof(argv) / sizeof(argv[0])))

static void assert_command(const char *expected) {
  char *actual = bas_command();
  assert_equals_str_str(expected, actual);
  free(actual);
}

int main() {
  // Init never called
  assert_command("");

  // Program name only
  char *only_program[] = {"prog"};
  bas_init_command_line(ARGC(only_program), only_program);
  assert_command("");

  // No argv at all
  bas_init_command_line(0, NULL);
  assert_command("");

  // Program name dropped, letters in upper case
  char *mixed_case[] = {"/path/PROG.exe", "foo", "Bar"};
  bas_init_command_line(ARGC(mixed_case), mixed_case);
  assert_command("FOO BAR");

  // Leading spaces and tabs removed
  char *leading_blanks[] = {"prog", " \t foo", "bar"};
  bas_init_command_line(ARGC(leading_blanks), leading_blanks);
  assert_command("FOO BAR");

  // Inner and trailing blanks kept
  char *inner_trailing_blanks[] = {"prog", "a  ", "b "};
  bas_init_command_line(ARGC(inner_trailing_blanks), inner_trailing_blanks);
  assert_command("A   B ");

  // Non-ASCII bytes unchanged
  char *non_ascii[] = {"prog", "a\xc3\xa5"};
  bas_init_command_line(ARGC(non_ascii), non_ascii);
  assert_command("A\xc3\xa5");

  // A later call replaces the line
  char *first[] = {"prog", "one"};
  char *second[] = {"prog", "two"};
  bas_init_command_line(ARGC(first), first);
  bas_init_command_line(ARGC(second), second);
  assert_command("TWO");

  // Each call returns a new copy
  char *copy1 = bas_command();
  char *copy2 = bas_command();
  assert_true_bool(copy1 != copy2);
  free(copy1);
  assert_equals_str_str("TWO", copy2);
  free(copy2);
}
