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

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instr.h"

// Internal helper function that performs the actual substring search.
// Parameters:
//   s1: the string to search in
//   s2: the string to search for
//   start: 1-based start position (assumed to be >= 1)
// Returns the 1-based position of the first occurrence, or 0 if not found.
static int64_t instr_impl(const char* s1, const char* s2, int64_t start) {
  size_t len1 = strlen(s1);
  size_t len2 = strlen(s2);

  // If s2 is empty, return start position
  if (len2 == 0) {
    return start;
  }

  // If s1 is empty or start is beyond s1 length, return 0
  if (len1 == 0 || (size_t)start > len1) {
    return 0;
  }

  // Convert 1-based to 0-based index
  size_t start_index = (size_t)(start - 1);

  // Search for s2 in s1 starting from start_index
  const char* found = strstr(s1 + start_index, s2);

  if (found == NULL) {
    return 0;
  }

  // Calculate 1-based position
  return (int64_t)(found - s1) + 1;
}

int64_t instr_Str_Str(const char* s1, const char* s2) {
  return instr_impl(s1, s2, 1);
}

int64_t instr_I64_Str_Str(int64_t start, const char* s1, const char* s2) {
  if (start < 1) {
    printf("Error: Illegal function call: instr(%" PRId64 ", \"%s\", \"%s\")", start, s1, s2);
    exit(1);
  }

  return instr_impl(s1, s2, start);
}
