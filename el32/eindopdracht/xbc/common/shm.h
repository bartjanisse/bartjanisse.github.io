
#ifndef _SHM_H_
#define _SHM_H_

#include "buttons.h"

#define SHM_NAME			"shm_xbc"		//! Name of the shared memory
#define SEM_NAME			"sem_xbc"		//! Name of the daemon semaphore 

void initButtonSHM(char *name);
void closeButtonSHM(char *name);

void startButtonSHM(char *name);
void stopButtonSHM(char *name);

void witeButtonToSHM(button *b, uint8_t index);
void readButtonFromSHM(button *b, uint8_t index);

#endif
