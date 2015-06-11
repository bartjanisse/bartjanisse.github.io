#include <stdio.h>
#include <string.h>     // for strcmp()
#include <ctype.h>      // for isupper() etc.
#include <sys/socket.h> // for send() and recv()
#include <unistd.h>     // for sleep(), close()

#include "Auxiliary.h"
#include "HandleTCPChatClient.h"

#define RCVBUFSIZE 32   /* Size of receive buffer */

void HandleTCPChatClient (int clntSocket)
{
    // 'clntSocket' is obtained from AcceptTCPConnection()

    char    echoBuffer[RCVBUFSIZE];     /* Buffer for echo string */
    int     bytesRcvd   = 0;            /* Size of received message */
    int     bytesSend   = 0;            /* Bytes read in single send() */
    char    line[80];                   /* String entered by user to send to server */
       
    while (strcmp (line, "Quit") != 0)
    {

        while (bytesRcvd == 0)
        {
            /* Receive message from client */
            bytesRcvd = recv (clntSocket, echoBuffer, RCVBUFSIZE-1, 0);
            if (bytesRcvd < 0)
            {
                DieWithError ("recv() failed");
            }
            if (bytesRcvd > 0)
            {
                info_d ("Recv", bytesRcvd);
                add_nt (echoBuffer, bytesRcvd);
                printf ("Received: %s\n", echoBuffer);
            }
        }

        /* Send message to client */
        fgets (line, sizeof (line), stdin);
        remove_nl (line);
        info_s ("Entered:  ", line);

        bytesSend = send (clntSocket, line, strlen (line), 0);
        if (bytesSend < 0)
        {
            DieWithError ("send() failed");
        }

        bytesRcvd = 0;
    }

    close (clntSocket);    /* Close client socket */
    info ("close");
}
