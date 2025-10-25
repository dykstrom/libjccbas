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

#include <math.h>
#include <stdlib.h>

#include "randomize_rnd.h"

// The last number returned
static double last_number = 0.0;

void randomize(double seed) {
  // Add 32768 to get a better range from interactive seed
  seed += 32768;
  // Multiply by 1000 to get a better range from timer seed
  seed *= 1000;
  srand(fabs(seed));
}

double rnd() {
  return rnd_F64(1.0);
}

double rnd_F64(double seed) {
  if (seed < 0.0) {
    randomize(seed);
  }

  if (seed != 0.0) {
    last_number = (double) rand() / ((double) RAND_MAX + 1.0);
  }

  return last_number;
}
