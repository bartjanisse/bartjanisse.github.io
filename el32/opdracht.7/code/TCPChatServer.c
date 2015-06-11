#include "Auxiliary.h"
#include "AcceptTCPConnection.h"
#include "CreateTCPServerSocket.h"
#include "HandleTCPChatClient.h"

int main (int argc, char * argv[])
{
    int servSock;                    /* Socket descriptor for server */
    int clntSock;                    /* Socket descriptor for client */

    parse_args (argc, argv);
    
    servSock = CreateTCPServerSocket (argv_port);

    for (;;) /* Run forever */
    {
        clntSock = AcceptTCPConnection (servSock);
        HandleTCPChatClient (clntSock);
    }
    /* NOT REACHED */
}
