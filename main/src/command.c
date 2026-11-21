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

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"

// The command line returned by bas_command, or NULL if not initialized
static char *cached_command_line = NULL;

static char *empty_string() {
  char *result = malloc(1);
  result[0] = '\0';
  return result;
}

void bas_init_command_line(int argc, char *argv[]) {
  free(cached_command_line);

  if (argc <= 1 || argv == NULL) {
    cached_command_line = empty_string();
    return;
  }

  // Skip argv[0], the program name, like QuickBASIC does
  size_t total_len = 0;
  for (int i = 1; i < argc; i++) {
    total_len += strlen(argv[i]) + 1; // Argument plus a space or the terminator
  }

  char *line = malloc(total_len);
  char *p = line;
  for (int i = 1; i < argc; i++) {
    if (i > 1) {
      *p++ = ' ';
    }
    size_t len = strlen(argv[i]);
    memcpy(p, argv[i], len);
    p += len;
  }
  *p = '\0';

  // QuickBASIC converts all letters to upper case
  for (p = line; *p != '\0'; p++) {
    *p = (char) toupper((unsigned char) *p);
  }

  // QuickBASIC removes all leading blanks
  size_t skip = strspn(line, " \t");
  if (skip > 0) {
    memmove(line, line + skip, strlen(line + skip) + 1);
  }

  cached_command_line = line;
}

char *bas_command() {
  if (cached_command_line == NULL) {
    return empty_string();
  }
  char *result = malloc(strlen(cached_command_line) + 1);
  return strcpy(result, cached_command_line);
}
