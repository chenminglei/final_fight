/*                                                                              
 * installHandler.c: hijack the swi or irq handler                                      
 *                                                                              
 * Authors: Di Li <dil1@andrew.cmu.edu>                                      
 *          Zhe Shen <zshen@andrew.cmu.edu>
 *          Minglei Chen <mingleic@andrew.cmu.edu>                                      
 * Date:    7:47 PM  Nov 3rd 2013                                             
 */ 

#include <exports.h>
#include "main.h"
#include "constant.h"


/* the address of the swi handler*/
extern unsigned int *jump_swi;
/* the address of the irq handler*/
extern unsigned int *jump_irq;

/* the original first instruction of the swi handler*/
extern unsigned int swi_instr1;
/* the original second instruction of the swi handler*/
extern unsigned int swi_instr2;

/* the original first instruction of the irq handler*/
extern unsigned int irq_instr1;
/* the original second instruction of the irq handler*/
extern unsigned int irq_instr2;


void installHandler(unsigned int * vec_address, unsigned int new_address, unsigned int type) {

        unsigned int vec_instr = *vec_address;
        unsigned int * jump_handler;

        /* Check the ldr instruction */
        if ((vec_instr & 0xfffff000) != LDR_CHECK) {
            printf("info : instruction in vector table incorrect!\n");
            swi_exit(vec_instr);
        }

        /* Get the address of the original swi handler */
        jump_handler = (unsigned int *)((0x00000fff & vec_instr) + (unsigned int)vec_address + 0x08);
        jump_handler = (unsigned int *)(*jump_handler);


        /* if it is the swi handler */
        if (type == HANDLER_SWI) {
            jump_swi = jump_handler;
            swi_instr1 = *jump_swi;
            swi_instr2 = *(jump_swi + 1);
        }
        /* if it is the irq handler */
        else {
            jump_irq = jump_handler;
            irq_instr1 = *jump_irq;
            irq_instr2 = *(jump_irq + 1);
        }
        /* Change the original instructions */
        *jump_handler = (unsigned int)LDR_INSTR;
        *(jump_handler + 1) = (unsigned int)new_address;
}

