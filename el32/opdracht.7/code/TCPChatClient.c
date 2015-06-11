#include <stdio.h>
#include <string.h>     // for strlen(), strcmp()
#include <stdlib.h>     // for exit()
#include <sys/socket.h> // for send() and recv()
#include <unistd.h>     // for sleep(), close()

#include "Auxiliary.h"
#include "CreateTCPClientSocket.h"

#define RCVBUFSIZE 32   /* Size of receive buffer */
 
int main (int argc, char *argv[])
{
    int         sock;                   /* Socket descriptor */
    char        echoBuffer[RCVBUFSIZE]; /* Buffer for received string */
    int         bytesRcvd;              /* Bytes read in single recv() */
    int         bytesSend;              /* Bytes read in single send() */
    char        line[80];               /* String entered by user to send to server */

    parse_args (argc, argv);

    sock = CreateTCPClientSocket (argv_ip, argv_port);
        
    while (strcmp (line, "Quit") != 0)
    {
        /* Send message to server */
        fgets (line, sizeof (line), stdin);
        remove_nl (line);
        info_s ("Entered:  ", line);

        bytesSend = send (sock, line, strlen (line), 0);
        if (bytesSend < 0)
        {
            DieWithError ("send() failed");
        }

        /* Receive message from server */
        if(strcmp (line, "Quit") != 0)
        {
            bytesRcvd = recv (sock, echoBuffer, RCVBUFSIZE-1, 0);
            if (bytesRcvd < 0)
            {
                DieWithError ("recv() failed");
            }
            if (bytesRcvd > 0)
            {
                add_nt (echoBuffer, bytesRcvd);
                printf ("Received: %s\n", echoBuffer);
            }
        }
    }

    delaying ();

    close (sock);
    info ("close & exit");
    exit (0);
}
