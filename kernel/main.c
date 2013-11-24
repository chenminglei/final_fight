/** @file main.c
 *  
 *  @brief kernel main
 *
 *  @author: Di Li<dil1@andrew.cmu.edu>
 *           Minglei Chen<mingleic@andrew.cmu.edu>
 *           Zhe Shen<zshen@andrew.cmu.edu>
 *  @date: Nov 24th  2013
 */

#include <main.h>
#include <kernel.h>
#include <task.h>
#include <sched.h>
#include <device.h>
#include <lock.h>
#include <assert.h>
#include <constant.h>

uint32_t global_data;

int kmain(int argc __attribute__((unused)), char** argv  __attribute__((unused)), uint32_t table)
{
    app_startup();
    global_data = table;


    /*install SWI and IRQ handler */
    if (installHandler((unsigned int *)VEC_SWI, (unsigned int)S_Handler) < 0) {
        printf("Error: swi handler install failed\n");
        return 0;
    }
    if (installHandler((unsigned int *)VEC_IRQ, (unsigned int)irq_wrapper) < 0) {
        printf("Error: irq handler install failed\n");
        return 0;
    }


    /*set up the interrupt controller*/
    init_interrupt();

    /*set up the time registers */
    timeSetup();

    /*set up the devices */
    dev_init();


    /*set up the mutex */
    mutex_init();


    /* Set up user program */
    userSetup();

    argc = argc;
    argv[0] = argv[0];

    assert(0);        /* should never get here */
}

