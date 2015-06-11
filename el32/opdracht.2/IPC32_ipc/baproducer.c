/*
** (C) 2015, A.W. Janisse
** 22 maart 2015
**
**
**
** Compile with: gcc baproducer.c -o baproducer -pthread -lrt
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
char 			shm_pc_name[] 	= "pc";
char         	sem_e_name[]    = "e";
char         	sem_s_name[]    = "s";
char         	sem_n_name[]    = "n";
sem_t *     	sem_e           = SEM_FAILED;
sem_t *     	sem_s           = SEM_FAILED;
sem_t *     	sem_n           = SEM_FAILED;
unsigned long *	shm_buffer      = MAP_FAILED; 
unsigned long *	shm_pc	    	= MAP_FAILED; 
unsigned long	data			= 0L;
int				in 				= 0;
int 			size;

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

sem_t*
openSemaphore(char name[])
{
	sem_t *     sem = SEM_FAILED;

	sem = sem_open (name, 0);
                
    if (sem == SEM_FAILED)
    {
        perror ("ERROR: sem_open() failed");
        exit(EXIT_FAILURE);
    }
    return(sem);
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

int
openSHM(char name[])
{
	int 	fd;

	fd = shm_open (name, O_RDWR, 0600);
    if (fd == -1)
    {
        perror ("ERROR: shm_open() failed");
        exit(EXIT_FAILURE);
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
	unsigned long 	current, new, in;

	/* read the buffer index from the shared memory */
	in = shm_pc[1];

	/* read de value from the shared memory */
	new = shm_pc[0];
	new++;

	/* read the current value from the buffer and check for 0 */
	current = shm_buffer[in % size];
	if(current != 0L)
	{
		perror("Position to write not 0");
	}

	/* write new value into buffer and increment the index */
	shm_buffer[in % size] = new;
	in++;

	/* save buffer index and value for next use */
	shm_pc[0] = new;
	shm_pc[1] = in;

	dumpBuffer();
}

void
work()
{
	int 		rnd;
	int 		max = 100000;
	int 		min = 0;
	useconds_t	workdelay;

	sem_wait(sem_e);
	sem_wait(sem_s);

	append();

	sem_post(sem_s);
	sem_post(sem_n);

	rnd = (rand() % (max+1-min))+min;
	usleep(rnd);
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
	int 		bufsize, delay;
	int			shm_fd 		= -1;
	int 		shm_pc_fd 	= -1;
	int         choice 		= ' ';
	char        line[80];

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
    bufsize = sizeof(data) * size;

	while (choice != 'q' && choice != 'Q')
    {
        printf ("\n   Menu (please be aware that program maybe blocked)\n"
                "   =================================================\n"
                "   [c]    create semphores and SHM's\n"
                "   [o]    open semaphores end SHM's\n"
                "   [s]    start producing\n"
                "   [q]    quit\n"
                "   [Q]    quit and cleanup\n"
                "\n   Enter choice: "
               );
        fgets (line, sizeof (line), stdin);
        choice = line[0];
        printf ("\n");

        switch (choice)
        {
        	case 'c':
			    /* Create the buffer */
				
				shm_fd = createSHM(shm_name);
				shm_buffer = mapSHM(shm_fd, bufsize);
				/* Create producers/consumers SHM */
				shm_pc_fd = createSHM(shm_pc_name);
				shm_pc = mapSHM(shm_pc_fd, sizeof(data) * 3);
				/* Create semphores */
				sem_e = createSemaphore(sem_e_name, size);
				sem_n = createSemaphore(sem_n_name, 0);
				sem_s = createSemaphore(sem_s_name, 1);
				break;
        	case 'o':
				/* Open the buffer SHM*/
				shm_fd = openSHM(shm_name);
				shm_buffer = mapSHM(shm_fd, bufsize);
				/* calculate the size of the buffer */
				size = lseek (shm_fd, 0, SEEK_END) / sizeof(data);
				
				/* Open the producer/consumer SHM */
				shm_pc_fd = openSHM(shm_pc_name);
				shm_pc = mapSHM(shm_pc_fd, sizeof(data) * 3);
				

				/* Open the semaphores */
				sem_e = openSemaphore(sem_e_name);
				sem_n = openSemaphore(sem_n_name);
				sem_s = openSemaphore(sem_s_name);
				break;

            case 's':
				printf("Press a p to pause producing\n");
				do {
				    work();
				} while (mygetch() != 'p');
            	break;

			case 'q': 
				break;

			case 'Q':
			    /* Cleanup the shared memory and semaphores */
				if (close (shm_fd) != 0)
			        perror ("ERROR: close() failed");

			    if (shm_unlink(shm_name) != 0)
				    perror ("ERROR: shm_unlink() failed");

				if (close (shm_pc_fd) != 0)
			        perror ("ERROR: close() failed");

			    if (shm_unlink(shm_pc_name) != 0)
				    perror ("ERROR: shm_unlink() failed");
			 
			    if (sem_unlink (sem_e_name) != 0)
			        perror ("ERROR: sem_unlink(sem_e) failed");

			    if (sem_unlink (sem_s_name) != 0)
			        perror ("ERROR: sem_unlink(sem_s) failed");

			    if (sem_unlink (sem_n_name) != 0)
			        perror ("ERROR: sem_unlink(sem_n) failed"); 
				break;
           	
           	default:
                printf ("Invalid option '%c' (%#x)\n", choice, choice);
                break;
        }
    }    	
	exit(EXIT_SUCCESS);
}