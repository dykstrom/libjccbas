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

#include <stdlib.h>

#include "assert.h"
#include "mkd_cvd.h"
#include "randomize_rnd.h"
#include "timer.h"

int main(int argc, char *argv[]) {
  bas_randomize_f64(bas_timer());

  for (int i = 0; i < 10; i++) {
    double value = 1000.0 * bas_rnd() * bas_rnd() - 1000.0 * bas_rnd() * bas_rnd();
    char *s = bas_mkd_f64(value);
    assert_equals_f64_f64_f64(value, bas_cvd_str(s), 0.00001);
    free(s);
  }
}
