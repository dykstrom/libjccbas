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

#include "assert.h"
#include "randomize_rnd.h"
#include "timer.h"

#define MAX_NUMBERS 100000
#define DELTA (MAX_NUMBERS / 100.0)

void randomNumbersShouldBeRandom() {
  randomize(timer());
  double a = rnd();
  double b = rnd();
  double c = rnd();
  assert_not_equals_F64_F64(a, b);
  assert_not_equals_F64_F64(a, c);
  assert_not_equals_F64_F64(b, c);
}

void sameSeedShouldGiveSameNumbers() {
  double random_numbers1[MAX_NUMBERS];
  double random_numbers2[MAX_NUMBERS];

  randomize(10);
  for (int i = 0; i < MAX_NUMBERS; i++) {
    random_numbers1[i] = rnd();
  }

  randomize(10);
  for (int i = 0; i < MAX_NUMBERS; i++) {
    random_numbers2[i] = rnd();
  }

  assert_array_equals_F64_F64(random_numbers1, random_numbers2, MAX_NUMBERS);
}

void differentSeedShouldGiveDifferentNumbers() {
  double random_numbers1[MAX_NUMBERS];
  double random_numbers2[MAX_NUMBERS];

  randomize(10);
  for (int i = 0; i < MAX_NUMBERS; i++) {
    random_numbers1[i] = rnd();
  }

  randomize(20);
  for (int i = 0; i < MAX_NUMBERS; i++) {
    random_numbers2[i] = rnd();
  }

  assert_array_not_equals_F64_F64(random_numbers1, random_numbers2, MAX_NUMBERS);
}

void numbersShouldBeEquallyProbable() {
  int bound = 7;
  int count[7] = {0};
  
  randomize(timer());
  for (int i = 0; i < MAX_NUMBERS; i++) {
    count[(int) (rnd() * bound)]++;
  }

  double expected = (double) MAX_NUMBERS / bound;
  for (int i = 0; i < bound; i++) {
    assert_equals_F64_F64_F64(expected, count[i], DELTA);
  }
}

void seedEqZeroShouldReturnLastNumber() {
  randomize(timer());
  double expected = rnd();
  assert_equals_F64_F64(expected, rnd_F64(0.0));
  assert_equals_F64_F64(expected, rnd_F64(0.0));
  assert_equals_F64_F64(expected, rnd_F64(0.0));
}

void seedGtZeroShouldReturnNextNumber() {
  randomize(timer());
  double unexpected = rnd();
  assert_not_equals_F64_F64(unexpected, rnd_F64(1.0));
}

void seedLtZeroShouldReseedRNG() {
  randomize(-1.0);
  double expected = rnd();
  assert_equals_F64_F64(expected, rnd_F64(-1.0));
}

void allNumbersShouldBeInValidRange() {
  randomize(timer());
  bool all_in_range = true;
  for (int i = 0; i < MAX_NUMBERS; i++) {
    double value = rnd();
    if (value < 0.0 || value >= 1.0) {
      all_in_range = false;
      break;
    }
  }
  assert_true_Bool(all_in_range);
}

int main(int argc, char *argv[]) {
  randomNumbersShouldBeRandom();
  sameSeedShouldGiveSameNumbers();
  differentSeedShouldGiveDifferentNumbers();
  numbersShouldBeEquallyProbable();
  seedEqZeroShouldReturnLastNumber();
  seedGtZeroShouldReturnNextNumber();
  seedLtZeroShouldReseedRNG();
  allNumbersShouldBeInValidRange();
}
