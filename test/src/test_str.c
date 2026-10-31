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

#include <stdlib.h>

#include "assert.h"
#include "str.h"

int main(int argc, char *argv[]) {
  char* actual;

  // Integer version
  actual = bas_str_i64(0);
  assert_equals_str_str(" 0", actual);
  free(actual);

  actual = bas_str_i64(4711);
  assert_equals_str_str(" 4711", actual);
  free(actual);

  actual = bas_str_i64(-9000);
  assert_equals_str_str("-9000", actual);
  free(actual);

  // MAXINT is 2_147_483_647
  actual = bas_str_i64(2147483700);
  assert_equals_str_str(" 2147483700", actual);
  free(actual);

  // Float version
  actual = bas_str_f64(0.0);
  assert_equals_str_str(" 0.000000", actual);
  free(actual);

  actual = bas_str_f64(123456.123456);
  assert_equals_str_str(" 123456.123456", actual);
  free(actual);

  actual = bas_str_f64(-539.33);
  assert_equals_str_str("-539.330000", actual);
  free(actual);
}
