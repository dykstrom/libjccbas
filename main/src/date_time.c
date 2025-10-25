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
#include <time.h>

#include "date_time.h"
#include "time_util.h"

char* date$() {
  struct tm t;
  _getsystime(&t);

  int length = snprintf(0, 0, "%02d-%02d-%04d", t.tm_mon + 1, t.tm_mday, t.tm_year + 1900);
  char *result = (char *) malloc(length + 1);
  snprintf(result, length + 1, "%02d-%02d-%04d", t.tm_mon + 1, t.tm_mday, t.tm_year + 1900);
  return result;
}

char* time$() {
  struct tm t;
  _getsystime(&t);

  int length = snprintf(0, 0, "%02d:%02d:%02d", t.tm_hour, t.tm_min, t.tm_sec);
  char *result = (char *) malloc(length + 1);
  snprintf(result, length + 1, "%02d:%02d:%02d", t.tm_hour, t.tm_min, t.tm_sec);
  return result;
}
