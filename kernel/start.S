/**
 * @file  start.S
 * Authors: Di Li <dil1@andrew.cmu.edu>                                      
 *          Zhe Shen <zshen@andrew.cmu.edu>
 *          Minglei Chen <mingleic@andrew.cmu.edu>                                      
 * Date:    7:47 PM  Nov 3rd 2013  
 */

#include <asm.h>

	.file   "start.S"

FUNC(_start)

        mov r2, r8              @put r8 as the third parameter of kmain func                                         
        b       kmain            

