/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */
#include <asm.h>
#include <bits/swi.h>                     

        .file "time.S"
        .text

FUNC(time)

        swi TIME_SWI   @call read interrupt
        mov pc, lr     @restore pc
