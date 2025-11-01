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

#ifndef KBD_UTIL_H_
#define KBD_UTIL_H_

#ifndef _WIN32
// Unix-like keyboard utility functions
// These provide conio.h-like functionality on Unix systems

// Check if a key has been pressed (non-blocking)
int _kbhit(void);

// Get a character from keyboard without echo
int _getch(void);

// Sleep for specified milliseconds
void Sleep(unsigned int milliseconds);

#endif

#endif /* KBD_UTIL_H_ */
