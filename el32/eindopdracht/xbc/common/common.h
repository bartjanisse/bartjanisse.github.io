/*! \file common.h
 *  \ingroup common
 * 	\brief Common macro definitions and the constant for the max. number of controllers
 *
 * 	\author A.W Janisse
 * 	\bug No known bugs.
 *
 * 	\version 1.0 	First release.
*/
#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdlib.h> 	// for exit()
#include <stdio.h> 		// for fprintf()
#include <stdint.h>
#include <syslog.h>		// for syslog()

/** \ingroup common
 *  \brief Constant for the maximum number of controllers
 */
#define MAX_DEVS		4

/** \ingroup common
 *  \brief Macro to put a error message in the logfile
 */
#define LOGERR(msg) syslog(LOG_ERR, "ERROR: %s. Function %s line %d in file %s", msg, __func__, __LINE__, __FILE__); 

#endif
