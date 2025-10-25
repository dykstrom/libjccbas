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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "asc.h"
#include "assert.h"
#include "inkey.h"

void should_not_wait() {
  char* actual = inkey$();
  assert_equals_Str_Str("", actual);
  free(actual);
}

void should_wait_for_q() {
  char* actual = inkey$();
  while (strcmp(actual, "q") != 0) {
    if (strcmp("", actual) != 0) {
      printf("%s - %" PRId64 "\n", actual, asc(actual));
    }
    free(actual);
    actual = inkey$();
  }
  printf("%s\n", actual);
  assert_equals_Str_Str("q", actual);
  free(actual);
}

int main(int argc, char *argv[]) {
  // Skip test if stdin is not a terminal (e.g., in CI/automation environments)
  if (!isatty(STDIN_FILENO)) {
    printf("Skipping test_inkey: stdin is not a terminal\n");
    return 0;
  }

  should_not_wait();
  //should_wait_for_q();
}
