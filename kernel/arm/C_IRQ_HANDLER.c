#include <arm/reg.h>
#include <arm/timer.h>
#include <exports.h>
#include "constant.h"
/*introduce the global variable */
extern volatile unsigned int cur_time;
extern volatile unsigned int cur_bias;
void C_IRQ_HANDLER() {
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
	/*acknowledge the IRQ */
	reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);
}
