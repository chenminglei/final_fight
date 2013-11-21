/*
 * kernel.h: Kernel header file
 *
 * Author: Di Li   <dil1@andrew.cmu.edu>
 *         Zhe Shen <zshen@andrew.cmu.edu>
 *         Minglei Chen <mingleic@andrew.cmu.edu>
 * Date:   Oct 18th
 */
#define STDIN 0
#define STDOUT 1

// swi read 
unsigned swi_read(unsigned* regs);
// swi write
unsigned swi_write(unsigned* regs);
// swi exit
unsigned swi_exit(unsigned staus);
//swi sleep
void swi_sleep(unsigned * regs);
// swi time
unsigned long swi_time();
//install the swi and irq handler
int installHandler(unsigned int * vec_address, unsigned int new_address);
// user set up
void userSetup();

//set up the irq
void irqSetup();

void timeSetup();

// new swi handler
unsigned S_Handler();
//new irq handler
void IRQ_Handler();
void irq_wrapper();

void init_interrupt();
