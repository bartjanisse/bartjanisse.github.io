
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
		} \
	} while (0) \


#define LOGERR(msg) syslog(LOG_ERR, "ERROR: %s. Function %s line %d in file %s", msg, __func__, __LINE__, __FILE__); 
#define LOGERR_D(msg, d) syslog(LOG_ERR, "ERROR: %s %d. Function %s line %d in file %s", msg, d, __func__, __LINE__, __FILE__); 
	
#define MAX_DEVS		4		//! Maximum number of controllers
	
#endif
