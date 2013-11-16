@ userSetup.S: switch to user mode and setup user mode stack
@
@ Author: Di Li <dil1@andrew.cmu.edu>
@         Zhe Shen <zshen@andrew.cmu.edu>
@         Minglei Chen <mingleic@andrew.cmu.edu>
@ Date:   Oct 18th 2013

        .file   "userSetup.S"
        .text
        .global  userSetup

userSetup:
        MRS  r2, cpsr            @ switch to user mode and mask IRQs and FIQs
        BIC  r2, r2, #0xff       
        ORR  r2, r2, #0x50       
        MSR  cpsr, r2

        MOV  sp, #0xa3000000     @ set stack top at 0xa3000000
        MOV  r2, r0
        Add  r1, r1, r0, LSL #2 

LC1:
        SUB  r1, r1, #4          @ push all addresses of strings reference by argv into stack
        LDR  r3, [r1]
        STR  r3, [sp, #-4]!

        SUB  r2, r2, #1
        CMP  r2, #0
        BGT  LC1
LC2:
        STR  r0, [sp, #-4]!      @ push argc into stack
        MOV  pc, #0xa0000000

