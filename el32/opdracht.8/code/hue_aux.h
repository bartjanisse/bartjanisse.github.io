#ifndef _HUELIB_H_
#define _HUELIB_H_

#include <stdbool.h>    /* for bool,true,false */

extern char *       argv_ip;
extern char *       argv_user;
extern bool			argv_debug;

extern void DieWithError(const char * errorMessage);  /* Error handling function */
extern void parse_args (int argc, char * argv[]);
extern char * remove_nl (char * s);

#endif

/* EOF */