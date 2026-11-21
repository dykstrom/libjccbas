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

// Converts a 24-bit seed to the number RND returns for that seed
#define SEED_TO_RND(seed) ((seed) / 16777216.0)

// Expected values below come from the QuickBASIC 4.5 algorithm. The golden
// tests must run first, and in order, because they depend on the initial seed.

void rndZeroShouldReturnInitialSeed() {
  assert_equals_f64_f64(SEED_TO_RND(327680), bas_rnd_f64(0.0));
}

void rndShouldReturnQuickBasicSequence() {
  assert_equals_f64_f64(SEED_TO_RND(11837123), bas_rnd());
  assert_equals_f64_f64(SEED_TO_RND(8949370), bas_rnd());
  assert_equals_f64_f64(SEED_TO_RND(9722709), bas_rnd());
  assert_equals_f64_f64(SEED_TO_RND(4858052), bas_rnd());
  assert_equals_f64_f64(SEED_TO_RND(5065847), bas_rnd());
}

void rndNegativeShouldReturnQuickBasicSequence() {
  assert_equals_f64_f64(SEED_TO_RND(3758214), bas_rnd_f64(-1.0));
  assert_equals_f64_f64(SEED_TO_RND(601393), bas_rnd());
  assert_equals_f64_f64(SEED_TO_RND(1448752), bas_rnd());
}

void randomizeShouldReturnQuickBasicSequence() {
  bas_rnd_f64(-1.0);
  bas_randomize_f64(10);
  assert_equals_f64_f64(SEED_TO_RND(2410801), bas_rnd());
  assert_equals_f64_f64(SEED_TO_RND(3360560), bas_rnd());
  assert_equals_f64_f64(SEED_TO_RND(8554803), bas_rnd());

  bas_rnd_f64(-1.0);
  bas_randomize_f64(-5);
  assert_equals_f64_f64(SEED_TO_RND(6617393), bas_rnd());
  assert_equals_f64_f64(SEED_TO_RND(3323696), bas_rnd());
  assert_equals_f64_f64(SEED_TO_RND(4471091), bas_rnd());

  bas_rnd_f64(-1.0);
  bas_randomize_f64(1234.5);
  assert_equals_f64_f64(SEED_TO_RND(4291633), bas_rnd());
  assert_equals_f64_f64(SEED_TO_RND(6893104), bas_rnd());
  assert_equals_f64_f64(SEED_TO_RND(3986483), bas_rnd());

  assert_equals_f64_f64(SEED_TO_RND(9870467), bas_rnd_f64(-3.5));
}

void randomizeTwiceShouldKeepLowByteOfSeed() {
  bas_randomize_f64(10);
  assert_equals_f64_f64(SEED_TO_RND(2948410), bas_rnd());
  assert_equals_f64_f64(SEED_TO_RND(13766421), bas_rnd());
  assert_equals_f64_f64(SEED_TO_RND(1049988), bas_rnd());

  bas_randomize_f64(10);
  assert_equals_f64_f64(SEED_TO_RND(2769207), bas_rnd());
  assert_equals_f64_f64(SEED_TO_RND(15890206), bas_rnd());
  assert_equals_f64_f64(SEED_TO_RND(3551593), bas_rnd());
}

void randomNumbersShouldBeRandom() {
  bas_randomize_f64(bas_timer());
  double a = bas_rnd();
  double b = bas_rnd();
  double c = bas_rnd();
  assert_not_equals_f64_f64(a, b);
  assert_not_equals_f64_f64(a, c);
  assert_not_equals_f64_f64(b, c);
}

void sameSeedShouldGiveSameNumbers() {
  static double random_numbers1[MAX_NUMBERS];
  static double random_numbers2[MAX_NUMBERS];

  bas_rnd_f64(-1.0);
  bas_randomize_f64(10);
  for (int i = 0; i < MAX_NUMBERS; i++) {
    random_numbers1[i] = bas_rnd();
  }

  bas_rnd_f64(-1.0);
  bas_randomize_f64(10);
  for (int i = 0; i < MAX_NUMBERS; i++) {
    random_numbers2[i] = bas_rnd();
  }

  assert_array_equals_f64_f64(random_numbers1, random_numbers2, MAX_NUMBERS);
}

void differentSeedShouldGiveDifferentNumbers() {
  static double random_numbers1[MAX_NUMBERS];
  static double random_numbers2[MAX_NUMBERS];

  bas_randomize_f64(10);
  for (int i = 0; i < MAX_NUMBERS; i++) {
    random_numbers1[i] = bas_rnd();
  }

  bas_randomize_f64(20);
  for (int i = 0; i < MAX_NUMBERS; i++) {
    random_numbers2[i] = bas_rnd();
  }

  assert_array_not_equals_f64_f64(random_numbers1, random_numbers2, MAX_NUMBERS);
}

void numbersShouldBeEquallyProbable() {
  int bound = 7;
  int count[7] = {0};
  
  bas_randomize_f64(bas_timer());
  for (int i = 0; i < MAX_NUMBERS; i++) {
    count[(int) (bas_rnd() * bound)]++;
  }

  double expected = (double) MAX_NUMBERS / bound;
  for (int i = 0; i < bound; i++) {
    assert_equals_f64_f64_f64(expected, count[i], DELTA);
  }
}

void seedEqZeroShouldReturnLastNumber() {
  bas_randomize_f64(bas_timer());
  double expected = bas_rnd();
  assert_equals_f64_f64(expected, bas_rnd_f64(0.0));
  assert_equals_f64_f64(expected, bas_rnd_f64(0.0));
  assert_equals_f64_f64(expected, bas_rnd_f64(0.0));
}

void seedGtZeroShouldReturnNextNumber() {
  bas_randomize_f64(bas_timer());
  double unexpected = bas_rnd();
  assert_not_equals_f64_f64(unexpected, bas_rnd_f64(1.0));
}

void seedLtZeroShouldReseedRNG() {
  bas_randomize_f64(bas_timer());
  bas_rnd();
  assert_equals_f64_f64(SEED_TO_RND(3758214), bas_rnd_f64(-1.0));
  bas_rnd();
  assert_equals_f64_f64(SEED_TO_RND(3758214), bas_rnd_f64(-1.0));
}

void allNumbersShouldBeInValidRange() {
  bas_randomize_f64(bas_timer());
  bool all_in_range = true;
  for (int i = 0; i < MAX_NUMBERS; i++) {
    double value = bas_rnd();
    if (value < 0.0 || value >= 1.0) {
      all_in_range = false;
      break;
    }
  }
  assert_true_bool(all_in_range);
}

int main(int argc, char *argv[]) {
  rndZeroShouldReturnInitialSeed();
  rndShouldReturnQuickBasicSequence();
  rndNegativeShouldReturnQuickBasicSequence();
  randomizeShouldReturnQuickBasicSequence();
  randomizeTwiceShouldKeepLowByteOfSeed();
  randomNumbersShouldBeRandom();
  sameSeedShouldGiveSameNumbers();
  differentSeedShouldGiveDifferentNumbers();
  numbersShouldBeEquallyProbable();
  seedEqZeroShouldReturnLastNumber();
  seedGtZeroShouldReturnNextNumber();
  seedLtZeroShouldReseedRNG();
  allNumbersShouldBeInValidRange();
}
