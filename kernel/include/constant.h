/*                                                                              
 * Authors: Di Li <dil1@andrew.cmu.edu>                                      
 *          Zhe Shen <zshen@andrew.cmu.edu>
 *          Minglei Chen <mingleic@andrew.cmu.edu>                                      
 * Date:    7:47 PM  Nov 3th 2013                                             
 */ 

#define VEC_SWI   0x08
#define VEC_IRQ   0x18
#define VEC_ADD   0x08
#define LDR_INSTR 0xe51ff004
#define LDR_CHECK 0xe59ff000
#define LDR_DEL_DIST  0xfffff000
#define LDR_GET_DIST  0x00000fff


#define IRQ_STACK_SIZE 1024

#define OSMR_COUNT  32500
#define OSMR_ONE_SECOND 3250

#define OSCR_CLEAR  0x00

#define TIME_RESOLUTION 10

