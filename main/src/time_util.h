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

#ifndef TIME_UTIL_H_
#define TIME_UTIL_H_

#include <time.h>

#ifndef _WIN32
// Unix-like time utility functions
// These provide Windows-like time functionality on Unix systems

// Get current system time and return milliseconds
// Fills the provided tm struct with local time and returns the millisecond component
unsigned _getsystime(struct tm *t);

#endif

#endif /* TIME_UTIL_H_ */
