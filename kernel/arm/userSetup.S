@ userSetup.S: switch to user mode and setup user mode stack
@
@ Author: Di Li <dil1@andrew.cmu.edu>
@         Zhe Shen <zshen@andrew.cmu.edu>
@         Minglei Chen <mingleic@andrew.cmu.edu>
@ Date:   Oct 18th 2013

#include <config.h>

        .file   "userSetup.S"
        .text
        .extern  global_data
        .global  userSetup

userSetup:
        MRS  r2, cpsr            @ switch to user mode and mask IRQs and FIQs
        BIC  r2, r2, #0xff       
        ORR  r2, r2, #0x50       
        MSR  cpsr, r2

        MOV  sp, #USR_STACK    @ set stack top at 0xa3000000

        LDR  r2, =global_data  @ restore r8
        LDR  r8, [r2]

        MOV  pc, #LOAD_ADDR    @ set pc to the start address of user program

