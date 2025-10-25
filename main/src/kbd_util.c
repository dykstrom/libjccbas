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

#include "kbd_util.h"

#ifndef _WIN32

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>

// Unix-like implementation of _kbhit() - check if a key has been pressed
int _kbhit(void) {
  // Check if stdin is a terminal
  if (!isatty(STDIN_FILENO)) {
    // Not a terminal - use select() directly without terminal manipulation
    struct timeval tv = {0, 0};
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    return select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0;
  }

  struct termios oldt, newt;

  if (tcgetattr(STDIN_FILENO, &oldt) != 0) {
    return 0;  // Failed to get terminal attributes
  }

  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  struct timeval tv = {0, 0};
  fd_set readfds;
  FD_ZERO(&readfds);
  FD_SET(STDIN_FILENO, &readfds);

  int result = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  return result > 0;
}

// Unix-like implementation of _getch() - get character without echo
int _getch(void) {
  struct termios oldt, newt;
  int ch;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  return ch;
}

// Unix-like implementation of Sleep() - sleep for milliseconds
void Sleep(unsigned int milliseconds) {
  usleep(milliseconds * 1000);
}

#endif