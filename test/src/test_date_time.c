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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "assert.h"
#include "date_time.h"

int main(int argc, char *argv[]) {
  // Get current system time for comparison
  time_t now = time(NULL);
  struct tm local_time;
#ifdef _WIN32
  localtime_s(&local_time, &now);
#else
  localtime_r(&now, &local_time);
#endif

  // Test date$()
  char* actual = date$();
  assert_equals_I64_I64(10, strlen(actual));
  assert_true_Bool(actual[2] == '-');
  assert_true_Bool(actual[5] == '-');

  // Parse the date string (format: MM-DD-YYYY) and verify it matches system date
  int month, day, year;
  int parsed = sscanf(actual, "%d-%d-%d", &month, &day, &year);
  assert_equals_I64_I64(3, parsed);

  assert_equals_I64_I64(local_time.tm_mon + 1, month);
  assert_equals_I64_I64(local_time.tm_mday, day);
  assert_equals_I64_I64(local_time.tm_year + 1900, year);

  free(actual);

  // Test time$()
  actual = time$();
  assert_equals_I64_I64(8, strlen(actual));
  assert_true_Bool(actual[2] == ':');
  assert_true_Bool(actual[5] == ':');

  // Parse the time string (format: HH:MM:SS) and verify it matches system time
  int hour, min, sec;
  parsed = sscanf(actual, "%d:%d:%d", &hour, &min, &sec);
  assert_equals_I64_I64(3, parsed);

  // Verify the time matches the current system time (within 2 seconds tolerance)
  // This accounts for the time between getting system time and calling time$()
  int expected_total_sec = local_time.tm_hour * 3600 + local_time.tm_min * 60 + local_time.tm_sec;
  int actual_total_sec = hour * 3600 + min * 60 + sec;
  int diff = actual_total_sec - expected_total_sec;

  // Allow for small time differences (up to 2 seconds)
  assert_true_Bool(diff >= 0 && diff <= 2);

  free(actual);
}
