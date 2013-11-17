/** @file main.c
 *
 * @brief kernel main
 *
 * @author 
 *	   
 *	   
 * @date   
 */
 
#include <kernel.h>
#include <task.h>
#include <sched.h>
#include <device.h>
#include <assert.h>
#include <constant.h>

uint32_t global_data;
volatile int cur_num_mutex = 0;

int kmain(int argc __attribute__((unused)), char** argv  __attribute__((unused)), uint32_t table)
{
    app_startup();
    global_data = table;

    /*install SWI and IRQ handler */
    installHandler((unsigned int *)VEC_SWI, (unsigned int)S_Handler, 0);
    installHandler((unsigned int *)VEC_IRQ, (unsigned int)irq_handler, 1);

    /*set up the IRQ stack */
    irqSetup();

    /*set up the time registers */
    timeSetup();

    /* Set up user program */
    userSetup(argc, argv);

    assert(0);        /* should never get here */
}






