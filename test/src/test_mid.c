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

#include <stdlib.h>

#include "assert.h"
#include "mid.h"

int main(int argc, char *argv[]) {
  char* actual;

  // Test bas_mid_str_i64 - two argument version

  // Test with empty string
  actual = bas_mid_str_i64("", 1);
  assert_equals_str_str("", actual);
  free(actual);

  // Test starting at position 1
  actual = bas_mid_str_i64("hello", 1);
  assert_equals_str_str("hello", actual);
  free(actual);

  // Test starting at position 2
  actual = bas_mid_str_i64("hello", 2);
  assert_equals_str_str("ello", actual);
  free(actual);

  // Test starting at last character
  actual = bas_mid_str_i64("hello", 5);
  assert_equals_str_str("o", actual);
  free(actual);

  // Test starting beyond string length
  actual = bas_mid_str_i64("hello", 10);
  assert_equals_str_str("", actual);
  free(actual);

  // Test bas_mid_str_i64_i64 - three argument version

  // Test with empty string
  actual = bas_mid_str_i64_i64("", 1, 5);
  assert_equals_str_str("", actual);
  free(actual);

  // Test with zero length
  actual = bas_mid_str_i64_i64("hello", 1, 0);
  assert_equals_str_str("", actual);
  free(actual);

  // Test normal case - middle 3 characters
  actual = bas_mid_str_i64_i64("hello", 2, 3);
  assert_equals_str_str("ell", actual);
  free(actual);

  // Test from start with length
  actual = bas_mid_str_i64_i64("hello", 1, 3);
  assert_equals_str_str("hel", actual);
  free(actual);

  // Test when length exceeds remaining string
  actual = bas_mid_str_i64_i64("hello", 3, 10);
  assert_equals_str_str("llo", actual);
  free(actual);

  // Test single character extraction
  actual = bas_mid_str_i64_i64("hello", 3, 1);
  assert_equals_str_str("l", actual);
  free(actual);

  // Test when start equals string length
  actual = bas_mid_str_i64_i64("hello", 5, 2);
  assert_equals_str_str("o", actual);
  free(actual);

  // Test when start is beyond string length
  actual = bas_mid_str_i64_i64("hello", 10, 5);
  assert_equals_str_str("", actual);
  free(actual);

  // Test with longer string
  actual = bas_mid_str_i64_i64("QuickBASIC", 6, 5);
  assert_equals_str_str("BASIC", actual);
  free(actual);

  // Test entire string extraction
  actual = bas_mid_str_i64_i64("hello", 1, 5);
  assert_equals_str_str("hello", actual);
  free(actual);

  // Test length exceeding string length from start
  actual = bas_mid_str_i64_i64("hello", 1, 100);
  assert_equals_str_str("hello", actual);
  free(actual);
}
