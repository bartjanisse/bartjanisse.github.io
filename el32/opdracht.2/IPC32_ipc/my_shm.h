#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

#ifndef __MYSHM_H__
#define __MYSHM_H__

static int  shm_fd = -1;

static char *
my_shm_create (char * shm_name, int size)
{
    int     rtnval;
    char *  shm_addr;
    #ifdef VERBOSE
        printf ("Calling shm_open('%s')\n", shm_name);
    #endif
    shm_fd = shm_open (shm_name, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (shm_fd == -1)
    {
        perror ("ERROR: shm_open() failed");
    }
    #ifdef VERBOSE
        printf ("shm_open() returned %d\n", shm_fd);
        
        printf ("Calling ftrucate(%d,%d)\n", shm_fd, size);
    #endif

    rtnval = ftruncate (shm_fd, size);
    if (rtnval != 0)
    {
        perror ("ERROR: ftruncate() failed");
    }

    #ifdef VERBOSE
        printf ("ftruncate() returned %d\n", rtnval);
                
        printf ("Calling mmap(len=%d,fd=%d)\n", size, shm_fd);
    #endif
    shm_addr = mmap (NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_addr == MAP_FAILED)
    {
        perror ("ERROR: mmap() failed");
    }
    #ifdef VERBOSE
        printf ("mmap() returned %#x\n", (unsigned int) shm_addr);
    #endif

    return (shm_addr);
}

static char *
my_shm_open (char * shm_name, int* ret_size)
{
    int     rtnval;
    int     size;
    char *  shm_addr;
    
    #ifdef VERBOSE
        printf ("Calling shm_open('%s')\n", shm_name);
    #endif

    shm_fd = shm_open (shm_name, O_RDWR, 0600);
    if (shm_fd == -1)
    {
        perror ("ERROR: shm_open() failed");
    }
    #ifdef VERBOSE
        printf ("shm_open() returned %d\n", shm_fd);
                    
        printf ("Calling lseek(fd=%d,SEEK_END)\n", shm_fd);
    #endif
    size = lseek (shm_fd, 0, SEEK_END);
    #ifdef VERBOSE
        printf ("lseek() returned %d\n", size);
                    
        printf ("Calling mmap(len=%d,fd=%d)\n", size, shm_fd);
    #endif
    shm_addr = mmap (NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_addr == MAP_FAILED)
    {
        perror ("ERROR: mmap() failed");
    }
    #ifdef VERBOSE
        printf ("mmap() returned %#x\n", (unsigned int) shm_addr);
    #endif

    *ret_size = size;
    return (shm_addr);
}

#endif