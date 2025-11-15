/*
 * Copyright (C) 2019 Johan Dykstrom
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

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "rtrim.h"

char* rtrim$(const char* s) {
  int index = strlen(s) - 1;

  // Walk backwards from the end while we only see whitespace
  while ((index >= 0) && isspace(s[index])) {
    index--;
  }

  // index is now length of substring - 1 (for empty strings it is -1)
  char* result = malloc(index + 2);

  if (index != -1) {
    strncpy(result, s, index + 1);
  }

  result[index + 1] = 0;
  return result;
}
