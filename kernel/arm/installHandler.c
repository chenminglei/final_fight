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
#include <constant.h>

int installHandler(unsigned int * vec_address, unsigned int new_address) {

        unsigned int vec_instr = *vec_address;
        unsigned int * jump_handler;

        /* Check the ldr instruction */
        if ((vec_instr & LDR_DEL_DIST) != LDR_CHECK) {
            printf("Info : instruction in vector table incorrect!\n");
            return -1;
        }

        /* Get the address of the original swi handler */
        jump_handler = (unsigned int *)((LDR_GET_DIST & vec_instr) + (unsigned int)vec_address + VEC_ADD);
        jump_handler = (unsigned int *)(*jump_handler);

        /* Change the original instructions */
        *jump_handler = (unsigned int)LDR_INSTR;
        *(jump_handler + 1) = (unsigned int)new_address;
        return 0;
}
