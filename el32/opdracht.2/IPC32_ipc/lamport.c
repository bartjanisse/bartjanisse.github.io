#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <sched.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define SHM_NAME        "lamport"
#define    NROF_CLIENTS    10

typedef struct
{
    // TODO
    
    pid_t       pid [NROF_CLIENTS];
    int         cs_counter;
} LAMPORT_INFO;

static LAMPORT_INFO *   c;
static int              shm_fd = -1;

static long long
convert_time (struct timespec * ts)
{
    // convert 'ts' to microseconds
    long long   nsec;
    long long   sec;

    // ensure that all items in the expression are 'long long'
    nsec = ts->tv_nsec;
    sec  = ts->tv_sec;
    return (nsec / 1000LL + sec * 1000000LL);
}

static long long
get_time (void)
{
    // get the elapsed time (in microseconds)
    struct timespec ts;
    
    clock_gettime (CLOCK_REALTIME, &ts);
    return (convert_time (&ts));
}

static long long
get_cputime (void)
{
    // get the consumed CPU-time (of this process)
    struct timespec ts;
    
    clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &ts);
    return (convert_time (&ts));
}

static long long
get_consumed_cputime (void)
{
    // get percent of CPU-time that this process has consumed (since last call)
    static long long    prev_cpu = 0;
    static long long    prev_time = 0;
    long long           curr_cpu;
    long long           curr_time;
    long long           result;

    curr_time = get_time();
    curr_cpu  = get_cputime();
    
    result  = (curr_cpu - prev_cpu);
    result *= 100LL;  // to get percent
    result /= (curr_time - prev_time);
    
    prev_cpu  = curr_cpu;
    prev_time = curr_time;
    
    return (result);
}

static void
another_cycle (void)
{
    static long long    counter = 0;
    static long long    prev_time = 0;
    long long           curr_time;
    
    counter++;
    curr_time = get_time();
    if (curr_time >= prev_time + 1000000LL)
    {
        // each second...
        printf ("%lld%%  %lld\n", get_consumed_cputime(), counter);
        prev_time = curr_time;
        counter = 0;
    }
}

static char *
my_shm_open (char * shm_name, int size)
{
    int     rtnval;
    char *  shm_addr;
    
    shm_fd = shm_open (shm_name, O_CREAT | O_RDWR, 0600);
    if (shm_fd == -1)
    {
        perror ("ERROR: shm_open() failed");
    }

    rtnval = ftruncate (shm_fd, size);
    if (rtnval != 0)
    {
        perror ("ERROR: ftruncate() failed");
    }

    shm_addr = mmap (NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_addr == MAP_FAILED)
    {
        perror ("ERROR: mmap() failed");
    }

    return (shm_addr);
}

static void
my_shm_close (char * shm_name)
{
    int     rtnval;

    rtnval = close (shm_fd);
    if (rtnval != 0)
    {
        perror ("ERROR: close() failed");
    }
    rtnval = shm_unlink (shm_name);
    if (rtnval != 0)
    {
        perror ("ERROR: shm_unlink() failed");
    }
}

static void 
run_master(void)
{
    char *            shm_addr;
    char            line [100];
    int                i;
    
    shm_addr = my_shm_open(SHM_NAME, sizeof (LAMPORT_INFO));
    c = (LAMPORT_INFO *) shm_addr;
    memset (c, sizeof (LAMPORT_INFO), 0);

    printf ("shared memory has been created for %d clients.\n"
            "clients can be started right now (in separate console windows)\n\n"
            "after a while: enter <return> to stop this master process, \n"
            "to stop the client processes and to remove the shared memory\n", 
            NROF_CLIENTS);
    fgets (line, sizeof (line), stdin);     // wait for <return>

    printf ("\nkilling clients, removing shared memory and exiting...\n");
    for (i = 0; i < NROF_CLIENTS; i++)
    {
        if (c->pid [i] != 0)
        {
            if (kill (c->pid[i], SIGTERM) != 0)
            {
                perror ("kill()");
            }
        }
    }
    my_shm_close(SHM_NAME);
}

static void
check_mutual_exclusion (void)
{
    if (c->cs_counter != 1)
    {
        fprintf (stderr, "ENTRY: too many clients in CS\n");
    }
}


static int
getmax (void)
{
    // TODO;
}

static bool
higher_priority (int i, int j)
{
    // return true if process i has a higher Lamport priority for entering the Critical Section
    // else return false
    
    // TODO;
}

static void
lock (int i) 
{
    // TODO
    
    // gebruik de functies getmax() en higher_priority() (die je hierboven nog moet implementeren)
    
    // zorg dat deze regel aanwezig is: c->cs_counter++;
}


static void
unlock (int i) 
{
    // TODO
    
    // zorg dat deze regel aanwezig is: c->cs_counter--;
}


static void 
run_client (int client_id)
{
    char *                 shm_addr;

    printf("client %d\n", client_id);
    
    shm_addr = my_shm_open(SHM_NAME, sizeof (LAMPORT_INFO));
    c = (LAMPORT_INFO *) shm_addr;
    
    c->pid [client_id] = getpid();
    
    while (true)
    {
        lock (client_id);
        
        // The Critical Section:
        check_mutual_exclusion();
        another_cycle();
        
        unlock (client_id);
    }
    
    my_shm_close(SHM_NAME);    
}


extern int 
main (int argc, char * argv[])
{
    int     opt;
    bool     is_client = false;
    int     client_id = -1;

    while ((opt = getopt(argc, argv, "c:h")) != -1)
    {
        switch (opt)
        {
            case 'c':
                is_client = true;
                client_id = atoi(optarg);
                break;
            case '?':
            case 'h':
                printf("\noptions:\n"
                    "    -c <id>    client with an unique <id> [default:master]\n"
                    "\n");
                exit(0);
                break;
            default:
                fprintf(stderr, "ERROR: unknown option '%c'\n", opt);
                exit(1);
                break;
        }
    }
    for (; optind < argc; optind++)
    {
        fprintf(stderr, "ERROR: superfluous argument '%s'\n", argv [optind]);
        exit(1);
    }

    if (is_client == true)
    {
        if ((client_id < 0) || (client_id >= NROF_CLIENTS))
        {
            fprintf(stderr, "ERROR: client-id %d invalid (0..%d)\n", 
                    client_id, NROF_CLIENTS-1);
            exit(1);
        }

        run_client(client_id);
    } 
    else
    {
        run_master();
    }

    return (0);
}

/* End of file */
