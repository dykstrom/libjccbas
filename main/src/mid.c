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

#include "mid.h"

// Internal helper function that performs the actual substring extraction.
// Parameters:
//   s: the string to extract from
//   start: 1-based start position (assumed to be >= 1)
//   length: number of characters to extract, or -1 to extract to end of string
// Returns a newly allocated string containing the substring.
static char* mid$_impl(const char* s, int64_t start, int64_t length) {
  size_t len = strlen(s);

  // If start is beyond the string length, return empty string
  if ((size_t)start > len) {
    char* result = malloc(1);
    result[0] = '\0';
    return result;
  }

  // Convert 1-based to 0-based index
  size_t start_index = (size_t)(start - 1);
  size_t remaining = len - start_index;

  // Calculate copy length
  size_t copy_len;
  if (length < 0) {
    // Extract to end of string
    copy_len = remaining;
  } else if ((size_t)length < remaining) {
    copy_len = (size_t)length;
  } else {
    copy_len = remaining;
  }

  char* result = malloc(copy_len + 1);
  if (copy_len > 0) {
    strncpy(result, s + start_index, copy_len);
  }
  result[copy_len] = '\0';
  return result;
}

char* mid$_Str_I64(const char* s, int64_t start) {
  if (start < 1) {
    printf("Error: Illegal function call: mid$(\"%s\", %" PRId64 ")", s, start);
    exit(1);
  }

  return mid$_impl(s, start, -1);
}

char* mid$_Str_I64_I64(const char* s, int64_t start, int64_t length) {
  if (start < 1) {
    printf("Error: Illegal function call: mid$(\"%s\", %" PRId64 ", %" PRId64 ")", s, start, length);
    exit(1);
  }

  if (length < 0) {
    printf("Error: Illegal function call: mid$(\"%s\", %" PRId64 ", %" PRId64 ")", s, start, length);
    exit(1);
  }

  if (length == 0) {
    char* result = malloc(1);
    result[0] = '\0';
    return result;
  }

  return mid$_impl(s, start, length);
}
