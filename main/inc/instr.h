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

#ifndef INSTR_H_
#define INSTR_H_

#include <stdint.h>

// Searches for the first occurrence of s2 in s1 starting at position 1.
// Returns the position (1-based) of the first occurrence, or 0 if not found.
// If s1 is empty, returns 0.
// If s2 is empty, returns 1.
int64_t instr_Str_Str(const char *s1, const char *s2);

// Searches for the first occurrence of s2 in s1 starting at the specified position.
// Returns the position (1-based) of the first occurrence, or 0 if not found.
// If start is less than 1, an "Illegal function call" error is raised and the program exits.
// If start is greater than the length of s1, returns 0.
// If s1 is empty, returns 0.
// If s2 is empty, returns start (if valid).
int64_t instr_I64_Str_Str(int64_t start, const char *s1, const char *s2);

#endif /* INSTR_H_ */
