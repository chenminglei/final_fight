/** @file ub_test.c
 * 
 * @brief The UB Test for basic schedulability
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
 */

//#define DEBUG 0

#include <sched.h>
#ifdef DEBUG
#include <exports.h>
#endif

double look_up[62] = {1.000000,0.828427,0.779763,0.756828,0.743492,0.734772,0.728627,0.724062,0.720538,0.717735,0.715452,0.713557,0.711959,0.710593,0.709412,0.708381,0.707472,0.706666,0.705946,0.705298,0.704713,0.704182,0.703698,0.703254,0.702846,0.702469,0.702121,0.701798,0.701497,0.701217,0.700954,0.700709,0.700478,0.700261,0.700056,0.699863,0.699681,0.699508,0.699344,0.699188,0.699040,0.698898,0.698764,0.698636,0.698513,0.698396,0.698284,0.698176,0.698073,0.697974,0.697879,0.697788,0.697700,0.697615,0.697533,0.697455,0.697379,0.697306,0.697235,0.697166,0.697100,0.697036}; 
/**
 * @brief Perform UB Test and reorder the task list.
 *
 * The task list at the end of this method will be sorted in order is priority
 * -- from highest priority (lowest priority number) to lowest priority
 * (highest priority number).
 *
 * @param tasks  An array of task pointers containing the task set to schedule.
 * @param num_tasks  The number of tasks in the array.
 *
 * @return 0  The test failed.
 * @return 1  Test succeeded.  The tasks are now in order.
 */
int assign_schedule(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
	uint8_t i = 0;
	uint8_t k = 0;
	task_t tmp;
	double sum = 0;
	double sumK = 0;
	for(i = 0;i < num_tasks;i ++) {
	    for(k = num_tasks - 1;k >= i + 1;k --) {
		if((*tasks)[k].T < (*tasks)[k - 1].T) {
		    tmp = (*tasks)[k];
		    (*tasks)[k] = (*tasks)[k - 1];
		    (*tasks)[k - 1] = tmp;
		}
	    }
	}

        for(i = 1;i <= num_tasks;i ++) {
	    sumK = look_up[i - 1];
	    if(sum + ((double)(*tasks)[i - 1].C + (double)(*tasks)[i - 1].B)/(double)(*tasks)[i - 1].T > sumK)
		return 0;
	    sum += ((double)(*tasks)[i - 1].C / (double)(*tasks)[i - 1].T);
	    
	}	
	return 1; // fix this; dummy return to prevent compiler warnings	
}
	


