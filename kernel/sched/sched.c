/** @file sched.c
 * 
 * @brief Top level implementation of the scheduler.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
 */

#include <types.h>
#include <assert.h>

#include <kernel.h>
#include <config.h>
#include "sched_i.h"

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <types.h>

tcb_t system_tcb[OS_MAX_TASKS]; /*allocate memory for system TCBs */
static void __attribute__((unused)) idle(void);

void sched_init(task_t* main_task  __attribute__((unused)))
{
    /*assign the idle task and add to run queue */
    system_tcb[IDLE_PRIO].native_prio = IDLE_PRIO;
    system_tcb[IDLE_PRIO].cur_prio = IDLE_PRIO;
    system_tcb[IDLE_PRIO].context.lr = launch_task;
    system_tcb[IDLE_PRIO].context.sp = system_tcb[IDLE_PRIO].kstack_high;
    system_tcb[IDLE_PRIO].context.r4 = (uint32_t)idle;
    system_tcb[IDLE_PRIO].context.r5 = (uint32_t)NULL;
    system_tcb[IDLE_PRIO].context.r6 = (uint32_t)NULL;
    system_tcb[IDLE_PRIO].context.r8 = global_data;
    system_tcb[IDLE_PRIO].sleep_queue = NULL;
 
    runqueue_add(&system_tcb[IDLE_PRIO], (uint8_t)IDLE_PRIO);
    /*context switch */
    dispatch_nosave();
}

/**
 * @brief This is the idle task that the system runs when no other task is runnable
 */
 
static void __attribute__((unused)) idle(void)
{
    enable_interrupts();
    while(1);
}

/**
 * @brief Allocate user-stacks and initializes the kernel contexts of the
 * given threads.
 *
 * This function assumes that:
 * - num_tasks < number of tasks allowed on the system.
 * - the tasks have already been deemed schedulable and have been appropriately
 *   scheduled.  In particular, this means that the task list is sorted in order
 *   of priority -- higher priority tasks come first.
 *
 * @param tasks  A list of scheduled task descriptors.
 * @param size   The number of tasks is the list.
 */
void allocate_tasks(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
    uint8_t i = 0;
    uint8_t k = 0;
    task_t tmp;
    /*rank the tasks according to T */
    for (i = 0 ;i < num_tasks ;i++) {
        for ( k = num_tasks - 1; k >= i + 1; k --) {
            if ((*tasks)[k].T < (*tasks)[k - 1].T) {
                tmp = (*tasks)[k];
                (*tasks)[k] = (*tasks)[k - 1];
                (*tasks)[k - 1] = tmp;
            }
        }
    }
    /*initiate the run queue */
    runqueue_init();
    /*assign the tasks to tcbs one by one and add to the run queue*/
    for (i = 0; i < num_tasks; i++) {
        system_tcb[i+1].native_prio = i+1;
        system_tcb[i+1].cur_prio = i+1;
        system_tcb[i+1].context.lr = launch_task;
        system_tcb[i+1].context.sp = system_tcb[i+1].kstack_high;
        system_tcb[i+1].context.r4 = (uint32_t)((*tasks)[i].lambda);
        system_tcb[i+1].context.r5 = (uint32_t)((*tasks)[i].data);
        system_tcb[i+1].context.r6 = (uint32_t)((*tasks)[i].stack_pos);
        system_tcb[i+1].context.r8 = global_data;
        system_tcb[i+1].sleep_queue = NULL;
        runqueue_add(&system_tcb[i+1], i+1);
    }
}
