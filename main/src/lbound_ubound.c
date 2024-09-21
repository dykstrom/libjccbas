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

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "lbound_ubound.h"


/*

  From Arrays.md:

  Size of dimension N
  ...
  Size of dimension 2
  Size of dimension 1
  Number of dimensions
  Array element 0
  Array element 1
  ...
  
 */

// OPTION BASE value
static int64_t base = 0;

void option_base(int64_t new_base) {
  base = new_base;
}

int64_t lbound(const void* array) {
  return base;
}

int64_t lbound_I64(const void* array, int64_t dimension) {
  return base;
}

int64_t ubound(const void* array) {
  const int64_t *i64_array = array;
  // 1. Index -2 looks strange, but we know the size of the first dimension
  //    is stored two 64-bit memory cells before the start of the array
  // 2. Since the size of the dimension is stored in memory, we must subtract
  //    one, because ubound return the upper bound (inclusive)
  return i64_array[-2] - 1;
}

int64_t ubound_I64(const void* array, int64_t dimension) {
  const int64_t *i64_array = array;
  const int64_t num_dimensions = i64_array[-1];

  if (dimension < 1 || dimension > num_dimensions) {
    printf("Error: Illegal function call: ubound(*array*, %" PRId64 ")", dimension);
    exit(1);
  }
  
  return i64_array[-dimension - 1] - 1;
}
