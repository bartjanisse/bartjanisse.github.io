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
#include <pthread.h>
#include <unistd.h>
#include "../common/common.h"

//! VENDOR_ID
/*! Constant for vendor id. This constant is used to derive the handle.*/
#define 	VENDOR_ID	0x045e
//! PRODUCT_ID
/*! Constant for product id. This constant is used to derive the handle.*/
#define 	PRODUCT_ID	0x028e

#define REFR_TIME_MS	100	//! Refresh time in ms for udating USB devices


/*! \fn xxxxxxxxxxxxxxxxxxxxxxxxxx();
    \brief xxxxxxxxxxxx <b>all</b> x.

    \details xxxxxxxxxxxxxxxxxx.
*/
libusb_device_handle *getDeviceHandle(uint8_t id);

void initDevices(uint16_t vendor_id, uint16_t product_id);

void closeDevices();

#endif
