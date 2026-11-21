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

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "str.h"

char* bas_str_i64(int64_t number) {
  int length = snprintf(0, 0, "% " PRId64, number);
  char *result = (char *) malloc(length + 1);
  snprintf(result, length + 1, "% " PRId64, number);
  return result;
}

char* bas_str_f64(double number) {
  int length = snprintf(0, 0, "% f", number);
  char *result = (char *) malloc(length + 1);
  snprintf(result, length + 1, "% f", number);
  return result;
}
