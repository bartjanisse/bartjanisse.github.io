#include <stdio.h>
#include <stdlib.h>     // for exit()
#include <unistd.h>     // for getopt()
#include <string.h>     // for strlen()
#include <curl/curl.h>
#include <json/json.h>

#include "hue_aux.h"

char *          hue_ip      = NULL;
char *          hue_user	= NULL;
bool            debug       = false;

void
DieWithError (const char * errorMessage)
{
    perror (errorMessage);
    exit (1);
}

char * 
remove_nl (char * s)
{
    // remove the trailing nerwline in 's' (if present)
    int len;
    
    len = strlen (s);
    if (s [len - 1] == '\n')
    {
        s [len - 1] = '\0';
    }
    
    return (s);
}

void 
parse_args (int argc, char *argv[])
{
    int opt;
    
    while ((opt = getopt(argc, argv, "i:u:dh")) != -1)
    {
        switch (opt)
        {
            case 'i':
                hue_ip = optarg;
                break;
            case 'u':
                hue_user = optarg;
                break;
            case 'h':
                printf("\noptions: \n"
                    "    -i <ip> \n"
                    "    -u <username> \n"
                    "\n");
                exit(0);
                break;
           case 'd':
                debug = true;
                break;
            default:
                fprintf(stderr, "ERROR: unknown option '%c'\n", opt);
                exit(1);
                break;
        }
    }
        
    if (debug == true)
    {
        printf ("\ncompiler version: %s\n\n", __VERSION__);
        printf ("Parameters:\n"
                "    ip:\t\t%s\n"
                "    user:\t%s\n",
                hue_ip, hue_user);
        printf ("\n");
    }
}

/* EOF */