/** @file irq_handler.c 
 *  
 *  @brief This is the irq handler, update the global time,
 *   and call device update
 *
 *  @author: Di Li<dil1@andrew.cmu.edu>
 *           Minglei Chen<mingleic@andrew.cmu.edu>
 *           Zhe Shen<zshen@andrew.cmu.edu>
 *  @date: Nov 24th  2014
 */

#include <arm/reg.h>
#include <arm/timer.h>
#include <exports.h>
#include "constant.h"
#include <device.h>

/*introduce the global variable */
volatile unsigned int cur_time;
volatile unsigned int cur_bias;

void irq_handler() {
	/*increase the value of current time unit */
	cur_time++;

        volatile unsigned long oscr = reg_read(OSTMR_OSCR_ADDR);
	/*accumulate and record the time difference */
        cur_bias += (oscr - OSMR_COUNT);
	/*set OSCR to 0 */
	reg_write(OSTMR_OSCR_ADDR, OSCR_CLEAR);
	/*if bias is bigger than a time unit(10ms), we change the current time unit */
        if (cur_bias >= OSMR_COUNT) {
            cur_bias -= OSMR_COUNT;
            cur_time++;
        }    
        
	reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);

        dev_update(cur_time*TIME_RESOLUTION);
}
