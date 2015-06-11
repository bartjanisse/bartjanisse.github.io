/*! \file Controller.c
  	\brief Implementation for the controller.
 
   	\details This is the implementation of the controller.
 
   	\author A.W Janisse
   	\bug No known bugs.

   	\version 1.0 	First release.
*/

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <assert.h>
#include "Controller.h"

#define TIMEOUT 	1000	//!< Time-out of 1000ms for USB transfer

int
setControllerRumble(libusb_device_handle *handle, uint16_t speed)
{
	uint8_t 	rumble[] = { 0x00, 0x08, 0x00, speed >> 8, speed, 0x00, 0x00, 0x00 };
 	int 		result, transferred;

 	assert(handle != NULL);

 	result = libusb_interrupt_transfer(handle, EP_OUT, rumble, sizeof rumble, &transferred, TIMEOUT);
	
	return (result);
}

int 
setControllerLeds(libusb_device_handle *handle, Leds led)
{
	uint8_t 	data[] = { 0x01, 0x03, led };
 	int 		result, transferred;

 	assert(handle != NULL);

 	result = libusb_interrupt_transfer(handle, EP_OUT, data, sizeof data, &transferred, TIMEOUT);

	return (result);
}

int 
getControllerInput(libusb_device_handle *handle, Buttons *buttons)
{
	uint8_t 	input[20];
	int 		result, transferred;

	assert(handle != NULL);
	assert(buttons != NULL);

	result = libusb_interrupt_transfer(handle, EP_IN, input, sizeof input, &transferred, TIMEOUT);  

	buttons->D_UP 	  = input[2] & 0x01;
	buttons->D_DN 	  = input[2] & 0x02;
	buttons->D_LEFT   = input[2] & 0x04;
	buttons->D_RIGHT  = input[2] & 0x08;
	buttons->START 	  = input[2] & 0x10;
	buttons->BACK 	  = input[2] & 0x20;
	buttons->LS_PRESS = input[2] & 0x40;
	buttons->RS_PRESS = input[2] & 0x80;

	buttons->LB 	  = input[3] & 0x01;
	buttons->RB 	  = input[3] & 0x02;
	buttons->LOGO     = input[3] & 0x04;
	buttons->SPARE    = input[3] & 0x08;
	buttons->A        = input[3] & 0x10;
	buttons->B        = input[3] & 0x20;
	buttons->X        = input[3] & 0x40;
	buttons->Y        = input[3] & 0x80;

	buttons->Left_trigger  = input[0x04];
	buttons->Right_trigger = input[0x05];
	buttons->Left_stick_X  = input[0x07] << 8 | input[0x06];
	buttons->Left_stick_Y  = input[0x09] << 8 | input[0x08];
	buttons->Right_stick_X = input[0x0b] << 8 | input[0x0a];
	buttons->Right_stick_Y = input[0x0d] << 8 | input[0x0c];

	return (result);
}
