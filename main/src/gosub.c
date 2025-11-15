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
#include <stdio.h>
#include <stdlib.h>

#define GOSUB_STACK_SIZE 100

// Global GOSUB stack and pointer
static void* gosub_stack[GOSUB_STACK_SIZE];
static int gosub_stack_ptr = 0;
static int gosub_initialized = 0;

void gosub_init(void) {
    if (!gosub_initialized) {
        gosub_stack_ptr = 0;
        gosub_initialized = 1;
    }
}

void gosub_push(void* addr) {
    // Initialize on first use
    if (!gosub_initialized) {
        gosub_init();
    }

    // Check for stack overflow
    if (gosub_stack_ptr >= GOSUB_STACK_SIZE) {
        fprintf(stderr, "Error: GOSUB stack overflow (max depth: %d)\n", GOSUB_STACK_SIZE);
        exit(1);
    }

    // Push the address
    gosub_stack[gosub_stack_ptr] = addr;
    gosub_stack_ptr++;
}

void* gosub_pop(void) {
    // Initialize on first use
    if (!gosub_initialized) {
        gosub_init();
    }

    // Check for stack underflow
    if (gosub_stack_ptr <= 0) {
        fprintf(stderr, "Error: GOSUB stack underflow (RETURN without GOSUB)\n");
        exit(1);
    }

    // Pop the address
    gosub_stack_ptr--;
    return gosub_stack[gosub_stack_ptr];
}

int gosub_depth(void) {
    if (!gosub_initialized) {
        gosub_init();
    }
    return gosub_stack_ptr;
}
