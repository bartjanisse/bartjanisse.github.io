/*! \file DemoController.h
  	\brief Function prototypes for the DemoController.
 
   	\details This contains the prototypes for the controller and eventually any macros, constants or 
   	global variables you will need. Basicly it defines one function to demonstrate the Xbox controller
 
   	\author A.W Janisse
   	\bug No known bugs.

   	\version 1.0 	First release.
*/

#ifndef _DEMOCONTROLLER_
#define _DEMOCONTROLLER_

#include <libusb-1.0/libusb.h>
#include "Controller.h"

/*! \fn demoController(libusb_device_handle *handle)
    \brief When calling this function all the capablities of the Xbox controller are demonstrated.
    
    \param 	handle is a pointer to the USB device handle.
    
    \return 0 if succefull.

 	\pre 	A valid pointer to handle.
*/
void demoController(libusb_device_handle *h);

#endif