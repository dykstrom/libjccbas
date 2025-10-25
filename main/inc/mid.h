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

#ifndef MID$_H_
#define MID$_H_

#include <stdint.h>

// Returns a substring of the given string starting at the specified position (1-based).
// If start is greater than the length of the string, an empty string is returned.
// If start is less than 1, an "Illegal function call" error is raised and the program exits.
// This function allocates memory. It is the caller's responsibility to free this memory.
char *mid$_Str_I64(const char *s, int64_t start);

// Returns a substring of the given string starting at the specified position (1-based)
// with the specified length.
// If start is greater than the length of the string, an empty string is returned.
// If start is less than 1, an "Illegal function call" error is raised and the program exits.
// If length is negative, an "Illegal function call" error is raised and the program exits.
// If length is zero, an empty string is returned.
// If start + length exceeds the string length, returns from start to end of string.
// This function allocates memory. It is the caller's responsibility to free this memory.
char *mid$_Str_I64_I64(const char *s, int64_t start, int64_t length);

#endif /* MID$_H_ */
