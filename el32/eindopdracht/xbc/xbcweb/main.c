
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "../common/commands.h"
#include "../common/common.h"
#include "../common/buttons.h"
#include "../common/shm.h"

#define WEB_LOG_NAME	"xbc-web"	//! name used in th elog file

void
printButton(button *b)
{
	printf("%d %d \n",b->D_UP, b->A);	
}

void
readButtons()
{
	int i;
	button btn;
	
	for(i=0; i<MAX_DEVS; i++)
	{
		readButtonFromSHM(&btn, i);
		printButton(&btn);
	}
}

int
main()
{
	const char *strCmd;

	// Create a log entry in /var/log/syslog
	openlog (WEB_LOG_NAME, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0);
	syslog(LOG_INFO, "Program started by User %d", getuid());
	
	/* Start HTTP output */
	printf("Content-Type: text/plain\n\n");

	if ((strCmd = getenv("QUERY_STRING")) != NULL && *strCmd != '\0')
    {
		if (strstr(strCmd, CMD_BTNS))
		{
			startButtonSHM(SHM_NAME);
			//create string here
			
		}
		else if (strstr(strCmd, CMD_LED))
		{
			
		}
		else if (strstr(strCmd, CMD_RUMBLE))
		{
			
		}
	}

	return (0);

/*

	int i = 0;
	
	// Create a log entry in /var/log/syslog
	openlog (WEB_LOG_NAME, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0);
	syslog(LOG_INFO, "Program started by User %d", getuid());
	
	startButtonSHM(SHM_NAME);
	
	while(i<10)
	{
		readButtons();

		startQueue(MQ_CMDS_NAME);
			
		//sendCommand("0", CMD_RUMBLE, "1000");		
		//sleep(1);
		//sendCommand("0", CMD_RUMBLE, "0");
		//usleep(100000);
		//sendCommand("0", CMD_LED, "0");
		//usleep(100000);
		sendCommand("0", CMD_LED, "6");
		usleep(100000);
		sendCommand("0", CMD_LED, "7");
		usleep(100000);
		sendCommand("0", CMD_LED, "9");
		usleep(100000);
		sendCommand("0", CMD_LED, "8");
		usleep(100000);
		
		stopQueue();
	
		i++;
	}
	
//	stopButtonSHM(SHM_NAME);
	
	return (0); // not reached
*/
}
