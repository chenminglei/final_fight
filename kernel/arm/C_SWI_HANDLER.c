/*  author: Di Li<dil1@andrew.cmu.edu>
 *          Minglei Chen<mingleic@andrew.cmu.edu>
 *          Zhe Shen<zshen@andrew.cmu.edu>
 */

#include "main.h"
#include <exports.h>
#include <bits/swi.h>
#define BADCODE 0xbadc0de

/*Given swi_num and arguments, decide which swi handler to choose */

unsigned long C_SWI_HANDLER(unsigned swi_num, void* regs) {
        unsigned long result = 0;
	int fd = 0;
	char *buf = null;
	size_t count = 0;
	unsigned long millis = 0;
	task_t task = null;
	unsigned int num = 0;
        /*Decide which swi to run based on swi number */
        switch(swi_num) {
                case READ_SWI:
                    /* call swi_read */
		    fd = (int) (*regs);
		    buf = (char*) regs[1];
		    count = (size_t)(*(regs + 2));
                    result = read_syscall(fd, buf, count);          
                    break;                            
                case WRITE_SWI:                           
                    /* call swi_write */
		    fd = (int) (*regs);
                    buf = (char*) regs[1];
                    count = (size_t)(*(regs + 2));
                    result = write_syscall(fd, buf, count);      
                    break;    
		case TIME_SWI:
		    /* call swi_time */
		    result = time_syscall();
		    break;
		case SLEEP_SWI:
		    /*call swi_sleep */
		    millis = (unsigned long) (*regs);
		    sleep_syscall(millis);
		    break;
		case CREATE_SWI:
		    /* call swi_create */
		    task = (task_t) (*regs);
		    count = (size_t) regs[1];
		    result = task_create(task, count);
		    break;
		case MUTEX_CREATE:
		    /* call mutex create */
		    result = mutex_create();
		    break;
		case MUTEX_LOCK:
		    /*call mutex_lock */
		    fd = (int) (*regs)
		    result = mutex_lock(fd);
		    break;
		case MUTEX_UNLOCK:
		    fd = (int) (*regs);
		    result = mutex_unlock(fd);
		    break;
		case EVENT_WAIT:
		    num = (unsigned int) (*regs);
		    result = event_wait(num);
		    break;
                default:                           
                    /* return with status bad code */
                    result = BADCODE;                 
        }                                    

        return result;                       
}                     
