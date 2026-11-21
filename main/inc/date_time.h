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

#ifndef JCCBAS_DATE_TIME_H
#define JCCBAS_DATE_TIME_H

// Returns the current date as a string with format mm-dd-yyyy. This function
// allocates memory. It is the caller's responsibility to free this memory.
char* bas_date();

// Returns the current time as a string with format hh:mm:ss. This function
// allocates memory. It is the caller's responsibility to free this memory.
char* bas_time();

#endif /* JCCBAS_DATE_TIME_H */
