/*! \file Devices.h
  	\brief Function prototypes for Devices.
 
   	\details This contains the prototypes for the Devices.
 
   	\author A.W Janisse
   	\bug No known bugs.

   	\version 1.0 	First release.
*/

#ifndef _DEVICES_H_
#define _DEVICES_H_

#include <libusb-1.0/libusb.h>
#include "Controller.h"

/*! \fn printAllDevices();
    \brief Prints a list of <b>all</b> the USB devices to the console.

    \details This function prints a list of all the usb devives to the standard console.
*/
void printAllDevices();

/*! \fn libusb_device_handle *getAndClaimHandle(uint16_t vendor_id, uint16_t product_id)
	\brief Retreives a handle for the specified vendor_id and product_is.

	\details When the kernel is attached to this device it will try to detach it. After detaching it will claim the interface. 
	If claiming is not succesfulle the application will be aborted.

	\param 	vendor_id The macro #VENDOR_ID specified in in controller.h and is used to retreive a handle to the Xbox controller device.
	\param 	product_id The macro #PRODUCT_ID specified in in controller.h and is used to retreive a handle to the Xbox controller device.
	
	\return	0 if succefull.
	
	\pre 	A valid pointer to handle and button.
	\post	The structure buttons is filled with the current state.
*/
libusb_device_handle *getAndClaimHandle(uint16_t vendor_id, uint16_t product_id);

#endif