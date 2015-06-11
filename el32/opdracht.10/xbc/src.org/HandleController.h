/** @file HandleController.h
 *  @brief Function prototypes for the console driver.
 *
 *  @details This contains the prototypes for xxxxxxxxxxxxxxxxxxxxxxx
 *
 *  @author A.W Janisse
 *  @bug No known bugs.
 */

#ifndef _HANDLECONTROLLER_
#define _HANDLECONTROLLER_

#include <libusb-1.0/libusb.h>
#include "Controller.h"

void 
HandleController(libusb_device_handle *handle);


#endif