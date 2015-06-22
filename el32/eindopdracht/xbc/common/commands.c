
#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include "common.h"
#include "commands.h"

static mqd_t 	mq_cmds;

static void
retreiveCommand(command *cmd, char *msg)
{
	char *e, substr[50] = {0};
	int index1, index2;

	// Copy id
	e = strchr(msg, ',');
	index1 = (int)(e - msg);
	
	strncpy (substr, msg, index1);
	substr[index1+1] = '\0';
	
	cmd->id = atoi(substr);
	
	// copy value
	e = strrchr(msg, ',');
	index2 = (int)(e - msg);
	
	strncpy(substr, msg + index2 + 1, strlen(msg) - index2 - 1);
	substr[strlen(msg) - index2] = '\0';
	
	cmd->val = atoi(substr);
	
	// Copy command
	strncpy(cmd->cmd, msg + index1 + 1, index2 - index1- 1);
	cmd->cmd[index2 - index1 - 1] = '\0';
}

static char *
makeCommand(char *id, char *cmd, char *val)
{
	char *result = malloc(strlen(id)+strlen(cmd)+strlen(val)+3);
	
	strcpy(result, id);
	strcat(result, ",");
    strcat(result, cmd);
    strcat(result, ",");
    strcat(result, val);
    return result;
}

void 
initQueue(char *name)
{
	struct mq_attr attr;

	attr.mq_flags   	= 0;
	attr.mq_maxmsg  	= MAX_MESSAGE;
	attr.mq_msgsize 	= MAX_CMD_SIZE;
	attr.mq_curmsgs 	= 0;

	mq_unlink(name);
	mq_cmds = mq_open(name, O_RDONLY | O_CREAT, 0644, &attr);

	if (mq_cmds == (mqd_t)-1) 
	{
		LOGERR("Error creating messagequeue. Not listening to commands.");
		return;
	}
	syslog(LOG_INFO, "Succesfully created messagequeue\n");
}

void closeQueue()
{
	if(mq_close(mq_cmds) != 0)
	{
		LOGERR("mq_close() failed");
	}
	if(mq_unlink(MQ_CMDS_NAME) != 0)
	{
		LOGERR("mq_unlink() failed");
	}
	
	syslog(LOG_INFO, "Done closing messagequeue\n");
}

void 
startQueue(char *name) 
{
	mq_cmds = mq_open(name, O_WRONLY);

	if (mq_cmds == (mqd_t)-1) 
	{
		LOGERR("Error opening messagequeue. Not listening to commands.");
		return;
	}
	syslog(LOG_INFO, "Succesfully opened messagequeue\n");
}	

void
stopQueue()
{
	if(mq_close(mq_cmds) != 0)
	{
		LOGERR("mq_close() failed. Maybe the daemon is not running!");
	}
}

void 
sendCommand(char *id, char *cmd, char *val)
{
	char *c = makeCommand(id, cmd, val);
		
	if(mq_send(mq_cmds, c, MAX_CMD_SIZE, 0) != 0)
	{
		LOGERR("mq_send() failed");
	}
	
	free(c);
}

void
getCommand(command *cmd)
{
	char message[MAX_CMD_SIZE];
	
	if(mq_receive(mq_cmds, message, MAX_CMD_SIZE, MESSAGE_PRIORITY) > 0)
	{
		retreiveCommand(cmd, message);
	}
}

