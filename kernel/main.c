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

int kmain(int argc __attribute__((unused)), char** argv  __attribute__((unused)), uint32_t table)
{
    app_startup();
    global_data = table;

    /*install SWI and IRQ handler */
    if (installHandler((unsigned int *)VEC_SWI, (void *)S_Handler) < 0) {
        printf("install handler failed\n");
        return 0;
    }
    if (installHandler((unsigned int *)VEC_IRQ, (void *)irq_wrapper) < 0) {
        printf("install handler failed\n");
        return 0;
    }

    /*set up the interrupt controller*/
    init_interrupt();

    /*set up the time registers */
    timeSetup();

    /* Set up user program */
    userSetup(argc, argv);

    assert(0);        /* should never get here */
}






