
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include "shm.h"

static int 		fd = -1;
static button 	*map;
static size_t 	size;

button * 
createButtonSHM(char *name) 
{
	/* Calculate the size */
	size = sizeof(button) * MAX_DEVS;

	/* Get shared memory handle */
    if((fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0600)) == -1)
    {
        LOGERR("shm_open() failed");
    }
	/* Set shared memory to the required size */
	if (ftruncate(fd, size) != 0) 
	{
        LOGERR("ftruncate() failed");
    }
    /* Map shared memory into address space */
    if((map = (button *) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
	{
		LOGERR("mmap() failed");
	}
	/* Lock shared memory to prevent it from being paged to the swap */
	if(mlock(map, size) != 0)
	{
		LOGERR("mlock() failed");
	}

	syslog(LOG_INFO, "Shared memory succesfully created");
	return map;
}

button *
openButtonSHM(char *name)
{
/*	int     rtnval;
    int     size;
    char 	*shm_addr;
    
    shm_fd = shm_open (shm_name, O_RDWR, 0600);
    if (shm_fd == -1)
    {
        perror ("ERROR: shm_open() failed");
    }

    shm_addr = mmap (NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_addr == MAP_FAILED)
    {
        perror ("ERROR: mmap() failed");
    }
  
    return (shm_addr); */
    
    return NULL;
}

void
closeButtonSHM(char *name)
{
	/* Remove any mappings */
	munmap(map, size);
	/* Close the file */
	close(fd);
	/* Remove the shared memory object */
	shm_unlink(name);
	
	syslog(LOG_INFO, "Shared memory succesfully closed");
}
