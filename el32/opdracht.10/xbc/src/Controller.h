/*! \file Controller.h
  	\brief Function prototypes for the controller.
 
   	\details This contains the prototypes for the controller and eventually any macros, constants or 
   	global variables you will need.
 
   	\author A.W Janisse
   	\bug No known bugs.

   	\version 1.0 	First release.
*/
   	
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <libusb-1.0/libusb.h>
#include <stdbool.h>				// for boolean

//! VENDOR_ID
/*! Constant for vendor id. This constant can be used to derive the handle.*/
#define 	VENDOR_ID	0x045e
//! PRODUCT_ID
/*! Constant for product id. This constant can be used to derive the handle.*/
#define 	PRODUCT_ID	0x028e
//! EP_IN
/*! Constant for the endpoint in. */
#define 	EP_IN		0x81
//! EP_OUT
/*! Constant for the endpoint out. */
#define 	EP_OUT		0x02

//! Enum Leds
/*! This enumeration contains all the command availlable to controll the LED's. The function #setControllerLeds
	can be used to controll the states of the indvidual LED's.
*/
typedef enum {  
		all_off 	 = 0x00,	 	//!< All led's off
		blink_all 	 = 0x01,	 	//!< All blinking
		flash_1_on   = 0x02,	 	//!< 1 flashes, then on
		flash_2_on   = 0x03,	 	//!< 2 flashes, then on
		flash_3_on	 = 0x04,	 	//!< 3 flashes, then on
		flash_4_on	 = 0x05,	 	//!< 4 flashes, then on
		led_1_on 	 = 0x06,	 	//!< 1 on
		led_2_on 	 = 0x07,	 	//!< 2 on
		led_3_on 	 = 0x08,	 	//!< 3 on
		led_4_on 	 = 0x09,	 	//!< 4 on
		rotate		 = 0x0A,	 	//!< Rotating (e.g. 1-2-4-3)
		blink_select = 0x0B,	 	//!< previous setting will be used (all blinking, or 1, 2, 3 or 4 on). 
		blink_slow	 = 0x0C,	 	//!< Slow blinking
		alt		 	 = 0x0D	 		//!< Alternating (e.g. 1+4-2+3), then back to previous.
	} Leds;

//! struct Buttons
/*! This structure contains the states of all the buttons and analog joysticks of the Xbox controller. This
	structure is filled with the function #getControllerInput. 
*/
typedef struct
{
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
} Buttons;

/*! \fn int setControllerRumble(libusb_device_handle *handle, uint16_t speed)
    \brief Controll the Xbox 360 controller rumble actuator with the desired speed.
    
    \param 	handle is a pointer to the USB device handle.
    \param 	speed for the rumble actuator.
    
    \return 0 if succefull.

 	\pre 	A valid pointer to handle.
*/
int setControllerRumble(libusb_device_handle *handle, uint16_t speed);

/*! \fn int setControllerLeds(libusb_device_handle *handle, Leds led);
	\brief Controll the LED's from the Xbox 360 controller.
	
	\param 	handle is a pointer to the USB device handle.
 	\param 	led holds the desired pattern for the LED's. See #Leds for details
 	
 	\return 0 if succefull.

 	\pre 	A valid pointer (not NULL) to handle.
*/
int setControllerLeds(libusb_device_handle *handle, Leds led);

/*! \fn int getControllerInput(libusb_device_handle *handle, Buttons *buttons)
	\brief Retreives the state of the buttons and joysticks.

	\param 	handle is a pointer to the USB device handle.
	\param 	buttons is a pointer to a Buttons struct which will be filled with the button states and joystick values.
	
	\return	0 if succefull.
	
	\pre 	A valid pointer to handle and button.
	\post	The structure buttons is filled with the current state.
*/
int getControllerInput(libusb_device_handle *handle, Buttons *buttons);

#endif