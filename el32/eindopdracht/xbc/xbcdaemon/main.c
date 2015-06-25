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

#include "devices.h"
#include "controller.h"
#include "../common/commands.h"
#include "../common/shm.h"
#include "../common/common.h"

#define DEAMON_LOG_NAME	"xbc-deamon" //! name used in the log file

static uint8_t 		running = 1;
static pthread_t 	cmd_thread;
static pthread_t 	btn_thread;

void *
commandListenerThread() 
{
	command 	cmd;
		
	syslog(LOG_INFO, "Start listening for commands\n");
	
	/* Start reading the message queue */
	while(running)
	{	
		getCommand(&cmd);
			
		if (strstr(cmd.cmd, CMD_RUMBLE))
		{
			setControllerRumble(getDeviceHandle(cmd.id), cmd.val);
		}
		else if (strstr(cmd.cmd, CMD_LED))
		{
			setControllerLeds(getDeviceHandle(cmd.id), cmd.val);
		}
	}

	syslog(LOG_INFO, "Stopped listening for commands\n");
	return (NULL);
}

void *
buttonReaderThread() 
{
	buttons btns;
	uint8_t i;
			
	syslog(LOG_INFO, "Start reading controller buttons\n");
				
	/* Start reading the buttons of each controller */
	while(running)
	{
		for(i=0; i<MAX_DEVS; i++)
		{	
			getControllerInput(getDeviceHandle(i), &btns[i]);
			witeButtonToSHM(&btns[i], i);
		}
	}
	syslog(LOG_INFO, "Stopped reading controller buttons\n");	
	return (NULL);
}

void 
closeAndExit(int sig) 
{	
	// Stop running threads
	running = 0;
	// Wait for the trheads to stop.
	pthread_join(cmd_thread, NULL);
	pthread_join(btn_thread, NULL);
	
	closeButtonSHM(SHM_NAME);	
	closeDevices();
	closeQueue();
	
	syslog (LOG_INFO, "Program stopped by User %d", getuid());
	closelog();
	exit(0);
}

int
main()
{	
	signal(SIGINT, closeAndExit);
		
	// Create a log entry in /var/log/
	openlog (DEAMON_LOG_NAME, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0);
	
	if (daemon(0,0) != 0)
	{
		LOGERR("daemon() failed");
	}
	
	initDevices(VENDOR_ID, PRODUCT_ID);
	initButtonSHM(SHM_NAME);	
	initQueue(MQ_CMDS_NAME);

	if (pthread_create(&cmd_thread, NULL, commandListenerThread, NULL) != 0) 
	{
		LOGERR("Error creating commandlistenerThread");
	}

	if (pthread_create(&btn_thread, NULL, buttonReaderThread, NULL) != 0) 
	{
		LOGERR("Error creating buttonReaderThread");
	}

	while(1) { }
	
	return (0); // not reached
}


