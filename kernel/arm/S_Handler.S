@ S_Handler.S: switch to user mode and setup user mode stack
@                                               
@ Author: Di Li <dil1@andrew.cmu.edu>    
@         Zhe Shen <zshen@andrew.cmu.edu>       
@         Minglei Chen <mingleic@andrew.cmu.edu>
@ Date:   Oct 18th 2013      
                             
        .file   "S_Handler.S"
        .text            
        .global S_Handler                                         
        .extern global_data
        .extern C_SWI_HANDLER                                     
           
                                                       
S_Handler:                                                                     
        SUB    sp, sp, #4                                                      
        STMFD  sp!, {r0-r12, lr}      @ store registers into stack             
       
        MRS    r2, cpsr               @ enable irq
        BIC    r2, r2, #0x80
        MSR    cpsr, r2        

        MRS    r2, spsr               @ store spsr into stack  
        STR    r2, [sp, #14*4]  
	                                               
        MOV    r1, sp                 @ make current sp as the second parameter
        LDR    r0, [lr, #-4]          @ get the swi code number
        BIC    r0, r0, #0xff000000                                 
        LDR    r2, =global_data
        LDR    r8, [r2]

        BL     C_SWI_HANDLER          @ jump to C swi handler      

        LDR    r2, [sp, #14*4]                                     
        MSR    spsr, r2               @ restore spsr           
        ADD    sp, sp, #4                                      
        LDMFD  sp!, {r1-r12, lr}      @ restore registers except r0
        ADD    sp, sp, #4                                          
        MOVS   pc, lr                 @ go back to user program    

