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

#ifndef STRING$_H_
#define STRING$_H_

#include <stdint.h>

// Returns a string whose characters all have a given ASCII code. This function
// allocates memory. It is the caller's responsibility to free this memory.
char *string$_I64(int64_t len, int64_t code);

// Returns a string whose characters are all the first character of a string
// expression. This function allocates memory. It is the caller's responsibility
// to free this memory.
char *string$_Str(int64_t len, const char *s);

#endif /* STRING$_H_ */
