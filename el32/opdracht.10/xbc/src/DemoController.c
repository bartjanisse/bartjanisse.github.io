/*! \file DemoController.c
  	\brief Implementation for the DemoController.
 
   	\details This is the implementation of the DemoController.
 
   	\author A.W Janisse
   	\bug No known bugs.

   	\version 1.0 	First release.
*/

#include <stdio.h>
#include <stdlib.h>		// for abs()
#include <unistd.h>     // for sleep(), usleep()
#include <assert.h>
#include "Controller.h"
#include "DemoController.h"

libusb_device_handle 	*h;					//!< Reference to handle for local use.
Buttons 				buttons; 			//!< Buttons structure for local use.
bool					to_quit = false; 	//!< Flag signalling a step to quit.

static void waitForRelease();				//<! Prototype for a function which waits till all buttons of the controller are released.
static void demoLeds();						//!< Prototype for a function for demonstration some LED's
static void demoRumble();					//!< Prototype for a function to demonstrate the rumble actuator.
static void demoButtons();					//!< Prototype for a function to display which button is pressed.
static void demoAnalog();					//!< Prototype for a function to demonstrate the analog joysticks.

void 
demoController(libusb_device_handle *handle)
{
	// 'handle' is obtained from libusb_open_device()
	assert(handle != NULL);

	h = handle;
	
	setControllerLeds(h, all_off);

	demoLeds();
	demoRumble();
	demoButtons();
	demoAnalog();

	printf("All done. Leaving demonstrations.\n");
}

void
waitForRelease()
{
	while(
	 	buttons.D_UP ||
		buttons.D_DN ||
		buttons.D_LEFT ||
		buttons.D_RIGHT ||
		buttons.START ||
		buttons.BACK ||
		buttons.LS_PRESS ||
		buttons.RS_PRESS ||
		buttons.LB ||
		buttons.RB ||
		buttons.LOGO ||
		buttons.SPARE ||
		buttons.A ||
		buttons.B ||
		buttons.X ||
		buttons.Y )
	{
		getControllerInput(h, &buttons);
	}
	usleep(100000); // 100ms delay;	
}

void 
demoLeds()
{
	to_quit = false;

	printf("\nPress button A, B, X or Y to light the LED's. Press back to go to the next step.\n");

	while(to_quit == false)
	{
		getControllerInput(h, &buttons);

		if(buttons.Y == true) 
			setControllerLeds(h, led_1_on);
		else if(buttons.B == true)
			setControllerLeds(h, led_2_on);
		else if(buttons.X == true)
			setControllerLeds(h, led_3_on);
		else if(buttons.A == true)
			setControllerLeds(h, led_4_on);
		else
			setControllerLeds(h, all_off);

		if(buttons.BACK)
		{
			printf("Leaving LED's demonstration.\n");
			to_quit = true;
		}
	}
	waitForRelease();
}

void 
demoRumble()
{
	to_quit = false;

	printf("\nUse the left joystick X to controll the rumble. Press back to go to the next step.\n");

	while(to_quit == false)
	{
		getControllerInput(h, &buttons);

		setControllerRumble(h, abs(buttons.Left_stick_X));
		
		if(buttons.BACK)
		{
			printf("Leaving rumble demonstration.\n");
			
			setControllerRumble(h, 0);

			to_quit = true;
		}
	}	
	waitForRelease();
}

void
demoButtons()
{
	to_quit = false;

	printf("\nPress any button. Press back to go to the next step.\n");

	while(to_quit == false)
	{
		getControllerInput(h, &buttons);

		if(buttons.D_UP)
		{
			printf("D-up button pressed\n");
		}
		if(buttons.D_DN)
		{
			printf("D-down button pressed\n");
		}
		if(buttons.D_LEFT)
		{
			printf("D-left button pressed\n");
		}
		if(buttons.D_RIGHT)
		{
			printf("D-right button pressed\n");
		}
		if(buttons.START)
		{
			printf("Start button pressed\n");
		}
		if(buttons.LS_PRESS)
		{
			printf("LS button pressed\n");
		}
		if(buttons.RS_PRESS)
		{
			printf("RS button pressed\n");
		}
		if(buttons.LB)
		{
			printf("LB button pressed\n");
		}
		if(buttons.RB)
		{
			printf("RB button pressed\n");
		}
		if(buttons.LOGO)
		{
			printf("Logo button pressed\n");
		}
		if(buttons.A)
		{
			printf("A button pressed\n");
		}
		if(buttons.B)
		{
			printf("B button pressed\n");
		}
		if(buttons.X)
		{
			printf("X button pressed\n");
		}
		if(buttons.Y)
		{
			printf("Y button pressed\n");
		}
	
		if(buttons.BACK)
		{
			printf("Leaving button demonstration.\n");
			to_quit = true;
		}

		waitForRelease();
	}	
}


void
demoAnalog()
{
	to_quit = false;

	printf("\nUse the joysticks or triggerbuttons. Press back to go to the next step.\n");

	while(to_quit == false)
	{
		getControllerInput(h, &buttons);

		if(buttons.Left_trigger > 0)
		{
			printf("Left trigger value = %d\n", buttons.Left_trigger);
		}
		if(buttons.Right_trigger > 0)
		{
			printf("Right trigger value = %d\n", buttons.Right_trigger);
		}
		if(abs(buttons.Left_stick_X) > 500)
		{
			printf("Left-X value = %d\n", buttons.Left_stick_X);
		}
		if(abs(buttons.Left_stick_Y) > 500)
		{
			printf("Left-Y value = %d\n", buttons.Left_stick_Y);
		}
		if(abs(buttons.Right_stick_X) > 500)
		{
			printf("Right-X value = %d\n", buttons.Right_stick_X);
		}
		if(abs(buttons.Right_stick_Y) > 500)
		{
			printf("Right-Y value = %d\n", buttons.Right_stick_Y);
		}

		if(buttons.BACK)
		{
			printf("Leaving joystick demonstration.\n");
			to_quit = true;
			waitForRelease();
		}
	}
}