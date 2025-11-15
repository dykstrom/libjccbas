/*
 * Copyright (C) 2023 Johan Dykstrom
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
#include "sleep.h"
#include "timer.h"

int main(int argc, char *argv[]) {
  // Skip test if stdin is not a terminal (e.g., in CI/automation environments)
  // because sleep_F64 checks for keyboard input which requires a terminal
  if (!isatty(STDIN_FILENO)) {
    printf("Skipping test_sleep: stdin is not a terminal\n");
    return 0;
  }

  double start, stop;

  start = timer();
  sleep_F64(0.1);
  stop = timer();
  assert_true_Bool(stop - start >= 0.1);
  assert_true_Bool(stop - start < 0.2);

  start = timer();
  sleep_F64(0.5);
  stop = timer();
  assert_true_Bool(stop - start >= 0.5);
  assert_true_Bool(stop - start < 0.6);
}
