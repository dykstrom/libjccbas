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
  assert_equals_F64_F64(SEED_TO_RND(327680), rnd_F64(0.0));
}

void rndShouldReturnQuickBasicSequence() {
  assert_equals_F64_F64(SEED_TO_RND(11837123), rnd());
  assert_equals_F64_F64(SEED_TO_RND(8949370), rnd());
  assert_equals_F64_F64(SEED_TO_RND(9722709), rnd());
  assert_equals_F64_F64(SEED_TO_RND(4858052), rnd());
  assert_equals_F64_F64(SEED_TO_RND(5065847), rnd());
}

void rndNegativeShouldReturnQuickBasicSequence() {
  assert_equals_F64_F64(SEED_TO_RND(3758214), rnd_F64(-1.0));
  assert_equals_F64_F64(SEED_TO_RND(601393), rnd());
  assert_equals_F64_F64(SEED_TO_RND(1448752), rnd());
}

void randomizeShouldReturnQuickBasicSequence() {
  rnd_F64(-1.0);
  randomize(10);
  assert_equals_F64_F64(SEED_TO_RND(2410801), rnd());
  assert_equals_F64_F64(SEED_TO_RND(3360560), rnd());
  assert_equals_F64_F64(SEED_TO_RND(8554803), rnd());

  rnd_F64(-1.0);
  randomize(-5);
  assert_equals_F64_F64(SEED_TO_RND(6617393), rnd());
  assert_equals_F64_F64(SEED_TO_RND(3323696), rnd());
  assert_equals_F64_F64(SEED_TO_RND(4471091), rnd());

  rnd_F64(-1.0);
  randomize(1234.5);
  assert_equals_F64_F64(SEED_TO_RND(4291633), rnd());
  assert_equals_F64_F64(SEED_TO_RND(6893104), rnd());
  assert_equals_F64_F64(SEED_TO_RND(3986483), rnd());

  assert_equals_F64_F64(SEED_TO_RND(9870467), rnd_F64(-3.5));
}

void randomizeTwiceShouldKeepLowByteOfSeed() {
  randomize(10);
  assert_equals_F64_F64(SEED_TO_RND(2948410), rnd());
  assert_equals_F64_F64(SEED_TO_RND(13766421), rnd());
  assert_equals_F64_F64(SEED_TO_RND(1049988), rnd());

  randomize(10);
  assert_equals_F64_F64(SEED_TO_RND(2769207), rnd());
  assert_equals_F64_F64(SEED_TO_RND(15890206), rnd());
  assert_equals_F64_F64(SEED_TO_RND(3551593), rnd());
}

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
  static double random_numbers1[MAX_NUMBERS];
  static double random_numbers2[MAX_NUMBERS];

  rnd_F64(-1.0);
  randomize(10);
  for (int i = 0; i < MAX_NUMBERS; i++) {
    random_numbers1[i] = rnd();
  }

  rnd_F64(-1.0);
  randomize(10);
  for (int i = 0; i < MAX_NUMBERS; i++) {
    random_numbers2[i] = rnd();
  }

  assert_array_equals_F64_F64(random_numbers1, random_numbers2, MAX_NUMBERS);
}

void differentSeedShouldGiveDifferentNumbers() {
  static double random_numbers1[MAX_NUMBERS];
  static double random_numbers2[MAX_NUMBERS];

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
  randomize(timer());
  rnd();
  assert_equals_F64_F64(SEED_TO_RND(3758214), rnd_F64(-1.0));
  rnd();
  assert_equals_F64_F64(SEED_TO_RND(3758214), rnd_F64(-1.0));
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
