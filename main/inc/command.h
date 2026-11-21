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

#ifndef JCCBAS_COMMAND_H
#define JCCBAS_COMMAND_H

// Returns the arguments after the program name, like COMMAND$ in QuickBASIC 4.5.
// The arguments are joined with single spaces, letters are converted to upper
// case, and leading blanks are removed. Returns an empty string if there are no
// arguments or if bas_init_command_line has not been called. This function
// allocates memory. It is the caller's responsibility to free this memory.
char *bas_command();

// Stores the command line for bas_command. The main function must call this
// function on every OS. A later call replaces the stored command line.
void bas_init_command_line(int argc, char *argv[]);

#endif /* JCCBAS_COMMAND_H */
