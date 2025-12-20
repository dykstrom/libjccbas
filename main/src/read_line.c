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
#include <stdio.h>

#include "read_line.h"

char* read_line(void) {
  int length = 0;
  int size = 64;
  char* buffer = malloc(size);
  
  if (buffer == NULL) {
    return NULL;
  }

  while (1) {
    int c = getchar();
    if (c == '\n' || c == EOF) {
      break;
    }
    buffer[length++] = c;
    
    if (length >= size) {
      size *= 2;
      char* new_buffer = realloc(buffer, size);
      if (new_buffer == NULL) {
        free(buffer);
        return NULL;
      }
      buffer = new_buffer;
    }
  }
  buffer[length] = 0;
  
  return buffer;
}
