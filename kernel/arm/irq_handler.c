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
        
        dev_update(cur_time*TIME_RESOLUTION);

	/*acknowledge the IRQ */
	reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);
}
