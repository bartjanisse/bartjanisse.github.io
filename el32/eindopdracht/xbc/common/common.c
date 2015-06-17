
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "common.h"


char *
makeCommand(char *id, char *cmd, char *val)
{
	char *result = malloc(strlen(id)+strlen(cmd)+strlen(val)+3);//+1 for the zero-terminator
	
	strcpy(result, id);
	strcat(result, ",");
    strcat(result, cmd);
    strcat(result, ",");
    strcat(result, val);
    return result;
}
