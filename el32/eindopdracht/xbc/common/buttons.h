/*! \file buttons.h
 *  \ingroup coomon
 * 	\brief Implementation for the Devices.
 *
 *
 * 	\author A.W Janisse
 * 	\bug No known bugs.
 *
 * 	\version 1.0 	First release.
*/
#ifndef _BUTTONS_H_
#define _BUTTONS_H_

#include <stdbool.h>				// for boolean
#include "common.h"

/** \ingroup common
 * A structure representing the .....................
 */
typedef struct
{
	bool avail;				//!< Bit to track if controller is availlable
 	bool D_UP;				//!< D-Pad up
	bool D_DN;	  			//!< D-Pad down
	bool D_LEFT;			//!< D-Pad left
	bool D_RIGHT;			//!< D-pad right
	bool START;				//!< Start button
	bool BACK;	 			//!< Back button
	bool LS_PRESS;			//!< Left stick press
	bool RS_PRESS;			//!< Right stick press
	bool LB;	 			//!< Button LB
	bool RB;	 			//!< Button RB
	bool LOGO;	 			//!< Xbox logo button
	bool SPARE;	 			//!< Unused
	bool A;	 	 			//!< Button A
	bool B;	 	 			//!< Button B
	bool X;	 	 			//!< Button X
	bool Y;	 	 			//!< Button Y
	uint8_t Left_trigger;	//!< Left trigger. Produces a value from 0 to 255
	uint8_t Right_trigger;	//!< Right trigger. Produces a value from 0 to 255
	int16_t Left_stick_X;	//!< Left joystick x-value. Produces a value from -32768 to 32767
	int16_t Left_stick_Y;	//!< Left joystick y-value. Produces a value from -32768 to 32767
	int16_t Right_stick_X;	//!< Right joystick x-value. Produces a value from -32768 to 32767
	int16_t Right_stick_Y;	//!< Right joystick y-value. Produces a value from -32768 to 32767
} button;


/** \ingroup common
 * An array representing the .....................
 */
typedef button buttons[MAX_DEVS];

#endif
