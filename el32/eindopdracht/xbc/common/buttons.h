/*! \file buttons.h
 *  \ingroup common
 * 	\brief Definition for controller buttons
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
 *  \brief A structure representing the the button and joystick states 
 */
typedef struct
{
	bool avail;				//!< Bit to track if controller is availlable
 	bool D_UP;				//!< D-Pad up pressed/not pressed
	bool D_DN;	  			//!< D-Pad down pressed/not pressed
	bool D_LEFT;			//!< D-Pad left pressed/not pressed
	bool D_RIGHT;			//!< D-pad right pressed/not pressed
	bool START;				//!< Start button pressed/not pressed
	bool BACK;	 			//!< Back button pressed/not pressed
	bool LS_PRESS;			//!< Left stick press pressed/not pressed
	bool RS_PRESS;			//!< Right stick press pressed/not pressed
	bool LB;	 			//!< Button LB pressed/not pressed
	bool RB;	 			//!< Button RB pressed/not pressed
	bool LOGO;	 			//!< Xbox logo button pressed/not pressed
	bool SPARE;	 			//!< Unused
	bool A;	 	 			//!< Button A pressed/not pressed
	bool B;	 	 			//!< Button B pressed/not pressed
	bool X;	 	 			//!< Button X pressed/not pressed
	bool Y;	 	 			//!< Button Y pressed/not pressed
	uint8_t Left_trigger;	//!< Left trigger. Produces a value from 0 to 255
	uint8_t Right_trigger;	//!< Right trigger. Produces a value from 0 to 255
	int16_t Left_stick_X;	//!< Left joystick x-value. Produces a value from -32768 to 32767
	int16_t Left_stick_Y;	//!< Left joystick y-value. Produces a value from -32768 to 32767
	int16_t Right_stick_X;	//!< Right joystick x-value. Produces a value from -32768 to 32767
	int16_t Right_stick_Y;	//!< Right joystick y-value. Produces a value from -32768 to 32767
} button;


/** \ingroup common
 *  \brief An array representing the collection of controller buttons
 */
typedef button buttons[MAX_DEVS];

#endif
