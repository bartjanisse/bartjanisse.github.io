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


#include "Devices.h"
#include "../common/common.h"

int 	shm_fd = -1;
sem_t 	*semDaemon;
sem_t 	*semWeb;

void 
closeAndExit(int sig) 
{	
	shm_unlink(SHM_NAME);
	
	sem_close(semDaemon);
	sem_unlink(SEM_DAEMON);
	
	sem_close(semWeb);
	sem_unlink(SEM_WEB);
	
	closeDevices();
	
	syslog (LOG_INFO, "Program stopped by User %d", getuid());
	closelog();
	exit(0);
}

int
main()
{
	int16_t err = 0;
	
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

	while(1)
	{
		// getDevicehandles
		// getControllerStates
		// writeSHM
		// handle messages
	}
	
	//closeAndExit(0);
	return (0); // not reached
}


