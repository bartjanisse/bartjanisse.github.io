/*! \file shm.h
 *  \ingroup common
 * 	\brief Shared memory definitions
 *
 * 	\author A.W Janisse
 * 	\bug No known bugs.
 *
 * 	\version 1.0 	First release.
*/
#ifndef _SHM_H_
#define _SHM_H_

#include "buttons.h"

#define SHM_NAME			"shm_xbc"		//!< Name of the shared memory
#define SEM_NAME			"sem_xbc"		//!< Name of the daemon semaphore 

/** \ingroup common
 *  \brief function to create and open a shared memory
 * 
 * \param name for the shared memory
 */ 
void initButtonSHM(char *name);
/** \ingroup common
 *  \brief function to close and unlink a shared memory
 * 
 * \param name for the shared memory
 */ 
void closeButtonSHM(char *name);

/** \ingroup common
 *  \brief function to open an existing shared memory
 * 
 * \param name for the shared memory
 */ 
void startButtonSHM(char *name);

/** \ingroup common
 *  \brief function to close an existing shared memory
 * 
 * \param name for the shared memory
 */ 
void stopButtonSHM(char *name);

/** \ingroup common
 *  \brief threadsafe function to write a button struct to the shared memory
 * 
 * \param b is the button struct to write
 * \param id is the controller id
 */ 
void witeButtonToSHM(button *b, uint8_t id);

/** \ingroup common
 *  \brief threadsafe function to read a button struct from the shared memory
 * 
 * \param b is the button struct to store the read data
 * \param id is the controller id
 */ 
void readButtonFromSHM(button *b, uint8_t id);

#endif
