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

#ifdef _WIN32
#include <windows.h>
#else
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#endif

#include "screen_util.h"

bool get_cursor_position(int *row, int *col) {
  // Set default values
  *row = 1;
  *col = 1;

#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
    *row = csbi.dwCursorPosition.Y + 1;
    *col = csbi.dwCursorPosition.X + 1;
    return true;
  }
  return false;
#else
  // Check if stdout is a terminal
  if (!isatty(STDOUT_FILENO)) {
    return false;
  }

  struct termios oldt, newt;

  // Save current terminal settings
  if (tcgetattr(STDIN_FILENO, &oldt) != 0) {
    return false;
  }

  // Set terminal to raw mode for reading response
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  // Query cursor position using ANSI escape sequence
  // Send: ESC[6n (Device Status Report)
  // Response: ESC[row;colR
  printf("\033[6n");
  fflush(stdout);

  char buf[32];
  int i = 0;
  char ch;

  // Read response
  while (i < sizeof(buf) - 1) {
    if (read(STDIN_FILENO, &ch, 1) != 1) {
      break;
    }
    buf[i++] = ch;
    if (ch == 'R') {
      break;
    }
  }
  buf[i] = '\0';

  // Restore terminal settings
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  // Parse response: ESC[row;colR
  if (i > 2 && buf[0] == '\033' && buf[1] == '[') {
    if (sscanf(buf + 2, "%d;%d", row, col) == 2) {
      return true;
    }
  }

  // Reset to defaults on parse failure
  *row = 1;
  *col = 1;
  return false;
#endif
}
