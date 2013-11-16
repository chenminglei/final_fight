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
#include <exports.h>
#include "constant.h"


void timeSetup() {
    /* Clear the last bit of ICLR */
    reg_clear(INT_ICLR_ADDR, 1 << INT_OSTMR_0);
    /* Enable the OS time 0 in the ICMF */
    reg_write(INT_ICMR_ADDR, 1 << INT_OSTMR_0);

    /* Set the initial value of OSMR to be 10ms */
    reg_write(OSTMR_OSMR_ADDR(0), OSMR_COUNT);
    /* Clear the OSCR */
    reg_write(OSTMR_OSCR_ADDR, 0);
    /* Enable the OSMR0 interrupt in OIER */
    reg_set(OSTMR_OIER_ADDR, OSTMR_OIER_E0);
}

