/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <asm.h>
#include <bits/swi.h>

	.file "mutex_unlock.S"
        .extern errno

FUNC(mutex_unlock)
        swi MUTEX_UNLOCK   @call mutex unlock interrupt
        CMP r0, #0         @compare return value with 0
        bge .Locate        @if r0 is bigger or equal to 0, return normally
        neg r0, r0         @otherwise negate return value

        ldr r2, =errno     @get the address of error number
        str r0, [r2]       @store r0 to error number
        mov r0, #-1        @set return value to -1         

.Locate:
        mov pc, lr         @restore pc

