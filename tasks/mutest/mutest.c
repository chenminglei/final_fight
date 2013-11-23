/** @file mutest.c
 *
 * @brief Creates two simple periodic tasks.
 *
 * @note  This is like knife -- but smaller :)
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-30
 */
#include <stdio.h>
#include <task.h>
#include <unistd.h>
#include <syscall.h>
#include <lock.h>

unsigned int sum = 0;
size_t mutext_num = 0;
unsigned int index1 = 0;
unsigned int index2 = 0;

void panic(const char* str) {
	puts(str);
	while (1);
}

void fun1() {
        while (1) {
		if (mutex_lock(mutext_num) == 0) {
                        printf("fun1 enter lock\n");
			++sum;
                        index1++;
			if (event_wait(0) < 0) {
				panic("Dev 0 failed");
			}
	                //printf("Index1: %u, Index2: %u, Sum: %u\n", index1, index2, sum);
                        printf("fun1 out lock\n");
			mutex_unlock(mutext_num);
		}
	}
}

void fun2() {
	while (1) {
		if (mutex_lock(mutext_num) == 0) {
                        printf("fun2 enter lock\n");
			++sum;
                        index2++;
			if (event_wait(1) < 0) {
				panic("Dev 1 failed");
			}
                        printf("fun2 out lock\n");
			mutex_unlock(mutext_num);
		}
	}
}

int main(int argc, char** argv) {
	printf("Your code get mutest!\n");

	mutext_num = mutex_create();

	task_t tasks[2];
	tasks[0].lambda = fun1;
	tasks[0].data = (void*) '@';
	tasks[0].stack_pos = (void*) 0xa2000000;
	tasks[0].C = 1;
	tasks[0].T = PERIOD_DEV0;
	tasks[1].lambda = fun2;
	tasks[1].data = (void*) '<';
	tasks[1].stack_pos = (void*) 0xa1000000;
	tasks[1].C = 1;
	tasks[1].T = PERIOD_DEV1;

	task_create(tasks, 2);
	argc = argc; /* remove compiler warning */
	argv[0] = argv[0]; /* remove compiler warning */

	puts("Why did your code get here!\n");
	return 0;
}

