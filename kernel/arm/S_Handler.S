@ S_Handler.S: switch to user mode and setup user mode stack
@                                               
@ Author: Di Li <dil1@andrew.cmu.edu>    
@         Zhe Shen <zshen@andrew.cmu.edu>       
@         Minglei Chen <mingleic@andrew.cmu.edu>
@ Date:   Oct 18th 2013      
                             
#include <constant.h>
        .file   "S_Handler.S"
        .text            
        .global S_Handler                                         
        .extern global_data
        .extern C_SWI_HANDLER                                     
                                                       
S_Handler:                                                                     
        STMFD  sp!, {r0-r12, lr}      @ store registers into stack             
      
        MRS    r0, spsr               @ store the spsr and sp, lr of user mode
        STMFD  sp, {r0, sp, lr}^
        ADD    sp, sp, #-12
 
        MRS    r2, cpsr               @ enable irq
        BIC    r2, r2, #IRQ_BIT
        MSR    cpsr, r2        
                                  
        ADD    r1, sp, #12            @ make current sp as the second parameter
        LDR    r0, [lr, #-4]          @ get the swi code number
        BIC    r0, r0, #SWI_CODE_GET                                 
        LDR    r2, =global_data
        LDR    r8, [r2]

        BL     C_SWI_HANDLER          @ jump to C swi handler      

        LDMFD  sp, {r1, sp, lr}^      @ restore the spsr and sp, lr of user mode
        ADD    sp, sp, #12
        MSR    spsr, r1

        ADD    sp, sp, #4
        LDMFD  sp!, {r1-r12, lr}      @ restore registers except r0
        MOVS   pc, lr                 @ go back to user program    

