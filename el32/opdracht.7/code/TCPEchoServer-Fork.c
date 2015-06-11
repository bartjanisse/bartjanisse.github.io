#include <stdio.h>
#include <stdlib.h>     // for atoi() and exit()
#include <unistd.h>     // for fork()
#include <sys/wait.h>   // for waitpid()

#include "Auxiliary.h"
#include "AcceptTCPConnection.h"
#include "CreateTCPServerSocket.h"
#include "HandleTCPClient.h"

int main(int argc, char *argv[])
{
    int     servSock;                  /* Socket descriptor for server */
    int     clntSock;                  /* Socket descriptor for client */
    pid_t   processID;                 /* Process ID from fork() */
    bool    to_quit = false;

    parse_args (argc, argv);

    servSock = CreateTCPServerSocket (argv_port);

    while (to_quit == false)                /* run until someone indicates to quit... */
    {
        clntSock = AcceptTCPConnection (servSock);

        processID = fork();
        if (processID < 0)
        {
            // fatal error, fork failed
            DieWithError ("fork() failed");
        }
        else
        {
            if (processID == 0)
            {
                // processID == 0: child process, handle client communication
                info_d ("New child process created. ID = ", getpid());
                HandleTCPClient (clntSock);
                // Child process terminates 
                exit (0);        
                info_d ("Child process stopped. ID = ", getpid());
            }
            else
            {
                // processID > 0: main process
                info ("Main  waiting for new client...");
            }
        }
    }
    
    // server stops...
    exit (0);
}
