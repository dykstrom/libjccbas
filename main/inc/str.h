/*
 * Copyright (C) 2019 Johan Dykstrom
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

#ifndef JCCBAS_STR_H
#define JCCBAS_STR_H

#include <stdint.h>

// Returns a string representation of the given integer value. This function
// allocates memory. It is the caller's responsibility to free this memory.
char* bas_str_i64(int64_t number);

// Returns a string representation of the given floating point value. This
// function allocates memory. It is the caller's responsibility to free this
// memory.
char* bas_str_f64(double number);

#endif /* JCCBAS_STR_H */
