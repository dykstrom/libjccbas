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

#include "assert.h"
#include "instr.h"

int main(int argc, char *argv[]) {
  // Test instr_Str_Str - two argument version (search from position 1)

  // Test finding substring at the beginning
  assert_equals_I64_I64(1, instr_Str_Str("hello world", "hello"));

  // Test finding substring in the middle
  assert_equals_I64_I64(7, instr_Str_Str("hello world", "world"));

  // Test finding substring at the end
  assert_equals_I64_I64(9, instr_Str_Str("QuickBASIC", "IC"));

  // Test finding single character
  assert_equals_I64_I64(5, instr_Str_Str("hello", "o"));

  // Test substring not found
  assert_equals_I64_I64(0, instr_Str_Str("hello", "xyz"));

  // Test with empty string1
  assert_equals_I64_I64(0, instr_Str_Str("", "hello"));

  // Test with empty string2 (should return 1)
  assert_equals_I64_I64(1, instr_Str_Str("hello", ""));

  // Test with both strings empty
  assert_equals_I64_I64(1, instr_Str_Str("", ""));

  // Test finding first occurrence when multiple exist
  assert_equals_I64_I64(2, instr_Str_Str("banana", "a"));

  // Test case sensitivity
  assert_equals_I64_I64(0, instr_Str_Str("Hello", "hello"));

  // Test longer string2 than string1
  assert_equals_I64_I64(0, instr_Str_Str("hi", "hello"));

  // Test identical strings
  assert_equals_I64_I64(1, instr_Str_Str("test", "test"));

  // Test instr_I64_Str_Str - three argument version (search from specified position)

  // Test starting at position 1 (should match two-arg version)
  assert_equals_I64_I64(7, instr_I64_Str_Str(1, "hello world", "world"));

  // Test starting at position 2
  assert_equals_I64_I64(2, instr_I64_Str_Str(2, "banana", "a"));

  // Test starting after first occurrence to find second
  assert_equals_I64_I64(4, instr_I64_Str_Str(3, "banana", "a"));

  // Test starting after second occurrence to find third
  assert_equals_I64_I64(6, instr_I64_Str_Str(5, "banana", "a"));

  // Test starting beyond string length
  assert_equals_I64_I64(0, instr_I64_Str_Str(20, "hello", "o"));

  // Test starting at last character
  assert_equals_I64_I64(5, instr_I64_Str_Str(5, "hello", "o"));

  // Test starting at exact position where substring exists
  assert_equals_I64_I64(7, instr_I64_Str_Str(7, "hello world", "world"));

  // Test starting just after substring position
  assert_equals_I64_I64(0, instr_I64_Str_Str(8, "hello world", "world"));

  // Test with empty string2 and valid start position
  assert_equals_I64_I64(3, instr_I64_Str_Str(3, "hello", ""));

  // Test with empty string1
  assert_equals_I64_I64(0, instr_I64_Str_Str(1, "", "test"));

  // Test with both strings empty
  assert_equals_I64_I64(1, instr_I64_Str_Str(1, "", ""));

  // Test finding substring when start position is in the middle
  assert_equals_I64_I64(9, instr_I64_Str_Str(5, "QuickBASIC", "IC"));

  // Test not finding substring when start position is after its location
  assert_equals_I64_I64(0, instr_I64_Str_Str(2, "hello world", "hello"));

  // Test with start at string length
  assert_equals_I64_I64(0, instr_I64_Str_Str(5, "hello", "lo"));

  // Test with start at string length and single char match
  assert_equals_I64_I64(11, instr_I64_Str_Str(11, "hello world", "d"));

  return 0;
}
