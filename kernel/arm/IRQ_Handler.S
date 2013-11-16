@ IRQ_Handler.S: Customized IRQ handler
@
@ Author: Di Li <dil1@andrew.cmu.edu>
@         Zhe Shen <zshen@andrew.cmu.edu>
@         Minglei Chen <mingleic@andrew.cmu.edu>
@ Date:   Nov 1st 2013

        .file   "IRQ_Handler.S"
        .text
        .global IRQ_Handler
        .extern C_IRQ_HANDLER
        .extern irq_stack_top

IRQ_Handler:
        SUB    sp, sp, #4                                                      
        STMFD  sp!, {r0-r12, lr}      @ store registers into stack             

        MRS    r2, spsr               @ store spsr into stack
        STR    r2, [sp, #14*4]                                                 

        LDR    r2, =global_data
        LDR    r8, [r2]
	
	MRS    r3, cpsr               @disable IRQ
	ORR    r3, r3, #0x80
        MSR    cpsr, r3

        BL     C_IRQ_HANDLER          @ jump to C IRQ handler

	MRS    r3, cpsr               @enable IRQ
	BIC    r3, r3, #0x80
	MSR    cpsr, r3

        LDR    r2, [sp, #14*4]                                     
        MSR    spsr, r2               @ restore spsr

        LDMFD  sp!, {r0-r12, lr}      @ restore registers except r0
        ADD    sp, sp, #4                                          
        
        SUB    lr, lr, #4                 @ go back to user program
	MOVS   pc, lr
