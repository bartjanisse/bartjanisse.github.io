/*
 * Dit programma opent en shared memory en wacht tot er een kleine letter a vooraan staat, als
 * dit zo is dan leest het de rest van het shared memory. Hierna maakt van de kleine letter a een hoofdletter A en stopt.
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
    int         rtnval, size, cntr = 0, index = 0;

    #define     TIME_OUT        20  // Time-out in seconds while waiting for character a    

    // 1: Open the shared memory
    printf("Opening shared memory\n");
    shm_addr = my_shm_open (shm_name, &size);
    printf("Succesfull opened shared memory. Size is: %d\n", size);

    // 2: Wait for character a
    printf("Waiting for 'a'\n");
    do {
        printf("Reading from shared memory '%c'\n", shm_addr[0]);
        sleep(1);
        cntr++;
    } while ((shm_addr[0] != 'a') & (cntr < TIME_OUT));

    if(cntr >= TIME_OUT) {
        printf("Time-out occured, waited for %d seconds\n", cntr);
    } else {
        printf("Succesfull read char 'a'.\n");
    }

    // 3: Read all the characters
    printf("Reading back: ");
    for (index = 0; index <= size; index++) 
    {
        printf("%c", shm_addr[index]);
    }
    // 4: Write capital A
    printf("\nWriting captital 'A'");
    shm_addr[0] = 'A';

    // 5: Close the program
    printf ("\nBye bye. Hope to see you soon.\n");
    return 0;
}
