/*
** (C) 2015, A.W. Janisse
** 16 maart 2015
**
**
**
** Compile with: gcc semseq.c -o semseq -pthread -lrt -lm
*/

//#define VERBOSE

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <math.h>
#include "my_shm.h"

char        sem_flags_name[]    = "flags";
char        sem_sync_name[]     = "sync";
char        shm_name[]          = "flagstore";  
char *      shm_addr            = MAP_FAILED; 
sem_t *     sem_flags           = SEM_FAILED;
sem_t *     sem_sync            = SEM_FAILED;
int         SHM_SIZE            = 1;
unsigned char  flags            = 0x0;
unsigned char  mask;

void
initSemSHM(int pid)
{
    printf ("\n");
    #ifdef VERBOSE
    printf("Creating flags semaphore for procces %d\n", pid);
    #endif
    
    sem_flags = sem_open (sem_flags_name, O_CREAT | O_EXCL, 0600, 1);
    
    #ifdef VERBOSE
    if (sem_flags == SEM_FAILED)
    {
        perror ("ERROR: sem_open() failed");
    }
    printf("Creating sync semaphore for procces %d\n", pid);
    #endif
    
    sem_sync = sem_open (sem_sync_name, O_CREAT | O_EXCL, 0600, 0);
    
    #ifdef VERBOSE
    if (sem_sync == SEM_FAILED)
    {
        perror ("ERROR: sem_open() failed");
    }
    printf ("Creating Shared memory '%s' \n", shm_name);
    #endif
    
    shm_addr = my_shm_create (shm_name, SHM_SIZE);
    shm_addr[0] = flags; // make sure it's 0;
}

void
openSemSHM()
{
    int size;
    sem_sync = sem_open (sem_sync_name, 0);
    sem_flags = sem_open (sem_flags_name, 0);
    shm_addr = my_shm_open (shm_name, &size);
}

void
cleanupSemSHM(int pid)
{
    int rtnval;

    #ifdef VERBOSE
    printf("Closing flags semaphore for procces %d\n", pid);
    #endif

    rtnval = sem_close (sem_flags);

    #ifdef VERBOSE
    if (rtnval != 0)
    {
        perror ("ERROR: sem_close() failed");
    }
    printf ("sem_close() flags returned %d\n", rtnval);
    #endif

    rtnval = sem_unlink (sem_flags_name);

    #ifdef VERBOSE
    if (rtnval != 0)
    {
        perror ("ERROR: sem_unlink() flags failed");
    }
    printf ("sem_unlink() flags returned %d\n", rtnval);
    #endif

    rtnval = sem_close (sem_sync);

    #ifdef VERBOSE
    if (rtnval != 0)
    {
        perror ("ERROR: sem_close() sync failed");
    }
    printf ("sem_close() sync returned %d\n", rtnval);
    #endif

    rtnval = sem_unlink (sem_sync_name);

    #ifdef VERBOSE
    if (rtnval != 0)
    {
        perror ("ERROR: sem_unlink() sync failed");
    }
    printf ("sem_unlink() sync returned %d\n", rtnval);
    printf ("Calling close(%#x)", shm_fd);
    #endif

    rtnval = close (shm_fd);

    #ifdef VERBOSE
    if (rtnval != 0)
    {
        perror ("ERROR: close() failed");
    }
    printf (" -> returned %d\n", rtnval);
    printf ("Calling shm_unlink('%s')", shm_name);
    #endif

    rtnval = shm_unlink (shm_name);

    #ifdef VERBOSE
    if (rtnval != 0)
    {
        perror ("ERROR: shm_unlink() failed");
    }
    printf (" -> returned %d\n", rtnval);
    #endif
}

void createMask(int pid)
{
    switch(pid)
    {
        case 0 : 
            mask = 0x1;
            break;
        case 1 : 
            mask = 0x2;
            break;
        case 2 : 
            mask = 0x4;
            break;
        case 3 : 
            mask = 0x8;
            break;
    }
    #ifdef VERBOSE
    printf ("Mask 0x%02x created\n", mask);
    #endif
}

void
statement_1(int pid)
{
    printf("Stament 1 from proces %d\n", pid);
}

void
statement_2(int pid)
{
    printf("Stament 2 from proces %d\n", pid);
}

int 
main(int argc, char * argv[])
{  
    int     pid, initresult;

    if (argc != 2)
    {
        fprintf (stderr, "Usage: %s <proces-number 0..4>\n", argv [0]);
        return (-1);
    }

    pid = atoi(argv[1]);
    #ifdef VERBOSE
    printf ("Starting up process %d\n", pid);
    #endif

    if(pid == 0)
    {
        initSemSHM(pid);
    }else{
        openSemSHM(pid);
    }
    createMask(pid);
    statement_1(pid);

    // enter Critical Section
    sem_wait(sem_flags);
    // Read flags from shm
    flags = shm_addr[0];
    #ifdef VERBOSE
    printf ("Flags in store are 0x%02x\n", flags);
    #endif
    // mask flags
    flags = flags | mask;
    #ifdef VERBOSE
    printf ("Flags after masking 0x%02x\n", flags);
    #endif
    // update flag in shm
    shm_addr[0] = flags;
    #ifdef VERBOSE
    printf ("Flags are 0x%02x in process %d\n", flags, pid);
    #endif
    // leave Critical Section
    sem_post(sem_flags);

    // Check the flags, do sem_post(sem_sync) or sem_wait(sem_sync)
    if(flags == 0x0F){
        sem_post(sem_sync);
    }else{
        sem_wait(sem_sync);
    }
    sem_post(sem_sync);
    statement_2(pid);

    // Clean up   
    cleanupSemSHM(pid);
}
