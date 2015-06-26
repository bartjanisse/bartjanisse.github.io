/** \file devices.h
 *  \ingroup xbcdeamon
 * 	\brief Function prototypes for the devices
 *
 * 	\author A.W Janisse
 * 	\bug No known bugs.
 *
 * 	\version 1.0 	First release.
*/

#ifndef _DEVICES_H_
#define _DEVICES_H_

#include <libusb-1.0/libusb.h>
#include <pthread.h>
#include <unistd.h>
#include "../common/common.h"


/** \ingroup xbcdeamon
 *  \brief Constant for vendor id which is used to derive the handle.
 */
#define 	VENDOR_ID	0x045e

/** \ingroup xbcdeamon
 *  \brief Constant for product id which is used to derive the handle.
 */
#define 	PRODUCT_ID	0x028e

/** \ingroup xbcdeamon
 *  \brief Refresh time in ms for udating USB devices 
 */
#define REFR_TIME_MS	100		

/** \ingroup xbcdeamon
 *  \brief function to get the device handle based on the index
 * 
 *  \param id is the controller id
 *  \return a handle to the the device
 */
libusb_device_handle *getDeviceHandle(uint8_t id);

/** \ingroup xbcdeamon
 *  \brief function to initialize the libusb library an start the polling thread
 * 
 * The polling threads constantly monitors if a USB device is plugged in or out.
 * When this happens the internal devicelist is updated accordingly. Only USB devices
 * which match the givven vendor_id and product_id will be put into the devices list.
 * 
 *  \param vendor_id for the USB device
 *  \param product_id for the the device
 */
void initDevices(uint16_t vendor_id, uint16_t product_id);

/** \ingroup xbcdeamon
 *  \brief function to close the libusb library and to stop the polling thread
 * 
 */
void closeDevices();

#endif
