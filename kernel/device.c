/**
 * @file device.c
 *
 * @brief Implements simulated devices.
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-12-01
 */

#include <types.h>
#include <assert.h>

#include <task.h>
#include <sched.h>
#include <device.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>

#include <exports.h>

/**
 * @brief Fake device maintainence structure.
 * Since our tasks are periodic, we can represent 
 * tasks with logical devices. 
 * These logical devices should be signalled periodically 
 * so that you can instantiate a new job every time period.
 * Devices are signaled by calling dev_update 
 * on every timer interrupt. In dev_update check if it is 
 * time to create a tasks new job. If so, make the task runnable.
 * There is a wait queue for every device which contains the tcbs of
 * all tasks waiting on the device event to occur.
 */

struct dev
{
	tcb_t* sleep_queue;
	unsigned long   next_match;
};
typedef struct dev dev_t;

/* devices will be periodically signaled at the following frequencies */
const unsigned long dev_freq[NUM_DEVICES] = { 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900, 3000, 3100, 3200, 3300, 3400, 3500, 3600, 3700, 3800, 3900, 4000, 4100, 4200, 4300, 4400, 4500, 4600, 4700, 4800, 4900, 5000, 5100, 5200, 5300, 5400, 5500, 5600, 5700, 5800, 5900, 9000};

static dev_t devices[NUM_DEVICES];

/**
 * @brief Initialize the sleep queues and match values for all devices.
 */
void dev_init(void)
{
   /* the following line is to get rid of the warning and should not be needed */
    int i = 0;
    for(i = 0;i < NUM_DEVICES;i ++) {
	devices[i].next_match = dev_freq[i];
	devices[i].sleep_queue = NULL;
    }
}


/**
 * @brief Puts a task to sleep on the sleep queue until the next
 * event is signalled for the device.
 *
 * @param dev  Device number.
 */
void dev_wait(unsigned int dev __attribute__((unused)))
{
    disable_interrupts();

    tcb_t* cur_tcb = get_cur_tcb();

    /* Insert the current one into the device sleep queue */    
    cur_tcb->sleep_queue = devices[dev].sleep_queue;

    devices[dev].sleep_queue = cur_tcb;

    dispatch_sleep();

    enable_interrupts();
}


/**
 * @brief Signals the occurrence of an event on all applicable devices. 
 * This function should be called on timer interrupts to determine that 
 * the interrupt corresponds to the event frequency of a device. If the 
 * interrupt corresponded to the interrupt frequency of a device, this 
 * function should ensure that the task is made ready to run 
 */
void dev_update(unsigned long millis __attribute__((unused)))
{
    tcb_t * sleep_tcb = NULL;
    tcb_t * next_tcb = NULL;
    int i = 0;
    int add = 0;

    disable_interrupts();

    /* find the devices that should be called up */
    for ( i = 0; i < NUM_DEVICES; i++) {
        if (devices[i].next_match <= millis) {
            /* Put all tasks in the sleep queue into runqueue */
            sleep_tcb = devices[i].sleep_queue;
            devices[i].sleep_queue = NULL;
            while (sleep_tcb != NULL) {
                add = 1;
                runqueue_add(sleep_tcb, sleep_tcb->cur_prio);
                next_tcb = sleep_tcb->sleep_queue;
                sleep_tcb->sleep_queue = NULL;
                sleep_tcb = next_tcb; 
            }
            /* update device next match */
            devices[i].next_match = millis + dev_freq[i];
        }
    }
    if (add == 1) dispatch_save();	
}

