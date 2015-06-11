/** @file Devices.h
 *  @brief Function prototypes for the console driver.
 *
 *  This contains the prototypes for xxxxxxxxxxxxxxxxxxxxxxx
 *
 *  @author A.W Janisse
 *  @bug No known bugs.
 */
#ifndef _DEVICES_H_
#define _DEVICES_H_

#include <libusb-1.0/libusb.h>
#include "Controller.h"

/** @brief Retreives a fresh list of connected USB devives.
 *
 *  @param devices is a pointer to the list
 */
int getDevices(libusb_device **devices);

/** @brief Prints a list of connected USB devices
 *
 *  This function prints information about all the current connected
 *  USB devices to the standard console.
 *
 *  @param devices is a pointer to the list with devices.
 *  @return 0 if succefull or error code if fails.
 */
int  printAllDevices(libusb_device **devices);



#endif