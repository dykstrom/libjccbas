/*
 * Copyright (C) 2025 Johan Dykstrom
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

#include <stdio.h>
#include <unistd.h>

#include "assert.h"
#include "pos.h"

void should_return_column_at_start() {
  // At the start of a line, POS should return 1
  printf("\n");
  fflush(stdout);
  int64_t actual = pos(0);
  assert_equals_I64_I64(1, actual);
}

void should_return_column_after_text() {
  // After printing 5 characters, POS should return 6
  printf("\nHello");
  fflush(stdout);
  int64_t actual = pos(0);
  assert_equals_I64_I64(6, actual);
}

void should_ignore_parameter() {
  // The parameter should be ignored
  printf("\n");
  fflush(stdout);
  int64_t actual1 = pos(0);
  int64_t actual2 = pos(99);
  assert_equals_I64_I64(actual1, actual2);
}

int main(int argc, char *argv[]) {
  // Skip test if stdout is not a terminal (e.g., in CI/automation environments)
  if (!isatty(STDOUT_FILENO)) {
    printf("Skipping test_pos: stdout is not a terminal\n");
    return 0;
  }

  should_return_column_at_start();
  should_return_column_after_text();
  should_ignore_parameter();

  printf("\n");
}
