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
#include "add.h"

int main(int argc, char *argv[]) {
  char* actual;

  actual = add_Str_Str("", "");
  assert_equals_Str_Str("", actual);
  free(actual);

  actual = add_Str_Str("abc", "");
  assert_equals_Str_Str("abc", actual);
  free(actual);

  actual = add_Str_Str("", "def");
  assert_equals_Str_Str("def", actual);
  free(actual);

  actual = add_Str_Str("abc", "def");
  assert_equals_Str_Str("abcdef", actual);
  free(actual);

  actual = add_Str_Str("Hello, ", "world!");
  assert_equals_Str_Str("Hello, world!", actual);
  free(actual);

  actual = add_Str_Str("123", "456");
  assert_equals_Str_Str("123456", actual);
  free(actual);

  actual = add_Str_Str("The quick brown fox ", "jumps over the lazy dog");
  assert_equals_Str_Str("The quick brown fox jumps over the lazy dog", actual);
  free(actual);
}
