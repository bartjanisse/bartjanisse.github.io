/*! \file shm.c
 *  \ingroup common
 * 	\brief Implementation for the shared memory
 *
 * 	\author A.W Janisse
 * 	\bug No known bugs.
 *
 * 	\version 1.0 	First release.
*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include "shm.h"

static int 		fd = -1;
static size_t 	size;
static button 	*map;
static sem_t 	*sem = SEM_FAILED;

void
initButtonSHM(char *name) 
{
	/* Calculate the size */
	size = sizeof(button) * MAX_DEVS;

	/* Get shared memory handle */
    if((fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0600)) == -1)
    {
        LOGERR("shm_open() failed");
        return;
    }
	/* Set shared memory to the required size */
	if (ftruncate(fd, size) != 0) 
	{
        LOGERR("ftruncate() failed");
        return;
    }
    /* Map shared memory into address space */
    if((map = (button *) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
	{
		LOGERR("mmap() failed");
		return;
	}
	/* Lock shared memory to prevent it from being paged to the swap */
	if(mlock(map, size) != 0)
	{
		LOGERR("mlock() failed");
		return;
	}
	/* Create a semaphore for mutual exclusion in writing and reading */
	if((sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0600, 1)) == SEM_FAILED)
	{
		LOGERR("sem_open() failed");
		return;
	}

	syslog(LOG_INFO, "Succesfully created shared memory and semaphore");
}

void
closeButtonSHM(char *name)
{
	/* Remove any mappings */
	if(munmap(map, size) != 0)
	{
		LOGERR("munmap() failed");
	}
	/* Close the file */
	if(close(fd) != 0)
    {
        LOGERR("close() failed");
    }
	/* Remove the shared memory object */
	if(shm_unlink(name) != 0)
	{
        LOGERR("shm_unlink() failed");
    }
	/* Close the semaphore */
    if (sem_close(sem) != 0)
    {
        LOGERR("sem_close() failed");
    }
    /* Unlink the semaphore */
    if (sem_unlink(SEM_NAME) != 0)
    {
        LOGERR("sem_unlink() failed");
    }

   	syslog(LOG_INFO, "Done cleaning up shared memory and semaphore");
}

void
startButtonSHM(char *name)
{
	/* Get shared memory handle */
    if ((fd = shm_open (name, O_RDWR, 0600)) == -1)
    {
        LOGERR("shm_open() failed");
        return;
    }
    /* Determine the size */
	if((size = lseek(fd, 0, SEEK_END)) == -1)
	{
		LOGERR("lseek() failed");
	}
    /* Map shared memory into address space */
    if((map = (button *) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
        LOGERR("mmap() failed");
        return;
    }
	/* Open the semaphore */
	if((sem = sem_open(SEM_NAME, 0)) == SEM_FAILED)
	{
		LOGERR("sem_open() failed");
		return;
	}
}

void
stopButtonSHM(char *name)
{
	/* Remove any mappings */
	if(munmap(map, size) != 0)
	{
		LOGERR("munmap() failed");
	}
	/* Remove the shared memory object */
	if(shm_unlink(name) != 0)
	{
        LOGERR("shm_unlink() failed");
    }
    /* Unlink the semaphore */
    if (sem_unlink(SEM_NAME) != 0)
    {
        LOGERR("sem_unlink() failed");
    }

   	syslog(LOG_INFO, "Done cleaning up shared memory and semaphore");
}

void
witeButtonToSHM(button *b, uint8_t id)
{
	if(id < 0 || id > MAX_DEVS)
	{
		return;
	}

	/* Begin critical section */
	sem_wait(sem);

	map[id] = *b;

	sem_post(sem);
	/* End critical section */
}

void
readButtonFromSHM(button *b, uint8_t id)
{
	if(id < 0 || id > MAX_DEVS)
	{
		return;
	}
	/* Begin critical section */	
	sem_wait(sem);

	memcpy(b, &map[id], sizeof(button));
		
	sem_post(sem);
	/* End critical section */	
}
