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
#include "controller.h"

#define TIMEOUT 	1000	//!< Time-out of 1000ms for USB transfer

int
setControllerRumble(libusb_device_handle *handle, uint16_t speed)
{
	uint8_t 	rumble[] = { 0x00, 0x08, 0x00, speed >> 8, speed, 0x00, 0x00, 0x00 };
 	int 		result, transferred;

	if(handle == NULL) 
		return (1); //Return positive since libusb will return negative values

 	result = libusb_interrupt_transfer(handle, EP_OUT, rumble, sizeof rumble, &transferred, TIMEOUT);
	
	return (result);
}

int 
setControllerLeds(libusb_device_handle *handle, Leds led)
{
	uint8_t 	data[] = { 0x01, 0x03, led };
 	int 		result, transferred;

	if(handle == NULL) 
		return (1); //Return positive since libusb will return negative values

 	result = libusb_interrupt_transfer(handle, EP_OUT, data, sizeof data, &transferred, TIMEOUT);

	return (result);
}

int 
getControllerInput(libusb_device_handle *handle, button *b)
{
	uint8_t 	input[20];
	int 		result, transferred;
	
	if(b == NULL) 
		return (1); //Return positive since libusb will return negative values

	if(handle == NULL)
	{
		b->avail = 0;
		return (1);
	}

	b->avail = 1;

	result = libusb_interrupt_transfer(handle, EP_IN, input, sizeof input, &transferred, TIMEOUT);  

	b->D_UP 	= input[2] & 0x01;
	b->D_DN 	= input[2] & 0x02;
	b->D_LEFT  	= input[2] & 0x04;
	b->D_RIGHT 	= input[2] & 0x08;
	b->START  	= input[2] & 0x10;
	b->BACK   	= input[2] & 0x20;
	b->LS_PRESS	= input[2] & 0x40;
	b->RS_PRESS	= input[2] & 0x80;

	b->LB 	  	= input[3] & 0x01;
	b->RB 	  	= input[3] & 0x02;
	b->LOGO     = input[3] & 0x04;
	b->SPARE    = input[3] & 0x08;
	b->A        = input[3] & 0x10;
	b->B        = input[3] & 0x20;
	b->X        = input[3] & 0x40;
	b->Y        = input[3] & 0x80;

	b->Left_trigger  = input[0x04];
	b->Right_trigger = input[0x05];
	b->Left_stick_X  = input[0x07] << 8 | input[0x06];
	b->Left_stick_Y  = input[0x09] << 8 | input[0x08];
	b->Right_stick_X = input[0x0b] << 8 | input[0x0a];
	b->Right_stick_Y = input[0x0d] << 8 | input[0x0c];
	
	return (result);
}
