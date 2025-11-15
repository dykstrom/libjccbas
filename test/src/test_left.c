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
#include "left.h"

int main(int argc, char *argv[]) {
  char* actual;

  // Test with empty string
  actual = left$("", 5);
  assert_equals_Str_Str("", actual);
  free(actual);

  // Test with zero length
  actual = left$("hello", 0);
  assert_equals_Str_Str("", actual);
  free(actual);

  // Test normal case - left 3 characters
  actual = left$("hello", 3);
  assert_equals_Str_Str("hel", actual);
  free(actual);

  // Test when n equals string length
  actual = left$("hello", 5);
  assert_equals_Str_Str("hello", actual);
  free(actual);

  // Test when n exceeds string length
  actual = left$("hello", 10);
  assert_equals_Str_Str("hello", actual);
  free(actual);

  // Test with single character
  actual = left$("hello", 1);
  assert_equals_Str_Str("h", actual);
  free(actual);

  // Test with longer string
  actual = left$("QuickBASIC", 5);
  assert_equals_Str_Str("Quick", actual);
  free(actual);
}
