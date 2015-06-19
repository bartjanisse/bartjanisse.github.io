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
#include "controller.h"
#include "../common/shm.h"
#include "../common/common.h"


int 	shm_fd = -1;
sem_t 	*semDaemon;
sem_t 	*semWeb;
mqd_t 	mq_cmds;

static uint8_t running = 1;

pthread_t cmd_thread;
pthread_t btn_thread;

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
		LOGERR("Error creating messagequeue. Not listening to commands.");
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
	
	/* Start reading the message queue */
	while(running)
	{
		length = mq_receive(mq_cmds, message, MAX_MESSAGE_SIZE, MESSAGE_PRIORITY);
		
		if(length > 0)
		{
			retreiveCommand(&cmd, message);
			
			if (strstr(cmd.cmd, CMD_RUMBLE))
			{
				setControllerRumble(getDeviceHandle(cmd.id), cmd.val);
			}
			else if (strstr(cmd.cmd, CMD_LED))
			{
				setControllerLeds(getDeviceHandle(cmd.id), cmd.val);
			}
		}		
	}
	syslog(LOG_INFO, "Stop listening for commands\n");
	return (NULL);
}

void *
buttonListenerThread() 
{
	buttons btns;	
	uint8_t i;
	button *buttonSHM;
			
	syslog(LOG_INFO, "Listening for buttons\n");
		
	buttonSHM = createButtonSHM(SHM_NAME);	
		
	/* Start reading the buttons of each controller */
	while(running)
	{
		for(i=0; i<MAX_DEVS; i++)
		{	
			getControllerInput(getDeviceHandle(i), &btns[i]);
			
			/* Start critical section */
			
			buttonSHM[i] = btns[i];
			
			/* End critical section */
		}
	}
	syslog(LOG_INFO, "Stop listening for buttons\n");	
	return (NULL);
}

void 
closeAndExit(int sig) 
{	
	// Stop running
	running = 0;
	// Wait for other trheads to stop.
	pthread_join(cmd_thread, NULL);
	pthread_join(btn_thread, NULL);
	
	closeButtonSHM(SHM_NAME);
	
	/*
	sem_close(semDaemon);
	sem_unlink(SEM_DAEMON);
	
	sem_close(semWeb);
	sem_unlink(SEM_WEB);
	*/
	
	closeDevices();

	mq_unlink(MQ_CMDS);
	
	syslog (LOG_INFO, "Program stopped by User %d", getuid());
	closelog();
	exit(0);
}


int
main()
{
	//int 	fd = -1;
	int16_t err = 0;
	uint8_t i;
	libusb_device_handle *handle;
	
	signal(SIGINT, closeAndExit);
	
	//fd = createSHM(SHM_NAME, sizeof(buttons));
	//mapSHM(fd, sizeof(buttons));
	
	
	
	// Create a log entry in /var/log/syslog
	openlog ("xbcdaemon", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0);
	syslog(LOG_INFO, "Program started by User %d", getuid());
	if(err != 0)
	{
		syslog(LOG_INFO, "Returned %d from daemon() function", err);
	}
	
	initDevices(0,0);

	open_mq();

	if (pthread_create(&cmd_thread, NULL, commandListenerThread, NULL) != 0) 
	{
		LOGERR("Error creating commandlistener");
	}

	if (pthread_create(&btn_thread, NULL, buttonListenerThread, NULL) != 0) 
	{
		LOGERR("Error creating buttonlistener");
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


