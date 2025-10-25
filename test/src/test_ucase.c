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
#include "ucase.h"

int main(int argc, char *argv[]) {
  char* actual;

  // Test with empty string
  actual = ucase$("");
  assert_equals_Str_Str("", actual);
  free(actual);

  // Test with lowercase string
  actual = ucase$("hello");
  assert_equals_Str_Str("HELLO", actual);
  free(actual);

  // Test with uppercase string
  actual = ucase$("HELLO");
  assert_equals_Str_Str("HELLO", actual);
  free(actual);

  // Test with mixed case string
  actual = ucase$("Hello World");
  assert_equals_Str_Str("HELLO WORLD", actual);
  free(actual);

  // Test with numbers and symbols
  actual = ucase$("abc123xyz!@#");
  assert_equals_Str_Str("ABC123XYZ!@#", actual);
  free(actual);

  // Test with all lowercase
  actual = ucase$("quickbasic");
  assert_equals_Str_Str("QUICKBASIC", actual);
  free(actual);

  // Test with single character
  actual = ucase$("a");
  assert_equals_Str_Str("A", actual);
  free(actual);
}
