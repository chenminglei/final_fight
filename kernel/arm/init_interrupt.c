/** @file init_interrupt.c 
 *  
 *  @brief This is used to set up and initialize the 
 *         Interrupt Controller
 *
 *  @author: Di Li<dil1@andrew.cmu.edu>
 *           Minglei Chen<mingleic@andrew.cmu.edu>
 *           Zhe Shen<zshen@andrew.cmu.edu>
 *  @date: Nov 24th  2013
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

