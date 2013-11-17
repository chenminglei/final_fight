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
extern volatile int cur_num_mutex;


//struct mutex
//{
//        bool_e  bAvailable;             /* flag for availability */
//        tcb_t*  pHolding_Tcb;   /* who are using this mutex */
//        bool_e  bLock;                  /* 1 for lock/0 for unlock */
//        tcb_t*  pSleep_queue;   /* list of applications waiting for this mutex */
//};
//typedef struct mutex mutex_t;
//*/


void mutex_init()
{
    mutex_t mutex_tmp;
    for (int i = 0; i < OS_NUM_MUTEX; i++) {
        mutex_tmp = gtMutex[i];
        mutex_tmp.bAvailable = FALSE;
        mutex_tmp.pHolding_Tcb = NULL;
        mutex_tmp.pSleep_queue = NULL;
    }	
}

int mutex_create(void)
{
    int i = 0;
    mutex_t mutex_tmp;
    
    disable_interrupts();
    
    if (cur_num_mutex == OS_NUM_MUTEX) {
        enable_interrupts();
        return -ENOMEM;
    }
    else {
        for (; i < OS_NUM_MUTEX; i++) {
            mutex_tmp = gtMutex[i];
            if (! mutex_tmp.bAvailable) {
                mutex_tmp.bAvailable = TRUE;
                break; 
            }
        }
        cur_num_mutex++;
        enable_interrupts();
        return i;
    }
}

int mutex_lock(int mutex  __attribute__((unused)))
{
    mutex_t mutex_tmp;
    tcb_t * cur_tcb;

    disable_interrupts();

    if (mutex < 0 || mutex >= OS_NUM_MUTEX) {
        enable_interrupts();
        return -EINVAL;
    }

    mutex_tmp = gtMutex[mutex];
    if (mutex_tmp.bAvailable == FALSE) {
        enable_interrupts();
        return -EINVAL;     
    }

    cur_tcb = get_cur_tcb();   
    if (cur_tcb == mutex_tmp.pHolding_Tcb) {
        enable_interrupts();
        return -EDEADLOCK;
    }
    else if (mutex_tmp.bLock == FALSE) {
        mutex_tmp.bLock = TRUE;
        mutex_tmp.pHolding_Tcb = cur_tcb;
        enable_interrupts();
        return 0;
    }    
    else {
        add_sleep_queue(tmp_mutex, cur_tcb);        
        dispatch_sleep();
        enable_interrupts();
        return 0;
    }
}

int mutex_unlock(int mutex  __attribute__((unused)))
{
    mutex_t mutex_tmp;
    tcb_t * cur_tcb;

    disable_interrupts();

    if (mutex < 0 || mutex >= OS_NUM_MUTEX) {
        enable_interrupts();
        return -EINVAL;
    }

    mutex_tmp = gtMutex[mutex];
    if (mutex_tmp.bAvailable == FALSE) {
        enable_interrupts();
        return -EINVAL;
    }

    cur_tcb = get_cur_tcb();
    if (cur_tcb != mutex_tmp.pHolding_Tcb) {
        enable_interrupts();
        return -EPERM;
    }
    else {
        if (cur_tcb->sleep_queue == NULL) {
            mutex_tmp.pHolding_Tcb = NULL;
            mutex_tmp.bLock = FALSE;
            mutex_tmp.pSleep_queue = NULL;
        }
        else {
            mutex_tmp.pHolding_Tcb = cur_tcb->sleep_queue;
            cur_tcb->sleep_queue = NULL;
            mutex_tmp.bLock = TRUE;
            mutex_tmp.pSleep_queue = mutex_tmp.pHolding_Tcb->sleep_queue;
        }
        dispatch_save();
        enable_interrupts();
        return 0;
    }    
}

void add_sleep_queue(mutex_t mutex_tmp, tcb_t * tcb_tmp) 
{
    tcb_t * tcb_sleep;
    if (mutex_tmp.pSleep_queue == NULL)
        mutex_tmp.pSleep_queue = tcb_tmp;
    else {
        tcb_sleep = mutex_tmp.pSleep_queue;
        while (tcb_sleep->sleep_queue != NULL) {
            tcb_sleep = tcb_sleep->sleep_queue;
        }
        tcb_sleep->sleep_queue = tcb_tmp;
    }
}




