/*
** (C) 2015, A.W. Janisse
** 16 maart 2015
**
**
**
** Compile with: gcc semseq.c -o semsex -pthread -lrt -lm
*/

/**********  writer.c  ***********/

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

char        this_sem_name[2];
char        next_sem_name[2];
char        prnchars[2];
sem_t *     this_semdes = SEM_FAILED;
sem_t *     next_semdes = SEM_FAILED;

int
init_params(int pid) 
{
    int retval = 0;

    switch(pid)
    {
        case 0:     
            strcpy(this_sem_name, "A");
            strcpy(next_sem_name, "B");
            prnchars[0] = '1';
            prnchars[1] = '5';
            break;

        case 1:     
            strcpy(this_sem_name, "B");
            strcpy(next_sem_name, "C");
            prnchars[0] = '2';
            prnchars[1] = '6';
            break;

        case 2:     
            strcpy(this_sem_name, "C");
            strcpy(next_sem_name, "D");
            prnchars[0] = '3';
            prnchars[1] = '7';
            break;

        case 3:     
            strcpy(this_sem_name, "D");
            strcpy(next_sem_name, "A");
            prnchars[0] = '4';
            prnchars[1] = '8';
            break;

        default:    
            printf ("Process id %d is not valid\n", pid);
            retval = -1;
    }
    
    this_semdes = sem_open (this_sem_name, 0);    
    if (this_semdes == SEM_FAILED)
    {
        perror ("ERROR: this_sem_open() failed");
    }

    next_semdes = sem_open (next_sem_name, 0);    
    if (next_semdes == SEM_FAILED)
    {
        perror ("ERROR: next_sem_open() failed");
    }

    printf("Proces name = %s\n", this_sem_name);
    return retval;
}

int 
main(int argc, char * argv[])
{
    
    int         initresult;

    if (argc != 2)
    {
        fprintf (stderr, "Usage: %s <proces-number 0..4>\n", argv [0]);
        return (-1);
    }

    initresult = init_params(atoi(argv[1]));

    if(initresult < 0)
    {
        perror("Initialize fatal error");
        return(-1);
    }
        
    while(1) 
    {
        sem_wait (this_semdes);
        printf("%c\n", prnchars[0]);
        sem_post(next_semdes);
        sem_wait(this_semdes);
        printf("%c\n", prnchars[1]);
        sem_post(next_semdes);
    }
}
