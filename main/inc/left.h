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

#ifndef LEFT$_H_
#define LEFT$_H_

#include <stdint.h>

// Returns a string containing the leftmost n characters of the given string.
// If n is greater than or equal to the length of the string, the entire string is returned.
// If n is zero, an empty string is returned.
// If n is negative, an "Illegal function call" error is raised and the program exits.
// This function allocates memory. It is the caller's responsibility to free this memory.
char *left$(const char *s, int64_t n);

#endif /* LEFT$_H_ */
