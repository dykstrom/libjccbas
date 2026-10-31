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

#ifndef JCCBAS_LTRIM_H
#define JCCBAS_LTRIM_H

// Returns a copy of the given string, with leading whitespace removed.
// This function allocates memory. It is the caller's responsibility to
// free this memory.
char* bas_ltrim_str(const char* s);

#endif /* JCCBAS_LTRIM_H */
