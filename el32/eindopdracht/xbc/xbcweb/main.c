
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "../common/commands.h"
#include "../common/common.h"
#include "../common/buttons.h"
#include "../common/shm.h"

#define WEB_LOG_NAME	"xbc-web"	//! name used in the log file

void
printButton(button *btn)
{	
	printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
		btn->avail,
		btn->D_UP,
		btn->D_DN,
		btn->D_LEFT,
		btn->D_RIGHT,
		btn->START,
		btn->BACK,
		btn->LS_PRESS,
		btn->RS_PRESS,
		btn->LB,
		btn->RB,
		btn->LOGO,
		btn->SPARE,
		btn->A,
		btn->B,
		btn->X,
		btn->Y,
		btn->Left_trigger,
		btn->Right_trigger,
		btn->Left_stick_X,
		btn->Left_stick_Y,
		btn->Right_stick_X,
		btn->Right_stick_Y);
}

void
removeSpaces(char *str)
{
	uint16_t i;
	char *tmpStr = (char *)malloc(strlen(str));
	char *dest = tmpStr;
	
	if(tmpStr){
		i = 0;
		while(str[i])
		{
			if(str[i] != '%') {
				*dest++ = str[i];
			} else {
				i += 2; // Skip the next 2 positions to remove "20"
			}
			i++;
		}
		*dest = '\0';
	}	
	strcpy(str, tmpStr);
	free(tmpStr);
}


int
main()
{
	char 		*strCmd;
	command 	cmd;
	button 		btn;
	
	// Create a log entry in /var/log/
	// cat messages | grep xbc
	openlog (WEB_LOG_NAME, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0);
	
	/* Start HTTP output */
	printf("Content-Type: text/plain\n\n");

	if ((strCmd = getenv("QUERY_STRING")) != NULL && *strCmd != '\0')
    {
		removeSpaces(strCmd);
		retreiveCommand(&cmd, strCmd);
			
		if (strstr(strCmd, CMD_BTNS))
		{
			if(cmd.id >= 0 && cmd.id < MAX_DEVS)
			{
				startButtonSHM(SHM_NAME);
				readButtonFromSHM(&btn, cmd.id);
				printButton(&btn);
			}
		}
		else if (strstr(strCmd, CMD_LED))
		{
			startQueue();
			sendCommand(&cmd);
			stopQueue();
		}
		else if (strstr(strCmd, CMD_RUMBLE))
		{
			startQueue();
			sendCommand(&cmd);
			stopQueue();
		}
	}
	return (0);
}
