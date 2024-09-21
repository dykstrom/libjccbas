/*
 * Copyright (C) 2024 Johan Dykstrom
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

#include "stringd.h"

char *string$_I64(int64_t len, int64_t code) {
  if (len < 0 || code < 0 || code > 255) {
    printf("Error: Illegal function call: string$(%" PRId64 ", %" PRId64 ")", len, code);
    exit(1);
  }

  char *result = (char *) malloc(len + 1);
  memset(result, code, len);
  result[len] = 0;
  return result;
}

char *string$_Str(int64_t len, const char *s) {
  if (len < 0 || strlen(s) < 1) {
    printf("Error: Illegal function call: string$(%" PRId64 ", \"%s\")", len, s);
    exit(1);
  }

  char *result = (char *) malloc(len + 1);
  memset(result, s[0], len);
  result[len] = 0;
  return result;
}
