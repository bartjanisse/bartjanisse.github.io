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
	int size;

	// Open the shared memory
    shm_addr = my_shm_open (shm_name, &size);

    wait_for_turn(1);
	printf ("2\n");
	set_turn(0);

	wait_for_turn(1);
	printf ("4\n");
	set_turn(0);

	wait_for_turn(1);
	printf ("6\n");
	set_turn(0);

	return 0;
}