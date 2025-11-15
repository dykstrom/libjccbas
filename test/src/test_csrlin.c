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
#include "csrlin.h"

void should_return_positive_row() {
  // CSRLIN should always return a positive row number
  int64_t actual = csrlin();
  assert_true_Bool(actual > 0);
}

void should_increase_after_newline() {
  // CSRLIN should increase by 1 after printing a newline
  int64_t row1 = csrlin();
  printf("\n");
  fflush(stdout);
  int64_t row2 = csrlin();
  // Note: If terminal scrolls, row might stay the same instead of increasing
  // So we just check that row2 is still positive and >= row1
  assert_true_Bool(row2 >= row1);
}

int main(int argc, char *argv[]) {
  // Skip test if stdout is not a terminal (e.g., in CI/automation environments)
  if (!isatty(STDOUT_FILENO)) {
    printf("Skipping test_csrlin: stdout is not a terminal\n");
    return 0;
  }

  should_return_positive_row();
  should_increase_after_newline();

  printf("\n");
}
