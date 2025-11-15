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

#include "left.h"

char* left$(const char* s, int64_t n) {
  if (n < 0) {
    printf("Error: Illegal function call: left$(\"%s\", %" PRId64 ")", s, n);
    exit(1);
  }

  if (n == 0) {
    char* result = malloc(1);
    result[0] = '\0';
    return result;
  }

  size_t len = strlen(s);
  size_t copy_len = (size_t)n < len ? (size_t)n : len;

  char* result = malloc(copy_len + 1);
  strncpy(result, s, copy_len);
  result[copy_len] = '\0';

  return result;
}
