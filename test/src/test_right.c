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
#include "right.h"

int main(int argc, char *argv[]) {
  char* actual;

  // Test with empty string
  actual = right$("", 5);
  assert_equals_Str_Str("", actual);
  free(actual);

  // Test with zero length
  actual = right$("hello", 0);
  assert_equals_Str_Str("", actual);
  free(actual);

  // Test normal case - right 3 characters
  actual = right$("hello", 3);
  assert_equals_Str_Str("llo", actual);
  free(actual);

  // Test when n equals string length
  actual = right$("hello", 5);
  assert_equals_Str_Str("hello", actual);
  free(actual);

  // Test when n exceeds string length
  actual = right$("hello", 10);
  assert_equals_Str_Str("hello", actual);
  free(actual);

  // Test with single character
  actual = right$("hello", 1);
  assert_equals_Str_Str("o", actual);
  free(actual);

  // Test with longer string
  actual = right$("QuickBASIC", 5);
  assert_equals_Str_Str("BASIC", actual);
  free(actual);
}
