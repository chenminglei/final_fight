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

tcb_t system_tcb[OS_MAX_TASKS]; /*allocate memory for system TCBs */

void sched_init(task_t* main_task  __attribute__((unused)))
{
    system_tcb[IDLE_PRIO].native_prio = IDLE_PRIO;
    system_tcb[IDLE_PRIO].cur_prio = IDLE_PRIO;
    system_tcb[IDLE_PRIO].lr = &launch_task;
    system_tcb[IDLE_PRIO].sp = system_tcb[i].kstack_high;
    system_tcb[IDLE_PRIO].context.r4 = (task_fun_t)idle;
    system_tcb[IDLE_PRIO].context.r5 = (uint8_t)main_task[i].data;
    system_tcb[IDLE_PRIO].context.r6 = (uint8_t)main_task[i].stack_pos;
    system.tcb[IDLE_PRIO].sleep_queue = NULL;
    runqueue_add(&system_tcb[IDLE_PRIO], IDLE_PRIO);
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
    task_t tmp = NULL;
    for (i = 0;i < num_tasks;i ++) {
        for (k = num_tasks - 1;k >= i + 1;k --) {
            if (main_task[k].T < main_task[k - 1].T) {
                tmp = main_task[k];
                main_task[k] = main_task[k - 1];
                main_task[k - 1] = tmp;
            }
        }
    }
    for (i = 0;i < num_tasks;i ++) {
        system_tcb[i].native_prio = i;
        system_tcb[i].cur_prio = i;
        system_tcb[i].lr = &launch_task;
        system_tcb[i].sp = system_tcb[i].kstack_high;
        system_tcb[i].context.r4 = (uint8_t)main_task[i].lambda;
        system_tcb[i].context.r5 = (uint8_t)main_task[i].data;
        system_tcb[i].context.r6 = (uint8_t)main_task[i].stack_pos;
        system.tcb[i].sleep_queue = NULL;
        runqueue_add(&system_tcb[i], i);
    }
}
