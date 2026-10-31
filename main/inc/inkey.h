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

#ifndef JCCBAS_INKEY_H
#define JCCBAS_INKEY_H

// Returns a one- or two-byte string containing a character read from the
// keyboard. An empty string is returned if no character is waiting there.
// This function allocates memory. It is the caller's responsibility to
// free this memory.
//
// Note: The current version of this function can only handle "normal"
// keys, not function keys, arrow keys, etc.
char* bas_inkey();

#endif /* JCCBAS_INKEY_H */
