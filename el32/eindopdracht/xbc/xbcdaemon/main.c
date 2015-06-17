/*! \file main.c
    \brief Starting point of the application.
 
    \details This file contains #main routine which is the starting point of the apllication. From within this main
    ????????????????????????????????????????????????????????.
 
    \author A.W Janisse
    \bug No known bugs.

    \version 1.0    First release.
*/
#include <semaphore.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <signal.h>
#include <syslog.h>
#include <unistd.h>
#include <pthread.h>
#include <mqueue.h>
#include <string.h>


#include "Devices.h"
#include "../common/common.h"

int 	shm_fd = -1;
sem_t 	*semDaemon;
sem_t 	*semWeb;
mqd_t 	mq_cmds;

void open_mq() 
{
	struct mq_attr attr;

	attr.mq_flags   	= 0;
	attr.mq_maxmsg  	= MAX_MESSAGE;
	attr.mq_msgsize 	= MAX_MESSAGE_SIZE;
	attr.mq_curmsgs 	= 0;

	mq_unlink(MQ_CMDS);
	mq_cmds = mq_open(MQ_CMDS, O_RDONLY | O_CREAT, 0644, &attr);

	if (mq_cmds == (mqd_t)-1) 
	{
	  syslog(LOG_ERR, "Error creating messagequeue. Not listening to commands.");
	}
	
	printf("Messagequeue %s is created\n", MQ_CMDS);
}

void
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

void *
commandListenerThread() 
{
	char 		message[MAX_MESSAGE_SIZE];
	command 	cmd;
	ssize_t 	length;
		
	syslog(LOG_INFO, "Listening for commands\n");
	
	/* Guarantees that thread resources are deallocated upon return */
	if (pthread_detach(pthread_self()) != 0) 
	{
	  syslog(LOG_ERR, "Device polling thread error in detach");
	}
	
	/* Start reading the message que */
	while(1)
	{
		length = mq_receive(mq_cmds, message, MAX_MESSAGE_SIZE, MESSAGE_PRIORITY);
		
		if(length > 0)
		{
			retreiveCommand(&cmd, message);
			
			if (strstr(cmd.cmd, CMD_RUMBLE))
			{
				printf("id=%d, cmd=%s, value=%d\n", cmd.id, cmd.cmd, cmd.val);
			}
		}		
	}
	
	return (NULL);
}

void 
closeAndExit(int sig) 
{	
	/*
	shm_unlink(SHM_NAME);
	
	sem_close(semDaemon);
	sem_unlink(SEM_DAEMON);
	
	sem_close(semWeb);
	sem_unlink(SEM_WEB);
	*/
	
	mq_unlink(MQ_CMDS);
	closeDevices();
	
	syslog (LOG_INFO, "Program stopped by User %d", getuid());
	closelog();
	exit(0);
}

int
main()
{
	int16_t err = 0;
	libusb_device_handle *handle;
	uint8_t i;
	//err = daemon(0,0);
	
	signal(SIGINT, closeAndExit);
	
	// Create a log entry in /var/log/syslog
	openlog ("xbcdaemon", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0);
	syslog(LOG_INFO, "Program started by User %d", getuid());
	if(err != 0)
	{
		syslog(LOG_INFO, "Returned %d from daemon() function", err);
	}
	
	initDevices(0,0);

	open_mq();

	pthread_t cmd_thread;
	if (pthread_create(&cmd_thread, NULL, commandListenerThread, NULL) != 0) 
	{
		syslog(LOG_ERR, "Error creating commandlistener");
	}

	
	//sleep(3);

	while(1)
	{
		// getDevicehandles
		// getControllerStates
		// writeSHM
		// handle messages
		for(i=0;i<MAX_DEVS;i++)
		{
			handle = getDeviceHandle(i);
			if(handle != NULL)
			{
				//printf("Valid handle found for device %d\n", i);
			}
		}
		sleep(3);
	}
	
	return (0); // not reached
}


