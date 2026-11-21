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
  // Test bas_instr_str_str - two argument version (search from position 1)

  // Test finding substring at the beginning
  assert_equals_i64_i64(1, bas_instr_str_str("hello world", "hello"));

  // Test finding substring in the middle
  assert_equals_i64_i64(7, bas_instr_str_str("hello world", "world"));

  // Test finding substring at the end
  assert_equals_i64_i64(9, bas_instr_str_str("QuickBASIC", "IC"));

  // Test finding single character
  assert_equals_i64_i64(5, bas_instr_str_str("hello", "o"));

  // Test substring not found
  assert_equals_i64_i64(0, bas_instr_str_str("hello", "xyz"));

  // Test with empty string1
  assert_equals_i64_i64(0, bas_instr_str_str("", "hello"));

  // Test with empty string2 (should return 1)
  assert_equals_i64_i64(1, bas_instr_str_str("hello", ""));

  // Test with both strings empty
  assert_equals_i64_i64(1, bas_instr_str_str("", ""));

  // Test finding first occurrence when multiple exist
  assert_equals_i64_i64(2, bas_instr_str_str("banana", "a"));

  // Test case sensitivity
  assert_equals_i64_i64(0, bas_instr_str_str("Hello", "hello"));

  // Test longer string2 than string1
  assert_equals_i64_i64(0, bas_instr_str_str("hi", "hello"));

  // Test identical strings
  assert_equals_i64_i64(1, bas_instr_str_str("test", "test"));

  // Test bas_instr_i64_str_str - three argument version (search from specified position)

  // Test starting at position 1 (should match two-arg version)
  assert_equals_i64_i64(7, bas_instr_i64_str_str(1, "hello world", "world"));

  // Test starting at position 2
  assert_equals_i64_i64(2, bas_instr_i64_str_str(2, "banana", "a"));

  // Test starting after first occurrence to find second
  assert_equals_i64_i64(4, bas_instr_i64_str_str(3, "banana", "a"));

  // Test starting after second occurrence to find third
  assert_equals_i64_i64(6, bas_instr_i64_str_str(5, "banana", "a"));

  // Test starting beyond string length
  assert_equals_i64_i64(0, bas_instr_i64_str_str(20, "hello", "o"));

  // Test starting at last character
  assert_equals_i64_i64(5, bas_instr_i64_str_str(5, "hello", "o"));

  // Test starting at exact position where substring exists
  assert_equals_i64_i64(7, bas_instr_i64_str_str(7, "hello world", "world"));

  // Test starting just after substring position
  assert_equals_i64_i64(0, bas_instr_i64_str_str(8, "hello world", "world"));

  // Test with empty string2 and valid start position
  assert_equals_i64_i64(3, bas_instr_i64_str_str(3, "hello", ""));

  // Test with empty string1
  assert_equals_i64_i64(0, bas_instr_i64_str_str(1, "", "test"));

  // Test with both strings empty
  assert_equals_i64_i64(1, bas_instr_i64_str_str(1, "", ""));

  // Test finding substring when start position is in the middle
  assert_equals_i64_i64(9, bas_instr_i64_str_str(5, "QuickBASIC", "IC"));

  // Test not finding substring when start position is after its location
  assert_equals_i64_i64(0, bas_instr_i64_str_str(2, "hello world", "hello"));

  // Test with start at string length
  assert_equals_i64_i64(0, bas_instr_i64_str_str(5, "hello", "lo"));

  // Test with start at string length and single char match
  assert_equals_i64_i64(11, bas_instr_i64_str_str(11, "hello world", "d"));

  return 0;
}
