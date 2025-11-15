/*
 * Copyright (C) 2019 Johan Dykstrom
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

#ifndef ASSERT_H_
#define ASSERT_H_

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

const char *FAIL = "\033[31mFAIL\033[0m";

const char *OK = "\033[32mOK\033[0m";

// These functions should be macros...

void assert_equals_F64_F64(double expected, double actual) {
  if (expected == actual) {
    printf("%s\n", OK);
  } else {
    printf("%s: expected: %f, but was: %f\n", FAIL, expected, actual);
  }
}

void assert_equals_F64_F64_F64(double expected, double actual, double delta) {
  if ((actual > expected - delta) && (actual < expected + delta)) {
    printf("%s\n", OK);
  } else {
    printf("%s: expected: %f +/- %f, but was: %f\n", FAIL, expected, delta, actual);
  }
}

void assert_not_equals_F64_F64(double unexpected, double actual) {
  if (unexpected != actual) {
    printf("%s\n", OK);
  } else {
    printf("%s: expected difference, but was: %f\n", FAIL, unexpected);
  }
}

void assert_equals_I64_I64(int64_t expected, int64_t actual) {
  if (expected == actual) {
    printf("%s\n", OK);
  } else {
    printf("%s: expected: %" PRId64 ", but was: %" PRId64 "\n", FAIL, expected, actual);
  }
}

void assert_equals_Str_Str(const char* expected, const char* actual) {
  if (strcmp(expected, actual) == 0) {
    printf("%s\n", OK);
  } else {
    printf("%s: expected: '%s', but was: '%s'\n", FAIL, expected, actual);
  }
}

void assert_array_equals_F64_F64(double expected[], double actual[], int length) {
  for (int i = 0; i < length; i++) {
    if (expected[i] != actual[i]) {
      printf("%s: at index %d, expected: %f, but was: %f\n", FAIL, i, expected[i], actual[i]);
      return;
    }
  }
  printf("%s\n", OK);
}

void assert_array_not_equals_F64_F64(double unexpected[], double actual[], int length) {
  for (int i = 0; i < length; i++) {
    if (unexpected[i] != actual[i]) {
      printf("%s\n", OK);
      return;
    }
  }
  printf("%s: expected difference, but all %d elements were equal\n", FAIL, length);
}

void assert_array_equals_char_char(char expected[], char actual[], int length) {
  for (int i = 0; i < length; i++) {
    if (expected[i] != actual[i]) {
      printf("%s: at index %d, expected: '%c' (%d), but was: '%c' (%d)\n", FAIL, i, expected[i], expected[i], actual[i], actual[i]);
      return;
    }
  }
  printf("%s\n", OK);
}

void assert_true_Bool(bool actual) {
  if (actual) {
    printf("%s\n", OK);
  } else {
    printf("%s: expected: %d, but was: %d", FAIL, true, actual);
  }
}

void assert_equals_int_int(int expected, int actual) {
  if (expected == actual) {
    printf("%s\n", OK);
  } else {
    printf("%s: expected: %d, but was: %d\n", FAIL, expected, actual);
  }
}

void assert_equals_ptr_ptr(void* expected, void* actual) {
  if (expected == actual) {
    printf("%s\n", OK);
  } else {
    printf("%s: expected: %p, but was: %p\n", FAIL, expected, actual);
  }
}

#endif /* ASSERT_H_ */
