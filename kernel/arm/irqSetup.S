@ irqSetup.S: switch to user mode and setup user mode stack
@
@ Author: Di Li <dil1@andrew.cmu.edu>
@         Zhe Shen <zshen@andrew.cmu.edu>
@         Minglei Chen <mingleic@andrew.cmu.edu>
@ Date:   Oct 18th 2013

        .file   "irqSetup.S"
        .text
        .extern  irq_stack_top
        .global  irqSetup

irqSetup:
        MRS  r2, cpsr            @ switch to irq mode
   
        MOV  r3, r2              
        BIC  r3, r3, #0x0f
        ORR  r3, r3, #0x02
        MSR  cpsr, r3
        
        LDR  sp, =irq_stack_top  @ set the IRQ stack address
        LDR  sp, [sp] 

        
        MSR  cpsr, r2
        MOV  pc, lr

