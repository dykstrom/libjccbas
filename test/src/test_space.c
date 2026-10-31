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

#include <stdlib.h>

#include "assert.h"
#include "space.h"

int main(int argc, char *argv[]) {
  char *actual;

  actual = bas_space_i64(0);
  assert_equals_str_str("", actual);
  free(actual);

  actual = bas_space_i64(1);
  assert_equals_str_str(" ", actual);
  free(actual);

  actual = bas_space_i64(10);
  assert_equals_str_str("          ", actual);
  free(actual);
}
