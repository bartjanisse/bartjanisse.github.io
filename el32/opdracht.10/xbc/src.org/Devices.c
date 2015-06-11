/** @file Devices.c
 *  @brief Function prototypes for the console driver.
 *
 *  This contains the prototypes for xxxxxxxxxxxxxxxxxxxxxxx
 *
 *  @author A.W Janisse
 *  @bug No known bugs.
 */

#include <stdio.h>
#include "Devices.h"


libusb_device *xbdevices[4] = {NULL, NULL, NULL, NULL};

int 
getDevices(libusb_device **devices)
{
	libusb_device *device;
	int i=0, idx = 0;
	int error;
	while ((device = devices[i++]) != NULL) 
	{
		struct libusb_device_descriptor descriptor;
		if((error = libusb_get_device_descriptor(device, &descriptor)) != 0)
		{
			fprintf(stderr, "*** Failed to get device descriptor");
			return (error);
		}
		if(descriptor.idVendor == VENDOR_ID && descriptor.idProduct == VENDOR_PROD)
		{
			if(idx <= 3)
			{
				xbdevices[idx] = devices[i];
				idx++;
			}else{
				
				fprintf(stderr, "*** Error. More than 4 xbox controller found");
				return (error);
			}
		}
	}
	printAllDevices(xbdevices);
	return (0);
}

int 
printAllDevices(libusb_device **devices)
{
	libusb_device *device;
	int i = 0;
	int error;
	while ((device = devices[i++]) != NULL) 
	{
		struct libusb_device_descriptor descriptor;
		if((error = libusb_get_device_descriptor(device, &descriptor)) != 0)
		{
			fprintf(stderr, "Failed to get device descriptor");
			return (error);
		}

		printf("Bus %03d Device %03d: ID %04x:%04x\n",
			libusb_get_bus_number(device), 
			libusb_get_device_address(device), 
			descriptor.idVendor, 
			descriptor.idProduct);
	}

	return (0);
}
