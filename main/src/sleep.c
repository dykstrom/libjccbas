/*
 * Copyright (C) 2023 Johan Dykstrom
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
#include <conio.h>
#include <Windows.h>
#else
#include "kbd_util.h"
#endif

#include "sleep.h"
#include "timer.h"

void sleep_F64(const double seconds) {
  // Empty keyboard buffer
  while (_kbhit()) {
    _getch();
  }

  if (seconds == 0.0) {
    // Loop until a key has been pressed
    while (!_kbhit()) {
      Sleep(100);
    }

  } else if (seconds < 0.05) {
    // If time is less than 50 millis, don't bother checking for keypress
    Sleep((long) (seconds * 1000.0));

  } else {
    const double start = timer();
    // Loop until the time is up, or a key has been pressed
    while (timer() - start < seconds) {
      Sleep(10);
      if (_kbhit()) {
        break;
      }
    }
  }
}
