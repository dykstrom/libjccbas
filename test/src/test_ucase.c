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
  actual = bas_ucase_str("");
  assert_equals_str_str("", actual);
  free(actual);

  // Test with lowercase string
  actual = bas_ucase_str("hello");
  assert_equals_str_str("HELLO", actual);
  free(actual);

  // Test with uppercase string
  actual = bas_ucase_str("HELLO");
  assert_equals_str_str("HELLO", actual);
  free(actual);

  // Test with mixed case string
  actual = bas_ucase_str("Hello World");
  assert_equals_str_str("HELLO WORLD", actual);
  free(actual);

  // Test with numbers and symbols
  actual = bas_ucase_str("abc123xyz!@#");
  assert_equals_str_str("ABC123XYZ!@#", actual);
  free(actual);

  // Test with all lowercase
  actual = bas_ucase_str("quickbasic");
  assert_equals_str_str("QUICKBASIC", actual);
  free(actual);

  // Test with single character
  actual = bas_ucase_str("a");
  assert_equals_str_str("A", actual);
  free(actual);
}
