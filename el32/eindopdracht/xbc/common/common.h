
#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdlib.h> 	// for exit()
#include <stdio.h> 		// for fprintf()
#include <syslog.h>		// for syslog()

#define CHECK(x) \
	do { \
		if(x) { \
			fprintf(stderr, "CHECK() in function: %s, line: %d, evaluation of:  ", __func__, __LINE__); \
			perror(#x); \
			exit(-1); \
			syslog (LOG_ERR, "Ouch an ERROR!"); \
		} \
	} while (0) \
	
#define SHM_NAME			"shm_xbc"		//! Name of the shared memory
#define SEM_DAEMON			"sem_daemon"	//! Name of the daemon semaphore 
#define SEM_WEB				"sem_web"		//! Name of the web semaphore
#define BUFFER_SIZE			1337			//<! ???????????????????????????????????
#define MAX_DEVS			4



#endif
