#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include "my_shm.h"

#define     SHM_SIZE        1  	// Size needed tot store flag
#define     TIME_OUT        60  // Time-out in seconds while waiting for capital A

char 		shm_name[] = "shm_dekkers";
char * 		shm_addr = MAP_FAILED;

void 
wait_for_turn(int turn)
{
	while(shm_addr[0] != turn){}
}

void 
set_turn(int turn)
{
	shm_addr[0] = turn;
}

extern int 
main (int argc, char * argv[])
{
    int     rtnval;

	#ifdef VERBOSE
        printf ("Verbose output\n");
    #endif

    // Create a shared memory.
    shm_addr = my_shm_create (shm_name, SHM_SIZE);
    printf ("\nShared memory name '%s' is created. \n\n", shm_name);
    set_turn(0);

    wait_for_turn(0);
	printf ("1\n");
	set_turn(1);

	wait_for_turn(0);
	printf ("3\n");
	set_turn(1);

	wait_for_turn(0);
	printf ("5\n");
	set_turn(1);

	wait_for_turn(0);

	// Close the shared memory
    printf ("\nCalling close(%#x)", shm_fd);
    rtnval = close (shm_fd);
    if (rtnval != 0)
    {
        perror ("ERROR: close() failed");
    }
    printf (" -> returned %d\n", rtnval);
    shm_fd = -1;

    // Unlink the shared memory so it can be removed (reference counting)
    printf ("Calling shm_unlink('%s')", shm_name);
    rtnval = shm_unlink (shm_name);
    if (rtnval != 0)
    {
        perror ("ERROR: shm_unlink() failed");
    }
    printf (" -> returned %d\n", rtnval);

    // Close the program
    printf ("\nBye bye. Hope to see you soon.\n");
    return 0;	
}