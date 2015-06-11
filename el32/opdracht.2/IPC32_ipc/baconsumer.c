/*
** (C) 2015, A.W. Janisse
** 22 maart 2015
**
**
**
** Compile with: gcc baconsumer.c -o baconsumer -pthread -lrt
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
int				out 			= 0;
int 			size;
useconds_t		workdelay;

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
mapSHM(int fd)
{
	unsigned long * shm_addr;
	int 			rtnval, bufsize;

	bufsize = lseek (fd, 0, SEEK_END);

	shm_addr = mmap (NULL, bufsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shm_addr == MAP_FAILED)
    {
        perror ("ERROR: mapSHM() failed\n");
        exit(EXIT_FAILURE);
    }
    return (shm_addr);
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

void
take()
{
	unsigned long current, out; 

	/* read the buffer index from the shared memory */
	out = shm_pc[2];

	/* read current value from the buffer */
	current = shm_buffer[out % size];

	/* clear the buffer */
	shm_buffer[out % size] = 0L;

	/* increment the index and save for next use */
	out++;
	shm_pc[2] = out;

	printf("Index = %ld data = %ld\n", out % size, current);
}

void 
work()
{
	int 		rnd;
	int 		max = 100000;
	int 		min = 0;
	useconds_t	workdelay;

	sem_wait(sem_n);
	sem_wait(sem_s);

	take();

	sem_post(sem_s);
	sem_post(sem_e);

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
	int 		delay;
	int			shm_fd 		= -1;
	int 		shm_pc_fd 	= -1;
	int         choice 		= ' ';
	char        line[80];

	/* Open the buffer */
	shm_fd = openSHM(shm_name);
	shm_buffer = mapSHM(shm_fd);
	/* calculate the size of the buffer */
	size = lseek (shm_fd, 0, SEEK_END) / sizeof(data);

	/* Open the producer/consumer SHM */
	shm_pc_fd = openSHM(shm_pc_name);
	shm_pc = mapSHM(shm_pc_fd);

	/* Open the semaphores */
	sem_e = openSemaphore(sem_e_name);
	sem_n = openSemaphore(sem_n_name);
	sem_s = openSemaphore(sem_s_name);

	do {
	    work();
	} while (mygetch() != 'p');

	// while (choice != 'q')
 //    {
 //        printf ("\n   Menu (please be aware that program maybe blocked\n"
 //                "   =================================================\n"
 //                "   [s]    start consuming\n"
 //                "   [q]    quit\n"
 //                "\n   Enter choice: "
 //               );
 //        fgets (line, sizeof (line), stdin);
 //        choice = line[0];
 //        printf ("\n");

 //        switch (choice)
 //        {
 //            case 's':
	// 			printf("Press a p to pause consuming\n");
	// 			do {
	// 			    work();
	// 			} while (mygetch() != 'p');
 //            	break;

 //            case 't':
 //            	printf("Enter a positive delay value (x10ms)\n");
	// 			scanf ("%d",&delay);
	// 			workdelay = 10000 * delay;
	// 			while (getchar()!='\n');
 //            	break;

	// 		case 'q': break;
           	
 //           	default:
 //                printf ("Invalid option '%c' (%#x)\n", choice, choice);
 //                break;
 //        }
 //    }

	exit(EXIT_SUCCESS);
}