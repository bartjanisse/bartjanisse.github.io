#include <pthread.h>
#include <stdlib.h>     // for exit()
#include <unistd.h>     // close()

#include "Auxiliary.h"
#include "AcceptTCPConnection.h"
#include "CreateTCPServerSocket.h"
#include "HandleTCPClient.h"

static void * myThread (void * arg);            /* thread that does the work */

int main (int argc, char *argv[])
{
    int         servSock;     /* Socket descriptor for server */
    int         clntSock;     /* Socket descriptor for client */
    int         result;       /* Result for thread creation */
    pthread_t   threadID;     /* Thread ID from pthread_create() */
    bool        to_quit = false;

    parse_args (argc, argv);

    servSock = CreateTCPServerSocket (argv_port);

    while (to_quit == false)                /* run until someone indicates to quit... */
    {
        clntSock = AcceptTCPConnection (servSock);

        // Use the 'void *' parameter for passing clntSock
        result = pthread_create (&threadID, NULL, myThread, (void *) clntSock);
        if (result == 0)
        {
            info ("Succesfully created new thread");
        }
        else
        {
            DieWithError ("pthread_create()");
        }
    }
    
    // server stops...
    close (servSock);
    exit (0);
}

static void *
myThread (void * threadArgs)
{
    int clntSocket;

    clntSocket = (int) threadArgs;

    info ("Thread start");

    if (pthread_detach (pthread_self ()) != 0)
    {
        DieWithError ("pthread_detach()");
    }

    // Do working
    HandleTCPClient(clntSocket);

    info ("Thread exit");
    return (NULL);
}
