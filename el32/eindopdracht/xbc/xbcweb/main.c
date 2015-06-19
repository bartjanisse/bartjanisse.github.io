
#include <stdlib.h>
#include <mqueue.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "../common/common.h"


mqd_t 	mq_cmds;

void 
open_mq() 
{
	mq_cmds = mq_open(MQ_CMDS, O_WRONLY);

	if (mq_cmds == (mqd_t)-1) {
		perror("createMQ: mq_open()");
	}
}

void
sendCommand(char *id, char *cmd, char *val)
{
	char *c = makeCommand(id, cmd, val);
	
	if(mq_send(mq_cmds, c, MAX_MESSAGE_SIZE, 0) == 0)
	{
		printf("Message %s is sent\n", cmd);
	}
	else
	{
		printf("Message error...");
	}
}

int
main()
{	
	while(1)
	{
		open_mq();
			
		//sendCommand("0", CMD_RUMBLE, "1000");		
		//sleep(1);
		sendCommand("0", CMD_RUMBLE, "0");
		sleep(1);
		sendCommand("0", CMD_LED, "0");
		sleep(1);
		sendCommand("0", CMD_LED, "6");
		sleep(1);
		sendCommand("0", CMD_LED, "7");
		sleep(1);
		sendCommand("0", CMD_LED, "8");
		sleep(1);
		sendCommand("0", CMD_LED, "9");
		sleep(1);
		
		mq_close(mq_cmds);
	}
	return (0); // not reached
}
