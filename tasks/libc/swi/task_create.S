/** @file read.S
 *
 * @brief read sycall wrapper
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 07 Oct 2007 01:53:16 -0400
 * 
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-29
 */
#include <asm.h>
#include <bits/swi.h>

        .file "task_create.S"
        .text
        .extern errno

FUNC(task_create)

        swi CREATE_SWI   @call task create interrupt
        CMP r0, #0     @compare return value with 0
        bge .Locate    @if r0 is bigger or equal to 0, return normally
        neg r0, r0     @otherwise negate return value

        ldr r2, =errno @get the address of error number
        str r0, [r2]   @store r0 to error number
        mov r0, #-1    @set return value to -1         

.Locate:
        mov pc, lr     @restore pc
