
#include <stdlib.h>
#include <mqueue.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "../common/common.h"


mqd_t 	mq_cmds;

void open_mq() 
{
	mq_cmds = mq_open(MQ_CMDS, O_WRONLY);

	if (mq_cmds == (mqd_t)-1) {
		perror("createMQ: mq_open()");
	}
	else
	{
		printf("Messagequeue %s is created\n", MQ_CMDS);
	}
}

int
main()
{
	
	
	char *cmd;
	char a[10]; 
	uint16_t i = 0;
	
	
	while(1)
	{
		open_mq();
		
		snprintf (a, sizeof(a), "%d",i);

		cmd = makeCommand("0", CMD_RUMBLE, a);
		if(mq_send(mq_cmds, cmd, MAX_MESSAGE_SIZE, 0) == 0)
		{
			printf("Message %s is sent\n", cmd);
		}
		else
		{
			printf("Message error...");
		}
		mq_close(mq_cmds);
		sleep(2);
		i++;
	}
	
	return (0);
}
