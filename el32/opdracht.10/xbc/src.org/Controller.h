/** @file Controller.h
 *  @brief Function prototypes for the console driver.
 *
 *  @details This contains the prototypes for xxxxxxxxxxxxxxxxxxxxxxx
 *
 *  @author A.W Janisse
 *  @bug No known bugs.
 */
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <libusb-1.0/libusb.h>
#include <stdbool.h>	// for boolean

/// @todo op een juiste manier documenteren
/*!
  \def VENDOR_ID	
  Microsoft corporation vendor id.
*/
#define 	VENDOR_ID	0x045e		//<! 
#define 	VENDOR_PROD	0x028e		//<! Xbox 360 controller product id
#define 	EP_IN		0x81		//<! Constant for endpoint input
#define 	EP_OUT		0x02		/*!< Computes the minimum of \a x and \a y. */ // Constant for endpoint output 

/** @brief Availlable LED patterns.
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
		alt		 	 = 0x0D	 		//!< Alternating (e.g. 1+4-2+3), then back to previous*
	} Leds;

/** @brief Structure representing the buttons and joysticks.
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

/** @brief Controll the rumble actuator from the Xbox 360 controller.
 *
 *  Controll the rumble actuator with the desired speed. The speed can varry from 0 (no rumble)
 *	to 0xFFFF (max rumble).
 *
 *  @param handle is a pointer to the USB device handle.
 *	@param speed is the value for the rumble actuator.
 *  @return 0 if succefull or error code if fails.
 */
int setControllerRumble(libusb_device_handle *handle, uint16_t speed);

/** @brief Controll the LED's from the Xbox 360 controller.
 *
 *  @param handle is a pointer to the USB device handle.
 *	@param led holds the desired pattern for the LED's.
 *  @return 0 if succefull or error code if fails.
 */
int setControllerLeds(libusb_device_handle *handle, Leds led);

/** @brief Retreives the state of the buttons and joysticks.
 *
 *  @param handle is a pointer to the USB device handle.
 *	@param buttons is a pointer to a Buttons struct which will be filled with the button states and joystick values.
 *  @return 0 if succefull or error code if fails.
 */
int getControllerInput(libusb_device_handle *handle, Buttons *buttons);

#endif