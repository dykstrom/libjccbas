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

/*
 * GOSUB/RETURN Implementation for BASIC
 *
 * This module provides C library functions for managing BASIC GOSUB/RETURN
 * statements in LLVM IR code. GOSUB/RETURN doesn't map naturally to standard
 * call/ret instructions - it's essentially a computed goto with a manually
 * managed return address stack.
 *
 * LLVM IR Integration
 * -------------------
 *
 * The JCC compiler should generate code using two key LLVM features:
 *
 * 1. blockaddress - Gets a pointer (ptr) to a basic block label
 * 2. indirectbr   - Performs an indirect branch to an address
 *
 * For GOSUB statements:
 *
 *   ; Push the return address (next statement after GOSUB)
 *   call void @gosub_push(ptr blockaddress(@function_name, %return_label))
 *   ; Jump to the subroutine
 *   br label %subroutine_label
 *
 * For RETURN statements:
 *
 *   ; Pop the return address
 *   %return_addr = call ptr @gosub_pop()
 *   ; Jump back to caller - must list ALL possible return targets
 *   indirectbr ptr %return_addr, [label %target1, label %target2, ...]
 *
 * Important: The indirectbr instruction requires listing all possible target
 * blocks at compile time. This is needed for LLVM's static analysis and
 * optimization. Every RETURN statement in the program must list EVERY label
 * that follows ANY GOSUB statement in the entire program, not just the ones
 * that could realistically reach that particular RETURN.
 *
 * Example BASIC code:
 *
 *   10 PRINT "First"
 *   20 GOSUB 100
 *   30 PRINT "Second"
 *   40 GOSUB 100
 *   50 END
 *   100 PRINT "Subroutine"
 *   110 RETURN
 *
 * Corresponding LLVM IR structure:
 *
 *   ; Declare library functions at top of module
 *   declare void @gosub_push(ptr)
 *   declare ptr @gosub_pop()
 *
 *   line_20_gosub:
 *     call void @gosub_push(ptr blockaddress(@main, %line_30))
 *     br label %line_100
 *
 *   line_30:
 *     ; ... continues here after RETURN
 *
 *   line_40_gosub:
 *     call void @gosub_push(ptr blockaddress(@main, %line_50))
 *     br label %line_100
 *
 *   line_50:
 *     ; ... continues here after RETURN
 *
 *   line_100:
 *     ; ... subroutine code here (PRINT "Subroutine")
 *     br label %line_110_return
 *
 *   line_110_return:
 *     %addr = call ptr @gosub_pop()
 *     ; Must list both %line_30 and %line_50 as possible targets
 *     indirectbr ptr %addr, [label %line_30, label %line_50]
 *
 * Compiler Implementation Notes:
 *
 * 1. Scan the entire program and collect all labels that immediately follow
 *    GOSUB statements into a set (e.g., Set<String> allReturnTargets)
 *
 * 2. When generating GOSUB:
 *    - Emit gosub_push with blockaddress of the next statement's label
 *    - Emit br to the subroutine label
 *
 * 3. When generating RETURN:
 *    - Emit gosub_pop to get return address
 *    - Emit indirectbr with the COMPLETE list of all return targets
 *      from step 1
 *
 * 4. Declare gosub_push and gosub_pop at the top of each LLVM module
 *
 * Stack Details:
 *
 * - Maximum nesting depth: 100 levels (configurable in gosub.c)
 * - Automatic initialization on first use
 * - Stack overflow triggers error message and exit(1)
 * - Stack underflow (RETURN without GOSUB) triggers error and exit(1)
 * - Stack is global and shared across the program
 * - Not thread-safe in current implementation
 */

#ifndef GOSUB_H_
#define GOSUB_H_

#include <stdint.h>

// Push a return address onto the GOSUB stack
// addr: The return address (typically from LLVM's blockaddress)
void gosub_push(void* addr);

// Pop a return address from the GOSUB stack
// Returns: The return address to jump to, or NULL if stack is empty
void* gosub_pop(void);

// Initialize the GOSUB stack (called automatically, but can be called manually)
void gosub_init(void);

// Get the current GOSUB stack depth (useful for debugging)
int gosub_depth(void);

#endif /* GOSUB_H_ */
