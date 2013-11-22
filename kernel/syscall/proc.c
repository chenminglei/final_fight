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

int task_create(task_t* tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
    disable_interrupts();
    printf("task_create1\n");
    int a = 0;


    printf("task lambda's r4: %x\n", (uint32_t)(tasks[0].lambda));
    printf("task lambda's r4: %x\n", (uint32_t)(tasks[1].lambda));


      
    if(num_tasks >= OS_AVAIL_TASKS) {
        enable_interrupts();
	return -EINVAL;
    }

    printf("task_create2\n");
    if(valid_addr(tasks, sizeof(task_t) * num_tasks, USR_START_ADDR, USR_END_ADDR) == 0) {
        enable_interrupts();
	return -EFAULT;
    }

    printf("task_create3\n");

    mutex_init();

    printf("task lambda's r4: %x\n", (uint32_t)(tasks[0].lambda));
    printf("task lambda's r4: %x\n", (uint32_t)(tasks[1].lambda));

    printf("mutex_init\n");

    allocate_tasks(&tasks, num_tasks);

    printf("allocate_tasks\n");

    sched_init(NULL);

    printf("sched_init\n");
    
    while (a == 0) {}

    return 1;
}

int event_wait(unsigned int dev  __attribute__((unused)))
{
    if (dev >= NUM_DEVICES)
        return -EINVAL;

    dev_wait(dev);
    
    printf("back to event_wait\n");
    
    return 0;
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num  __attribute__((unused)))
{
    printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

    disable_interrupts();

    while(1);
}
