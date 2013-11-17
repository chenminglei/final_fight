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

void mutex_init()
{
	
}

int mutex_create(void)
{
    if (cur_num_mutex == OS_NUM_MUTEX)
        return -ENOMEM;
    else {
        cur_num_mutex++;
        mutex_t tmp_mutex = gtMutex[cur_num_mutex - 1];
        tmp_mutex->bAvailable = TRUE;
        return cur_num_mutex-1;
    }
}

int mutex_lock(int mutex  __attribute__((unused)))
{
    if (mutex < 0 || mutex >= OS_NUM_MUTEX)
        return -EINVAL;

    mutex_t tmp_mutex = gtMutex[mutex];
    if (tmp_mutex->bAvailable == FALSE)
        return -EINVAL;     

    tcb_t* cur_tcb = get_cur_tcb();   
    if (cur_tcb == tmp_mutex->pHolding_Tcb)
        return -EDEADLOCK;
    else if (tmp_mutex->bLock == FALSE) {
        tmp_mutex->bLock = TRUE;
        tmp_mutex->pHolding_Tcb = cur_tcb;
        return 0;
    }    
    else {
        

    }




    return 0; // fix this to return the correct value
}

int mutex_unlock(int mutex  __attribute__((unused)))
{
    return 1; // fix this to return the correct value
}

