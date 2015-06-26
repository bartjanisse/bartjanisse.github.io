/** \file controller.h
 *  \ingroup xbcdeamon
 * 	\brief Function prototypes and definitions for the controller
 *
 * 	\author A.W Janisse
 * 	\bug No known bugs.
 *
 * 	\version 1.0 	First release.
*/
   	
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <libusb-1.0/libusb.h>
#include <stdbool.h>				// for boolean
#include "../common/buttons.h"
#include "../common/common.h"

//! EP_IN
/*! Constant for the endpoint in. */
#define 	EP_IN		0x81
//! EP_OUT
/*! Constant for the endpoint out. */
#define 	EP_OUT		0x02

/** \ingroup xbcdeamon
 * 
 *  This enumeration contains all the command availlable to controll the LED's. The function #setControllerLeds
 *	can be used to controll the states of the indvidual LED's.
 */	
typedef enum {  
		all_off 	 = 0x00,	 	/** All led's off */ 
		blink_all 	 = 0x01,	 	/** All blinking */
		flash_1_on   = 0x02,	 	/** 1 flashes, then on */
		flash_2_on   = 0x03,	 	/** 2 flashes, then on */
		flash_3_on	 = 0x04,	 	/** 3 flashes, then on */
		flash_4_on	 = 0x05,	 	/** 4 flashes, then on */
		led_1_on 	 = 0x06,	 	/** 1 on */
		led_2_on 	 = 0x07,	 	/** 2 on */
		led_3_on 	 = 0x08,	 	/** 3 on */
		led_4_on 	 = 0x09,	 	/** 4 on */
		rotate		 = 0x0A,	 	/** Rotating (e.g. 1-2-4-3) */
		blink_select = 0x0B,	 	/** previous setting will be used (all blinking, or 1, 2, 3 or 4 on). */
		blink_slow	 = 0x0C,	 	/** Slow blinking */
		alt		 	 = 0x0D	 		/** Alternating (e.g. 1+4-2+3), then back to previous. */
	} Leds;


/** \ingroup xbcdeamon
 *  \brief Controll the Xbox 360 controller rumble actuator with the desired speed.
 *   
 *  \param 	handle is a pointer to the USB device handle.
 *  \param 	speed for the rumble actuator.
 *   
 *  \return 0 if succefull.
 */
int setControllerRumble(libusb_device_handle *handle, uint16_t speed);

/** \ingroup xbcdeamon
 *	\brief Controll the LED's from the Xbox 360 controller.
 *	
 *	\param 	handle is a pointer to the USB device handle.
 * 	\param 	led holds the desired pattern for the LED's. See #Leds for details
 *	
 *	\return 0 if succefull.
 */
int setControllerLeds(libusb_device_handle *handle, Leds led);

/** \ingroup xbcdeamon
 *	\brief Retreives the state of the buttons and joysticks.
 *
 *	\param 	handle is a pointer to the USB device handle.
 *	\param 	b is a pointer to a button structure which will be filled with the values from the controller
 *	
 *	\return	0 if succefull.
 */
int getControllerInput(libusb_device_handle *handle, button *b);

#endif
