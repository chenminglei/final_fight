/** @file proc.c
 * 
 * @brief Implementation of `process' syscalls
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-12
 */

#include <exports.h>
#include <bits/errno.h>
#include <config.h>
#include <kernel.h>
#include <syscall.h>
#include <sched.h>

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>

int task_create(task_t* tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
    if(num_tasks > OS_MAX_TASKS)
	return -EINVAL;
    if(!valid_addr(tasks, num_tasks, USR_END_ADDR, USR_START_ADDR))
	return -EFAULT;
    runqueue_init();
    mutex_init();
    dev_init();
    sched_init(tasks);
    allocate_tasks(&tasks, num_tasks);
    dispatch_init(system_tcb + OS_MAX_TASKS - 1);
    return 1; /* remove this line after adding your code */
}

int event_wait(unsigned int dev  __attribute__((unused)))
{
    if (dev > NUM_DEVICES)
        return -EINVAL;

    dev_wait(dev);

    return 0;
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num  __attribute__((unused)))
{
    printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

    disable_interrupts();
    while(1);
}
