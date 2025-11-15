/*
 * Copyright (C) 2025 Johan Dykstrom
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

#include "gosub.h"
#include "assert.h"

int main() {
    // Test initial depth
    assert_equals_int_int(0, gosub_depth());

    // Test push and depth
    void* addr1 = (void*)0x1000;
    void* addr2 = (void*)0x2000;
    void* addr3 = (void*)0x3000;

    gosub_push(addr1);
    assert_equals_int_int(1, gosub_depth());

    gosub_push(addr2);
    assert_equals_int_int(2, gosub_depth());

    gosub_push(addr3);
    assert_equals_int_int(3, gosub_depth());

    // Test pop and depth
    void* popped = gosub_pop();
    assert_equals_ptr_ptr(addr3, popped);
    assert_equals_int_int(2, gosub_depth());

    popped = gosub_pop();
    assert_equals_ptr_ptr(addr2, popped);
    assert_equals_int_int(1, gosub_depth());

    popped = gosub_pop();
    assert_equals_ptr_ptr(addr1, popped);
    assert_equals_int_int(0, gosub_depth());

    // Test nested pushes
    gosub_push(addr1);
    gosub_push(addr2);
    assert_equals_int_int(2, gosub_depth());

    popped = gosub_pop();
    assert_equals_ptr_ptr(addr2, popped);

    popped = gosub_pop();
    assert_equals_ptr_ptr(addr1, popped);
    assert_equals_int_int(0, gosub_depth());

    return 0;
}
