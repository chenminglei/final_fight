/** @file time.c
 *
 * @brief Kernel timer based syscall implementations
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-16
 */

#include <types.h>
#include <config.h>
#include <bits/errno.h>
#include <arm/timer.h>
#include <arm/reg.h>
#include <syscall.h>
#include <constant.h>

#include <arm/timer.h>
#include <arm/reg.h>
#include <exports.h>
#include "constant.h"

extern volatile unsigned int cur_time;
extern volatile unsigned int cur_bias;

unsigned long time_syscall(void)
{
    /*read the current value of OSCR */
    volatile unsigned long oscr = reg_read(OSTMR_OSCR_ADDR);
    /*get the current time */
    unsigned long ret = cur_time*TIME_RESOLUTION + (oscr + cur_bias)/OSMR_ONE_SECOND;
    return ret;
}

/** @brief Waits in a tight loop for atleast the given number of milliseconds.
 *
 * @param millis  The number of milliseconds to sleep.
 *
 * 
 */
void sleep_syscall(unsigned long millis  __attribute__((unused)))
{
    /* Get current time */
    unsigned long currenttime = cur_time;
    /* Get the cumulated bias for oscr */
    unsigned long bias = cur_bias / OSMR_ONE_SECOND;

    /* Break until sleep enough */
    while (currenttime * TIME_RESOLUTION + millis + bias > cur_time * TIME_RESOLUTION + cur_bias / OSMR_ONE_SECOND) {
    };	
}
