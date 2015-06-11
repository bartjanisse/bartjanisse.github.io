
/*
 * Dit programma creëert een stuk shared memory, schrijft dit vol met het alfabet (in kleine letters),
 * wacht tot er een hoofdletter A aan het begin staat, verwijdert het shared memory en stopt.
 */
 #define VERBOSE

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include "my_shm.h"

int main(void)
{
    char *      shm_addr = MAP_FAILED;
    char        letter, shm_name[] = "shm_test";
    int         rtnval, cntr = 0, index = 0;

    #define     SHM_SIZE        26  // Size needed tot store the alfabet
    #define     TIME_OUT        60  // Time-out in seconds while waiting for capital A

    // 1: Create a shared memory.
    shm_addr = my_shm_create (shm_name, SHM_SIZE);
    printf ("Shared memory name '%s' is created. \n", shm_name);
    
    // 2: Write a...z into the shared memory
    printf("Writing alfabet: ");
    for (letter = 'a'; letter <= 'z'; letter++) 
    {
        printf("%c", letter);
        shm_addr[index] = letter;
        index++;
    }
    printf ("\nThere are %d characters written.\n", index);

    // 3: Read the first char and wait till it is 'A'
    printf("Waiting for 'A'\n");
    do {
        printf("Reading from shared memory '%c'\n", shm_addr[0]);
        sleep(1);
        cntr++;
    } while ((shm_addr[0] != 'A') & (cntr < TIME_OUT));

    if(cntr >= TIME_OUT) {
        printf("Time-out occured, waited for %d seconds\n", cntr);
    } else {
        printf("Succesfull read capital 'A', starting clean up.\n");
    }

    // 4: Close the shared memory
    printf ("Calling close(%#x)", shm_fd);
    rtnval = close (shm_fd);
    if (rtnval != 0)
    {
        perror ("ERROR: close() failed");
    }
    printf (" -> returned %d\n", rtnval);
    shm_fd = -1;

    // 5: Unlink the shared memory so it can be removed (reference counting)
    printf ("Calling shm_unlink('%s')", shm_name);
    rtnval = shm_unlink (shm_name);
    if (rtnval != 0)
    {
        perror ("ERROR: shm_unlink() failed");
    }
    printf (" -> returned %d\n", rtnval);

    // 6: Close the program
    printf ("\nBye bye. Hope to see you soon.\n");
    return 0;
}