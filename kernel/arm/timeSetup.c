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
#include "constant.h"


void timeSetup() {

    /* Set the initial value of OSMR to be 10ms */
    reg_write(OSTMR_OSMR_ADDR(0), OSMR_COUNT);
    /* Clear the OSCR */
    reg_write(OSTMR_OSCR_ADDR, 0);
    /* Enable the OSMR0 interrupt in OIER */
    reg_set(OSTMR_OIER_ADDR, OSTMR_OIER_E0);
}

