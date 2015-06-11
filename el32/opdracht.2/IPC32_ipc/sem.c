/*
** These examples use semaphores to ensure that writer and reader
** processes have exclusive, alternating access to the shared-memory region.
*/

/*
** Compile with: gcc sem.c -o sem -pthread -lrt -lm
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

static char * 
remove_nl (char * s)
{
    int len;
    
    len = strlen (s);
    if (s [len - 1] == '\n')
    {
        s [len - 1] = '\0';
    }
    
    return (s);
}


int 
octal_decimal(int n)
{ 
    int decimal=0, i=0, rem; 

    while (n != 0) 
    { 
        rem = n%10; 
        n /= 10; 
        decimal += rem * pow(8,i); 
        i++; 
    } 
    return decimal; 
}

int main(void)
{
    sem_t *     semdes = SEM_FAILED;
    char        line[80];
    char        sem_name[80];
    char        msg[80];
    int         value;
    int         permissions;
    int         choice = ' ';
    int         rtnval;
    int         initval;
    int         urights;

    while (choice != 'q')
    {
        printf ("\nMenu\n"
                "========\n"
                "   [n]    create new semaphore\n"
                "   [o]    open existing semaphore\n"
                "   [p]    post\n"
                "   [t]    trywait\n"
                "   [w]    wait\n"
                "   [g]    getvalue\n"
                "   [c]    close\n"
                "   [u]    unlink\n"
                "   [q]    quit\n"
                "Enter choice: "
               );
        fgets (line, sizeof (line), stdin);
        choice = line[0];
        printf ("\n");
        
        switch (choice)
        {
            case 'n':
                if (semdes != SEM_FAILED)
                {
                    printf ("ERROR: another semaphore already opened\n");
                    break;
                }
                printf ("Enter name: ");
                fgets  (sem_name, sizeof (sem_name), stdin);
                remove_nl (sem_name);

                printf( "Enter init value :");
                fgets (line, sizeof (line), stdin);
                initval = atoi(line);

                printf( "Enter permissions (Owner, Group, Other) i.e. 666 :");
                fgets (line, sizeof (line), stdin);
                urights = atoi(line);

                printf ("Calling sem_open('%s', O_CREAT | O_EXCL, 0%d, %d)\n", sem_name, urights, initval);
                semdes = sem_open (sem_name, O_CREAT | O_EXCL, octal_decimal(urights), initval);
                
                if (semdes == SEM_FAILED)
                {
                    perror ("ERROR: sem_open() failed");
                }
                printf ("sem_open() returned %#x\n", (unsigned int)semdes);
                break;
            case 'o':
                if (semdes != SEM_FAILED)
                {
                    printf ("ERROR: another semaphore already opened\n");
                    break;
                }
                printf ("Enter name: ");
                fgets  (sem_name, sizeof (sem_name), stdin);
                remove_nl (sem_name);

                printf ("Calling sem_open('%s', 0)\n", sem_name);
                semdes = sem_open (sem_name, 0);
                
                if (semdes == SEM_FAILED)
                {
                    perror ("ERROR: sem_open() failed");
                }
                printf ("sem_open() returned %#x\n", (unsigned int)semdes);
                break;
            case 'c':
                if (semdes == SEM_FAILED)
                {
                    printf ("ERROR: no open semaphore\n");
                    break;
                }
                
                printf ("Calling sem_close(%#x)\n", (unsigned int) semdes);
                rtnval = sem_close (semdes);
                if (rtnval != 0)
                {
                    perror ("ERROR: sem_close() failed");
                }
                printf ("sem_close() returned %d\n", rtnval);
                semdes = SEM_FAILED;
                break;
            case 'p':
                if (semdes == SEM_FAILED)
                {
                    printf ("ERROR: no open semaphore\n");
                    break;
                }
                
                printf ("Calling sem_post(%#x)\n", (unsigned int) semdes);
                rtnval = sem_post (semdes);
                if (rtnval != 0)
                {
                    perror ("ERROR: sem_post() failed");
                }
                printf ("sem_post() returned %d\n", rtnval);
                break;
            case 't':
                if (semdes == SEM_FAILED)
                {
                    printf ("ERROR: no open semaphore\n");
                    break;
                }
                
                printf ("Calling sem_trywait(%#x)\n", (unsigned int) semdes);
                rtnval = sem_trywait (semdes);
                if (rtnval != 0)
                {
                    perror ("ERROR/WARNING: sem_trywait() failed");
                }
                printf ("sem_trywait() returned %d\n", rtnval);
                break;
            case 'w':
                if (semdes == SEM_FAILED)
                {
                    printf ("ERROR: no open semaphore\n");
                    break;
                }
                
                printf ("Calling sem_wait(%#x)\n", (unsigned int) semdes);
                rtnval = sem_wait (semdes);
                if (rtnval != 0)
                {
                    perror ("ERROR: sem_wait() failed");
                }
                printf ("sem_wait() returned %d\n", rtnval);
                break;
            case 'g':
                if (semdes == SEM_FAILED)
                {
                    printf ("ERROR: no open semaphore\n");
                    break;
                }
                printf ("Calling sem_getvalue(%#x)\n", (unsigned int) semdes);
                rtnval = sem_getvalue (semdes, &value);
                if (rtnval != 0)
                {
                    perror ("ERROR: sem_getvalue() failed");
                }
                printf ("sem_getvalue() returned %d; value:%d\n", rtnval, value);
                break;
            case 'u':
                printf ("Enter name: ");
                fgets  (sem_name, sizeof (sem_name), stdin);
                remove_nl (sem_name);
                printf ("Calling sem_unlink('%s')\n", sem_name);
                rtnval = sem_unlink (sem_name);
                if (rtnval != 0)
                {
                    perror ("ERROR: sem_unlink() failed");
                }
                printf ("sem_unlink() returned %d\n", rtnval);
                break;
            case 'q':
                if (semdes != SEM_FAILED)
                {
                    printf ("WARNING: a semaphore still opened\n");
                }
                break;
            default:
                printf ("Invalid option '%c' (%#x)\n", choice, choice);
                break;
        }
    }
}
