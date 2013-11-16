/** @file io.c
 *
 * @brief Kernel I/O syscall implementations
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-16
 */

#include <types.h>
#include <bits/errno.h>
#include <bits/fileno.h>
#include <arm/physmem.h>
#include <syscall.h>
#include <exports.h>
#include <kernel.h>

#define EOT_CHAR 0x04
#define BAS_CHAR 0x08
#define DEL_CHAR 0x7f
#define NEWL_CHAR 0x0A
#define VTAB_CHAR 0x0D

#define SDRAM_LOW 0xa0000000
#define SDRAM_HIGH 0xa3ffffff
#define FLASH_HIGH 0x00ffffff

/* Read count bytes (or less) from fd into the buffer buf. */
ssize_t read_syscall(int fd __attribute__((unused)), void *buf __attribute__((unused)), size_t count __attribute__((unused)))
{
    int tmp;
    ssize_t i = 0;

    // if file descriptor does not match stdin return error
    if (fd != STDIN_FILENO) {
        return -EBADF;
    }
    // if the memory range specified exceeds the size of SDRAM, return error
    if ((unsigned)buf < SDRAM_LOW
        || (unsigned)(buf + count) > SDRAM_HIGH) {
        return -EFAULT;
    }
    while (i < count) {
        tmp = getc();

        // ctrl + d
        if (tmp == EOT_CHAR) {
            return i;
        }
        // backspace or delete
        else if (tmp == BAS_CHAR || tmp == DEL_CHAR) {
            putc(BAS_CHAR);
            putc(' ');
            putc(BAS_CHAR);
            if (i > 0) {
                ((char*) buf)[i] = '\0';
                i--;
            }
        }
        // newline
        else if (tmp == NEWL_CHAR || tmp == VTAB_CHAR) {
            ((char*) buf)[i] = '\n';
            putc('\n');
            return ++i;
        }
        else {
            ((char*) buf)[i] = tmp;
            putc(tmp);
            i++;
        }
    }
    return i;
}

/* Write count bytes to fd from the buffer buf. */
ssize_t write_syscall(int fd  __attribute__((unused)), const void *buf  __attribute__((unused)), size_t count  __attribute__((unused)))
{
    char c;
    ssize_t num = 0;

    if(fd == STDOUT_FILENO) {
        while(num < count) {
            //check whether the addrss is legal
            if(((unsigned)buf + num) < SDRAM_LOW
                &&((unsigned)buf + num) > FLASH_HIGH) {
                return -EFAULT;
            }
            else if((unsigned)buf + num > SDRAM_HIGH) {
                return -EFAULT;
            }
            //get the char from buffer                                  
            c = *((char*)buf + num);
            putc(c);
            num ++;
            // if we met '\0', it ends                 
            if (c == '\0')
                return num;
        }
    }
    else {
        //if this is not STDOUT, we return with -EBADF 
        return -EBADF;
    }
    return num;
}

