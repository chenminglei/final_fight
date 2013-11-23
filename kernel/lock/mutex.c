/**
 * @file mutex.c
 *
 * @brief Implements mutices.
 *
 * @author Harry Q Bovik < PUT YOUR NAMES HERE
 *
 * 
 * @date  
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

void mutex_init() {
	int i = 0;
	for (i = 0; i < OS_NUM_MUTEX; i++) {
		gtMutex[i].bAvailable = FALSE;
		gtMutex[i].bLock = FALSE;
		gtMutex[i].pHolding_Tcb = NULL;
		gtMutex[i].pSleep_queue = NULL;
	}
}

int mutex_create(void) {
	int i = 0;

	disable_interrupts();

	if (cur_num_mutex == OS_NUM_MUTEX) {
		enable_interrupts();
		return -ENOMEM;
	} else {
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

int mutex_lock(int mutex __attribute__((unused))) {
	tcb_t * cur_tcb;

	disable_interrupts();

	if (mutex < 0 || mutex >= OS_NUM_MUTEX) {
		enable_interrupts();
		return -EINVAL;
	}

	if (gtMutex[mutex].bAvailable == FALSE) {
		enable_interrupts();
		return -EINVAL;
	}

	cur_tcb = get_cur_tcb();
	if (cur_tcb == gtMutex[mutex].pHolding_Tcb) {
		enable_interrupts();
		return -EDEADLOCK;
	} else if (gtMutex[mutex].bLock == FALSE) {
		gtMutex[mutex].bLock = TRUE;
		gtMutex[mutex].pHolding_Tcb = cur_tcb;
		enable_interrupts();
		return 0;
	} else {

		add_sleep_queue((mutex_t*)&gtMutex[mutex], cur_tcb);
		dispatch_sleep();
		return 0;
	}
}

int mutex_unlock(int mutex __attribute__((unused))) {
	tcb_t * cur_tcb;

	disable_interrupts();

	if (mutex < 0 || mutex >= OS_NUM_MUTEX) {
		enable_interrupts();
		return -EINVAL;
	}

	//mutex_tmp = gtMutex[mutex];
	if (gtMutex[mutex].bAvailable == FALSE) {
		enable_interrupts();
		return -EINVAL;
	}

	cur_tcb = get_cur_tcb();
	if (cur_tcb != gtMutex[mutex].pHolding_Tcb) {
		enable_interrupts();
		return -EPERM;
	} else {
		if (gtMutex[mutex].pSleep_queue == NULL) {
			gtMutex[mutex].pHolding_Tcb = NULL;
			gtMutex[mutex].bLock = FALSE;
			//gtMutex[mutex].pSleep_queue = NULL;
		} else {
			gtMutex[mutex].pHolding_Tcb = gtMutex[mutex].pSleep_queue;
			//cur_tcb->sleep_queue = NULL;
			gtMutex[mutex].bLock = TRUE;
			gtMutex[mutex].pSleep_queue = gtMutex[mutex].pHolding_Tcb->sleep_queue;
			runqueue_add(gtMutex[mutex].pHolding_Tcb,
					gtMutex[mutex].pHolding_Tcb->cur_prio);
		}
		enable_interrupts();
		return 0;
	}
}

void add_sleep_queue(mutex_t* mutex_tmp, tcb_t * tcb_tmp) {
	tcb_t * tcb_sleep;
	if (mutex_tmp -> pSleep_queue == NULL)
		mutex_tmp -> pSleep_queue = tcb_tmp;
	else {
		tcb_sleep = mutex_tmp -> pSleep_queue;
		while (tcb_sleep->sleep_queue != NULL) {
			tcb_sleep = tcb_sleep->sleep_queue;
		}
		tcb_sleep->sleep_queue = tcb_tmp;
	}
}

