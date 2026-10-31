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

#ifndef JCCBAS_MKI_CVI_H
#define JCCBAS_MKI_CVI_H

#include <stdint.h>

// Converts an integer to a two-byte (actually eight-byte) string.
// Note that the returned string may contain null characters. For
// example, the call bas_mki_i64(0) will return a string that contains
// eight null characters, followed by the ending null character.
// This function allocates memory. It is the caller's responsibility
// to free this memory.
char* bas_mki_i64(int64_t number);

// Convert a string containing a numeric value to an integer.
// This function is the inverse of bas_mki_i64.
int64_t bas_cvi_str(const char* s);

#endif /* JCCBAS_MKI_CVI_H */
