/** @file write.S
 *
 * @brief write sycall wrapper
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 07 Oct 2007 01:53:46 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-29
 */
#include <asm.h>
#include <bits/swi.h>

        .file "write.S"
        .text
        .extern errno

FUNC(write)
        swi WRITE_SWI  @call write interrupt
        CMP r0, #0     @compare return value with 0
        bge .Locate    @If r0 is bigger or equal to 0, return normally
        neg r0, r0     @Otherwise negate r0
        ldr r2, =errno @get the address of error number
        str r0, [r2]   @store r0 to error number
        mov r0, #-1    @set return value to -1  

.Locate:
        mov pc, lr     @restore pc

