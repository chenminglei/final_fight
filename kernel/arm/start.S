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
        
        sub sp, sp, #8          @leave space for spsr and cpsr                  
        stmfd sp!, {r1-r12, lr} @store r1 to r12 and lr                         
        mrs r2, spsr            @get the value of spsr                          
        str r2, [sp, #14 * 4]   @store spsr to the right place                  
        mrs r2, cpsr            @get the value of cpsr          
        str r2, [sp, #13 * 4]   @store cpsr to the right place

        bic r2, r2, #0x80        
        msr cpsr, r2
     
        ldr r2, [sp, #4]        @restore the value of r2      
                                                              
        ldr r1, =spaddr         @get the address of var spaddr
        str sp, [r1]            @store sp to this variable    
        ldr r1, [sp]            @changed                                 


        mov r2, r8              @put r8 as the third parameter of kmain func                                         
        b       kmain            

