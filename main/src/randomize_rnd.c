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

#include <stdint.h>
#include <string.h>

#include "randomize_rnd.h"

// The algorithm is the 24-bit linear congruential generator used by
// QuickBASIC 4.5: seed = (seed * 16598013 + 12820163) mod 2^24.
// RND returns seed / 2^24.

#define RND_MULTIPLIER 16598013u
#define RND_INCREMENT 12820163u
#define RND_MASK 0xFFFFFFu
#define RND_MODULUS 16777216.0

// The current seed, initialized to the same value as in QuickBASIC 4.5
static uint32_t seed = 327680u;

void randomize(double n) {
  // Use the high 32 bits of the double, folded into 16 bits,
  // as the middle 16 bits of the seed, and keep the low byte
  uint64_t bits;
  memcpy(&bits, &n, sizeof(bits));
  uint32_t m = (uint32_t) (bits >> 32);
  m ^= m >> 16;
  seed = ((m & 0xFFFFu) << 8) | (seed & 0xFFu);
}

double rnd() {
  return rnd_F64(1.0);
}

double rnd_F64(double n) {
  if (n < 0.0) {
    // Use the bits of the single precision float as the new seed
    float f = (float) n;
    uint32_t m;
    memcpy(&m, &f, sizeof(m));
    seed = (m & RND_MASK) + (m >> 24);
  }

  if (n != 0.0) {
    seed = (seed * RND_MULTIPLIER + RND_INCREMENT) & RND_MASK;
  }

  return seed / RND_MODULUS;
}
