/** @file ctx_switch.c
 * 
 * @brief C wrappers around assembly context switch routines.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-21
 */
 

#include <types.h>
#include <assert.h>

#include <config.h>
#include <kernel.h>
#include "sched_i.h"

#include <arm/exception.h>

#ifdef DEBUG_MUTEX
#include <exports.h>
#endif


static __attribute__((unused)) tcb_t* cur_tcb; /* use this if needed */

/**
 * @brief Initialize the current TCB and priority.
 *
 * Set the initialization thread's priority to IDLE so that anything
 * will preempt it when dispatching the first task.
 */
void dispatch_init(tcb_t* idle __attribute__((unused)))
{
    printf("dispatch_init\n");
    printf("dispatch_init %x \n", (idle->context).r4);
    cur_tcb = idle;
    runqueue_remove(idle->cur_prio);
    ctx_switch_half(&(idle->context));
}


/**
 * @brief Context switch to the highest priority task while saving off the 
 * current task state.
 *
 * This function needs to be externally synchronized.
 * We could be switching from the idle task.  The priority searcher has been tuned
 * to return IDLE_PRIO for a completely empty run_queue case.
 */
void dispatch_save(void)
{
    if(cur_tcb->cur_prio < highest_prio())
	return;
    runqueue_add(cur_tcb, cur_tcb->cur_prio);
    tcb_t *next_tcb = runqueue_remove(highest_prio());
    tcb_t* old_tcb = cur_tcb;
    cur_tcb = next_tcb;
    ctx_switch_full(&(cur_tcb->context), &(old_tcb->context));
}

/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * don't save the current task state.
 *
 * There is always an idle task to switch to.
 */
void dispatch_nosave(void)
{
    printf("dispatch_nosave\n");
    tcb_t * next_tcb = runqueue_remove(highest_prio());
    printf("address of next tcb: %x\n", (unsigned int)next_tcb);
    cur_tcb = next_tcb;
    printf("dispatch_nosave %x \n", (next_tcb->context).r4);
    ctx_switch_half(&(next_tcb->context));
}


/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * and save the current task but don't mark is runnable.
 *
 * There is always an idle task to switch to.
 */
void dispatch_sleep(void)
{
    tcb_t * next_tcb = runqueue_remove(highest_prio());
    tcb_t * old_tcb = cur_tcb;
    cur_tcb = next_tcb;
    ctx_switch_full(&(cur_tcb->context), &(old_tcb->context));
}

/**
 * @brief Returns the priority value of the current task.
 */
uint8_t get_cur_prio(void)
{
    return cur_tcb->cur_prio;
}

/**
 * @brief Returns the TCB of the current task.
 */
tcb_t* get_cur_tcb(void)
{
    return cur_tcb;
}
