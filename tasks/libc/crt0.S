/** @file crt0.S
 *
 * @brief Execution startup routine
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 07 Oct 2007 01:51:29 -0400
 */

#include <asm.h>

	.file "crt0.S"

FUNC(_start)
        ldr r0, [sp], #4 @move content in address sp to r0 and modify sp
        mov r1, sp       @move sp + 4 to r1
.Loop
        bl main          @call main function
        b .Loop          @jump to main when return
