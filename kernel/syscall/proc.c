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
#include <lock.h>

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>


volatile int task_is_created = 0;


int task_create(task_t* tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
    disable_interrupts();
    size_t i = 0;  
    if(num_tasks >= OS_AVAIL_TASKS) {
        enable_interrupts();
	return -EINVAL;
    }

    if(valid_addr(tasks, sizeof(task_t) * num_tasks, USR_START_ADDR, USR_END_ADDR) == 0) {
        enable_interrupts();
	return -EFAULT;
    }
    for(;i < num_tasks;i ++) {
	if(tasks[i].C > tasks[i].T) {
	    enable_interrupts();
	    return -ESCHED;
	}
    }
    if (task_is_created) {
        dev_init();
        mutex_init();
    }
    else
        task_is_created = 1;

    allocate_tasks(&tasks, num_tasks);

    sched_init(NULL);
    
    while (1) {}

    return 1;
}

int event_wait(unsigned int dev  __attribute__((unused)))
{
    if (dev >= NUM_DEVICES)
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
