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

// check if at least one task has been created
volatile int task_is_created = 0;

/**
 * @brief Task creation
 *
 * @param pointer to tasks array to create
 * @param number of tasks to create
 */
int task_create(task_t* tasks __attribute__((unused)),
		size_t num_tasks __attribute__((unused))) {
	disable_interrupts();
	size_t i = 0;
	// too many task creation requests, return ENVAL error
	if (num_tasks >= OS_AVAIL_TASKS) {
		enable_interrupts();
		return -EINVAL;
	}
	// valid task allocation addresses
	if (valid_addr(tasks, sizeof(task_t) * num_tasks, USR_START_ADDR,
	USR_END_ADDR) == 0) {
		enable_interrupts();
		return -EFAULT;
	}
	// check if C and T specified are valid
	for (; i < num_tasks; i++) {
		if (tasks[i].C > tasks[i].T) {
			enable_interrupts();
			return -ESCHED;
		}
	}
	if (task_is_created) {
		// initialize devices and mutex
		dev_init();
		mutex_init();
	} else {
		task_is_created = 1;
	}
	// allocate tasks
	allocate_tasks(&tasks, num_tasks);
	// schedule initialization
	sched_init(NULL);

	// should never go here
	while (1)
		;

	return 1;
}

/**
 * @brief event wait and go to sleep queue
 *
 * @param dev device number
 */
int event_wait(unsigned int dev __attribute__((unused))) {
	// check error
	if (dev >= NUM_DEVICES)
		return -EINVAL;

        /* check whether the task holds locks */
        tcb_t* cur_tcb = get_cur_tcb();
        if (cur_tcb->holds_lock >= 1) {
            return -EHOLDSLOCK;
        }

	dev_wait(dev);
	return 0;
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num __attribute__((unused))) {
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

	disable_interrupts();

	while (1);
}
