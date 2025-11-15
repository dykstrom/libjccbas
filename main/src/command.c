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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <processenv.h>
#endif

#include "command.h"

#ifndef _WIN32
// Static storage for command line on Unix-like systems
static char *cached_command_line = NULL;

// This function should be called by the main program to initialize the command line
void init_command_line(int argc, char *argv[]) {
  if (cached_command_line != NULL) {
    return; // Already initialized
  }

  if (argc == 0 || argv == NULL) {
    cached_command_line = malloc(1);
    cached_command_line[0] = '\0';
    return;
  }

  // Calculate total length needed
  size_t total_len = 0;
  for (int i = 0; i < argc; i++) {
    total_len += strlen(argv[i]);
    if (i < argc - 1) {
      total_len++; // Space between arguments
    }
  }

  // Allocate and build the command line string
  cached_command_line = malloc(total_len + 1);
  cached_command_line[0] = '\0';

  for (int i = 0; i < argc; i++) {
    strcat(cached_command_line, argv[i]);
    if (i < argc - 1) {
      strcat(cached_command_line, " ");
    }
  }
}
#endif

char *command$() {
#ifdef _WIN32
  const char *cmd = GetCommandLineA();
  char *result = malloc(strlen(cmd) + 1);
  return strcpy(result, cmd);
#else
  // On Unix-like systems, return the cached command line
  // Assumes init_command_line has been called
  if (cached_command_line == NULL) {
    cached_command_line = malloc(1);
    cached_command_line[0] = '\0';
  }

  // Return a copy of the cached command line
  char *result = malloc(strlen(cached_command_line) + 1);
  return strcpy(result, cached_command_line);
#endif
}
