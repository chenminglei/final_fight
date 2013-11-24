/*                                                                              
 * timeSetup.c: Time driver to set the interrupt controller and 
 * OS timer register                                       
 *                                                                              
 * Authors: Di Li <dil1@andrew.cmu.edu>                                      
 *          Zhe Shen <zshen@andrew.cmu.edu>
 *          Minglei Chen <mingleic@andrew.cmu.edu>                                      
 * Date:    7:47 PM  Nov 3rd 2013                                             
 */ 



#include <arm/reg.h>
#include <arm/timer.h>
#include <arm/interrupt.h>

void init_interrupt() {
    /* Clear the last bit of ICLR */
    reg_clear(INT_ICLR_ADDR, 1 << INT_OSTMR_0);
    /* Enable the OS time 0 in the ICMF */
    reg_write(INT_ICMR_ADDR, 1 << INT_OSTMR_0);
}

