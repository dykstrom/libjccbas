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

#ifndef SCREEN_UTIL_H_
#define SCREEN_UTIL_H_

#include <stdbool.h>

// Get current cursor position (1-based coordinates)
// Returns true on success, false on failure
// On success, *row and *col are set to the cursor position
// On failure, *row and *col are set to 1
bool get_cursor_position(int *row, int *col);

#endif /* SCREEN_UTIL_H_ */
