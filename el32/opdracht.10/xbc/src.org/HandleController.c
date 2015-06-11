/** @file HandleController.c
 *  @brief Function prototypes for the console driver.
 *
 *  @details This contains the prototypes for xxxxxxxxxxxxxxxxxxxxxxx
 *
 *  @author A.W Janisse
 *  @bug No known bugs.
 */
#include <stdio.h>
#include "Controller.h"

void 
HandleController(libusb_device_handle *handle)
{
	// 'handle' is obtained from libusb_open_device()

	Buttons 	buttons;
	bool		to_quit = false;

	printf("Handle controller starting\n");

	setControllerLeds(handle, all_off);

	while(to_quit == false)
	{
		getControllerInput(handle, &buttons);

		if(buttons.Y == true) 
			setControllerLeds(handle, led_1_on);
		else if(buttons.B == true)
			setControllerLeds(handle, led_2_on);
		else if(buttons.X == true)
			setControllerLeds(handle, led_3_on);
		else if(buttons.A == true)
			setControllerLeds(handle, led_4_on);
		else
			setControllerLeds(handle, all_off);

		if(buttons.LOGO)
		{
			to_quit = true;
		}
	}

	printf("Handle controller end\n");
}

