/*
** (C) 2015, A.W. Janisse
** 22 maart 2015
**
**
**
** Compile with: gcc sched.c -o sched -lrt
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sched.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

sem_t *     sem_sync            = SEM_FAILED;
char        sem_sync_name[]     = "sync";

#define MAX_SIZE 1000000

static struct timespec ts[MAX_SIZE];

extern int
main (int argc, char * argv[])
{
    long delta;
    struct timespec tspec;
    struct sched_param sp;
    int pid, idx;

    if (argc != 2)
    {
        fprintf (stderr, "Usage: %s <proces-number 0..4>\n", argv [0]);
        return (-1);
    }
    pid = atoi(argv[1]);
    // Set scheduler and priority
    sp.sched_priority = 1;
    if(sched_setscheduler(0, SCHED_RR, &sp) != 0)
    {
        perror("Error: sched_setscheduler\n");
    }
    // Synchronize
    if(pid == 0)    {
        sem_sync = sem_open (sem_sync_name, O_CREAT | O_EXCL, 0600, 0);
        sem_wait(sem_sync);
    }else{
        sem_sync = sem_open (sem_sync_name, 0);
        sem_post(sem_sync);
    }
    // Collect time and store in array
    for (idx = 0; idx < MAX_SIZE; ++idx)
    {

        clock_gettime(CLOCK_REALTIME, &ts[idx]);
        usleep(10);
    }
    // Printout time data
    for (idx = 1; idx < MAX_SIZE; ++idx)
    {
        printf("%lu.%lu, delta = %lu", ts[idx].tv_sec, ts[idx].tv_nsec, ts[idx].tv_nsec - ts[idx-1].tv_nsec);
        if(ts[idx].tv_nsec - ts[idx-1].tv_nsec > 10000)
        {
            printf("  <-------------------------\n");
        }
        else
        {
            printf("\n");
        }
    }
    // Get RR interval
    if(sched_rr_get_interval(0, &tspec) != 0)
    {
        perror("Error: Retreiving sched_rr_get_interval\n");
    }
    printf("Timeslice: %lums\n", tspec.tv_nsec / 1000L / 1000L);
    // Close and unlink semafore
    sem_close (sem_sync);
    sem_unlink (sem_sync_name);

    exit(EXIT_SUCCESS);
}

