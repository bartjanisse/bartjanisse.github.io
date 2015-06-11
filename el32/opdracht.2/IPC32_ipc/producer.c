/*
** (C) 2015, A.W. Janisse
** 22 maart 2015
**
**
**
** Compile with: gcc producer.c -o producer -pthread -lrt
*/

//#define VERBOSE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <termios.h>
#include <unistd.h>

char        	shm_name[]      = "buffer"; 
char         	sem_e_name[]    = "e";
char         	sem_s_name[]    = "s";
char         	sem_n_name[]    = "n";
sem_t *     	sem_e           = SEM_FAILED;
sem_t *     	sem_s           = SEM_FAILED;
sem_t *     	sem_n           = SEM_FAILED;
unsigned long *	shm_buffer      = MAP_FAILED; 
unsigned long	data			= 0L;
int				in 				= 0;
int 			size;
useconds_t		workdelay;

sem_t *
createSemaphore(char name[], int initval)
{
	sem_t * sem	= SEM_FAILED;

	sem = sem_open (name, O_CREAT | O_EXCL, 0600, initval);
    if (sem == SEM_FAILED){
        perror ("ERROR: sem_open() failed");
    } 
   	return (sem);
}

int
createSHM(char name[])
{
	int 	fd;

	fd = shm_open (name, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd == -1)
    {
        perror ("ERROR: shm_open() failed\n");
    }
    return (fd);
}

unsigned long *
mapSHM(int fd, size_t bytesize)
{
	unsigned long * shm_addr;
	int 			rtnval;

	rtnval = ftruncate (fd, bytesize);
    if (rtnval != 0)
    {
        perror ("ERROR: ftruncate() failed\n");
    }

	shm_addr = mmap (NULL, bytesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shm_addr == MAP_FAILED)
    {
        perror ("ERROR: mapSHM() failed\n");
    }
    return (shm_addr);
}

void
dumpBuffer()
{
	int 	i;

	for (i = 0; i < size; ++i)
	{
		printf ("%ld ", (unsigned long) shm_buffer[i]);
	}
	printf ("\n");
}

void
resetAll()
{
	int 	i;

	for (i = 0; i < size; ++i)
	{
		shm_buffer[i] = 0L;
	}
	data = 0L;
	in = 0;
	printf ("All reset ok");
}

void 
append()
{
	int 			idx;
	unsigned long 	current;

	idx = in % size;

	current = shm_buffer[idx];
	if(current != 0L)
	{
		perror("Position to write not 0");
	}

	shm_buffer[idx] = ++data;	
	in++;

	dumpBuffer();
}

void
work()
{
	sem_wait(sem_e);
	sem_wait(sem_s);

	append();

	sem_post(sem_s);
	sem_post(sem_n);

	usleep(workdelay);
}

int 
mygetch( ) {
	int 		character;
    struct 		termios orig_term_attr;
    struct 		termios new_term_attr;

    /* set the terminal to raw mode */
    tcgetattr(fileno(stdin), &orig_term_attr);
    memcpy(&new_term_attr, &orig_term_attr, sizeof(struct termios));
    new_term_attr.c_lflag &= ~(ECHO|ICANON);
    new_term_attr.c_cc[VTIME] = 0;
    new_term_attr.c_cc[VMIN] = 0;
    tcsetattr(fileno(stdin), TCSANOW, &new_term_attr);
    character = fgetc(stdin);
    tcsetattr(fileno(stdin), TCSANOW, &orig_term_attr);

    return character;
}

int 
main(int argc, char const *argv[])
{
	int 		bufsize, delay, rtnval;
	int			shm_fd 	= -1;
	char        line[80];
	int         choice = ' ';

	if (argc != 2)
    {
        fprintf (stderr, "Usage: %s <buffersize 1..10000>\n", argv [0]);
        exit(EXIT_FAILURE);
    }

    size = atoi(argv[1]);

    if(size < 1 || size > 10000)
    {
        perror ("Wrong buffersize <buffersize 1..10000>\n");
        exit(EXIT_FAILURE);
    }
    /* Create the buffer */
	bufsize = sizeof(data) * size;
	shm_fd = createSHM(shm_name);
	shm_buffer = mapSHM(shm_fd, bufsize);
	/* Create semphores */
	sem_e = createSemaphore(sem_e_name, size);
	sem_n = createSemaphore(sem_n_name, 0);
	sem_s = createSemaphore(sem_s_name, 1);
	
	while (choice != 'q')
    {
        printf ("\n   Menu (please be aware that program maybe blocked)\n"
                "   =================================================\n"
                "   [s]    start producing\n"
                "   [t]    set producing delay\n"
                "   [d]    dump buffer\n"
                "   [r]    reset all\n"
                "   [q]    quit\n"
                "\n   Enter choice: "
               );
        fgets (line, sizeof (line), stdin);
        choice = line[0];
        printf ("\n");

        switch (choice)
        {
            case 's':
				printf("Press a p to pause producing\n");
				do {
				    work();
				} while (mygetch() != 'p');
            	break;

            case 't':
            	printf("Enter a positive delay value (x10ms)\n");
				scanf ("%d",&delay);
				workdelay = 10000 * delay;
				while (getchar()!='\n');
            	break;

            case 'd':
            	dumpBuffer();
            	break;

            case 'r':
            	resetAll();
            	break;

			case 'q': 
				break;
           	
           	default:
                printf ("Invalid option '%c' (%#x)\n", choice, choice);
                break;
        }
    }

    /* Cleanup the shared memory and semaphores */
    if (close (shm_fd) != 0)
        perror ("ERROR: close() failed");

    if (shm_unlink(shm_name) != 0)
	    perror ("ERROR: shm_unlink() failed");
 
    if (sem_unlink (sem_e_name) != 0)
        perror ("ERROR: sem_unlink(sem_e) failed");

    if (sem_unlink (sem_s_name) != 0)
        perror ("ERROR: sem_unlink(sem_s) failed");

    if (sem_unlink (sem_n_name) != 0)
        perror ("ERROR: sem_unlink(sem_n) failed");
    	
	exit(EXIT_SUCCESS);
}