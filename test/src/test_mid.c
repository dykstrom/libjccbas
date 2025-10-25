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

  // Test mid$_Str_I64 - two argument version

  // Test with empty string
  actual = mid$_Str_I64("", 1);
  assert_equals_Str_Str("", actual);
  free(actual);

  // Test starting at position 1
  actual = mid$_Str_I64("hello", 1);
  assert_equals_Str_Str("hello", actual);
  free(actual);

  // Test starting at position 2
  actual = mid$_Str_I64("hello", 2);
  assert_equals_Str_Str("ello", actual);
  free(actual);

  // Test starting at last character
  actual = mid$_Str_I64("hello", 5);
  assert_equals_Str_Str("o", actual);
  free(actual);

  // Test starting beyond string length
  actual = mid$_Str_I64("hello", 10);
  assert_equals_Str_Str("", actual);
  free(actual);

  // Test mid$_Str_I64_I64 - three argument version

  // Test with empty string
  actual = mid$_Str_I64_I64("", 1, 5);
  assert_equals_Str_Str("", actual);
  free(actual);

  // Test with zero length
  actual = mid$_Str_I64_I64("hello", 1, 0);
  assert_equals_Str_Str("", actual);
  free(actual);

  // Test normal case - middle 3 characters
  actual = mid$_Str_I64_I64("hello", 2, 3);
  assert_equals_Str_Str("ell", actual);
  free(actual);

  // Test from start with length
  actual = mid$_Str_I64_I64("hello", 1, 3);
  assert_equals_Str_Str("hel", actual);
  free(actual);

  // Test when length exceeds remaining string
  actual = mid$_Str_I64_I64("hello", 3, 10);
  assert_equals_Str_Str("llo", actual);
  free(actual);

  // Test single character extraction
  actual = mid$_Str_I64_I64("hello", 3, 1);
  assert_equals_Str_Str("l", actual);
  free(actual);

  // Test when start equals string length
  actual = mid$_Str_I64_I64("hello", 5, 2);
  assert_equals_Str_Str("o", actual);
  free(actual);

  // Test when start is beyond string length
  actual = mid$_Str_I64_I64("hello", 10, 5);
  assert_equals_Str_Str("", actual);
  free(actual);

  // Test with longer string
  actual = mid$_Str_I64_I64("QuickBASIC", 6, 5);
  assert_equals_Str_Str("BASIC", actual);
  free(actual);

  // Test entire string extraction
  actual = mid$_Str_I64_I64("hello", 1, 5);
  assert_equals_Str_Str("hello", actual);
  free(actual);

  // Test length exceeding string length from start
  actual = mid$_Str_I64_I64("hello", 1, 100);
  assert_equals_Str_Str("hello", actual);
  free(actual);
}
