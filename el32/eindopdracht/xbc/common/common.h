
#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdlib.h> 	// for exit()
#include <stdio.h> 		// for fprintf()
#include <stdint.h>
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
#define MQ_CMDS             "/mq_cmds"		// slash moet voor naam conform man page (7) mq_overview
#define BUFFER_SIZE			1337			//<! ???????????????????????????????????
#define MAX_DEVS			4

#define MAX_MESSAGE_SIZE 	50
#define MESSAGE_PRIORITY	0
#define MAX_MESSAGE			10

#define CMD_RUMBLE			"rumble"


typedef struct
{
	uint8_t id;
	char cmd[50];
	int16_t val;
}command;

char *makeCommand(char *id, char *cmd, char *val);

#endif
