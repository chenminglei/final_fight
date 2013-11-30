/** @file mutex.c 
 *  
 *  @brief Implements mutices
 *
 *  @author: Harry Q Bovik
 *           Di Li<dil1@andrew.cmu.edu>
 *           Minglei Chen<mingleic@andrew.cmu.edu>
 *           Zhe Shen<zshen@andrew.cmu.edu>
 *  @date: Nov 24th  2013
 */

//#define DEBUG_MUTEX

#include <lock.h>
#include <task.h>
#include <sched.h>
#include <bits/errno.h>
#include <arm/psr.h>
#include <arm/exception.h>

#ifdef DEBUG_MUTEX
#include <exports.h> // temp
#endif

mutex_t gtMutex[OS_NUM_MUTEX];
volatile int cur_num_mutex = 0;
void add_sleep_queue(mutex_t* mutex_tmp, tcb_t * tcb_tmp);

/**
 * @brief initialize the array of mutex, gtMutex
 *
 */
void mutex_init() {
	int i = 0;
	for (i = 0; i < OS_NUM_MUTEX; i++) {
		gtMutex[i].bAvailable = FALSE;
		gtMutex[i].bLock = FALSE;
		gtMutex[i].pHolding_Tcb = NULL;
		gtMutex[i].pSleep_queue = NULL;
	}
}

/**
 * @brief create a mutex
 *
 */
int mutex_create(void) {
	int i = 0;

	disable_interrupts();

	if (cur_num_mutex == OS_NUM_MUTEX) {
		enable_interrupts();
		return -ENOMEM;
	} else {
                /* find the first one that has not been created */
		for (; i < OS_NUM_MUTEX; i++) {
			if (!gtMutex[i].bAvailable) {
				gtMutex[i].bAvailable = TRUE;
				break;
			}
		}
		cur_num_mutex++;
		enable_interrupts();
		return i;
	}
}

/**
 * @brief lock the mutex 
 *
 * @param mutex  the index of the mutex in the mutex array
 */
int mutex_lock(int mutex __attribute__((unused))) {
	tcb_t * cur_tcb;

	disable_interrupts();

        /* Check whether the mutex value is invalid or not */
	if (mutex < 0 || mutex >= OS_NUM_MUTEX) {
		enable_interrupts();
		return -EINVAL;
	}
 
        /* Check whether this mutex has been created or not */
	if (gtMutex[mutex].bAvailable == FALSE) {
		enable_interrupts();
		return -EINVAL;
	}

	cur_tcb = get_cur_tcb();
        /* Check whether the current tcb is holding the mutex */
	if (cur_tcb == gtMutex[mutex].pHolding_Tcb) {
		enable_interrupts();
		return -EDEADLOCK;
	} else if (gtMutex[mutex].bLock == FALSE) {
                /* acquire the mutex */
		gtMutex[mutex].bLock = TRUE;
		gtMutex[mutex].pHolding_Tcb = cur_tcb;
                /* change the priority of the task and number of locks holded */
                cur_tcb->cur_prio = 0;
                cur_tcb->holds_lock++;
		
                enable_interrupts();
		return 0;
	} else {
                /* Add the one to the sleep queue of the mutex and sleep */
		add_sleep_queue((mutex_t*)&gtMutex[mutex], cur_tcb);
		dispatch_sleep();
		enable_interrupts();
		return 0;
	}
}

/**
 * @brief unlock the mutex 
 *
 * @param mutex  the index of the mutex in the mutex array
 */
int mutex_unlock(int mutex __attribute__((unused))) {
	tcb_t * cur_tcb;

	disable_interrupts();

        /* Check whether the mutex value is invalid or not */
	if (mutex < 0 || mutex >= OS_NUM_MUTEX) {
		enable_interrupts();
		return -EINVAL;
	}

        /* Check whether this mutex has been created or not */
	if (gtMutex[mutex].bAvailable == FALSE) {
		enable_interrupts();
		return -EINVAL;
	}

	cur_tcb = get_cur_tcb();
        /* Check whether the current tcb is holding the mutex */
	if (cur_tcb != gtMutex[mutex].pHolding_Tcb) {
		enable_interrupts();
		return -EPERM;
	} else {
		if (gtMutex[mutex].pSleep_queue == NULL) {
			gtMutex[mutex].pHolding_Tcb = NULL;
			gtMutex[mutex].bLock = FALSE;
		} else {
                        /* release the mutex to the first one in the sleep queue and 
                         * put that one into the run queue
                         */
			gtMutex[mutex].pHolding_Tcb = gtMutex[mutex].pSleep_queue;
			gtMutex[mutex].bLock = TRUE;
			gtMutex[mutex].pSleep_queue = gtMutex[mutex].pHolding_Tcb->sleep_queue;
			runqueue_add(gtMutex[mutex].pHolding_Tcb,
					gtMutex[mutex].pHolding_Tcb->cur_prio);
			gtMutex[mutex].pHolding_Tcb->sleep_queue = NULL;
		}
                /* update the number of locks holded, and the cur priority */ 
                cur_tcb->holds_lock--;
                if (cur_tcb->holds_lock == 0)
                    cur_tcb->cur_prio = cur_tcb->native_prio;

		enable_interrupts();
		return 0;
	}
}

/**
 * @brief Puts a task to sleep on the sleep queue for a mutex
 *
 * @param  mutex_tmp  the mutex
 *         tcb_tmp    tcp that should be put in the sleep queue
 */
void add_sleep_queue(mutex_t* mutex_tmp, tcb_t * tcb_tmp) {
	tcb_t * tcb_sleep;
	if (mutex_tmp->pSleep_queue == NULL)
		mutex_tmp->pSleep_queue = tcb_tmp;
	else {
		tcb_sleep = mutex_tmp->pSleep_queue;
		while (tcb_sleep->sleep_queue != NULL) {
			tcb_sleep = tcb_sleep->sleep_queue;
		}
		tcb_sleep->sleep_queue = tcb_tmp;
	}
}

