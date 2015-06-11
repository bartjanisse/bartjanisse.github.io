/*! \file Devices.c
  	\brief Implementation for the Devices.
 
   	This is the implementation of the Devices.
 
   	\author A.W Janisse
   	\bug No known bugs.

   	\version 1.0 	First release.
*/

#include <stdio.h>
#include <stdlib.h>  // for exit()
#include "Devices.h"


libusb_device_handle *
getAndClaimHandle(uint16_t vendor_id, uint16_t product_id)
{
	libusb_device_handle *handle;

	handle = libusb_open_device_with_vid_pid(NULL, vendor_id, product_id);
	if (handle == NULL) 
    {
		fprintf(stderr, "*** Failed to open device. Is it plugged in and did you run with sudo?\n");
		exit (EXIT_FAILURE);
	}

	// Find out if kernel driver is attached and if so detach it
	if(libusb_kernel_driver_active(handle, 0) == 1)  
	{
        fprintf(stdout, "Kernel Driver Active\n");

        if(libusb_detach_kernel_driver(handle, 0) == 0)
        {
        	fprintf(stdout, "Kernel Driver Detached!\n");
        }
    }

	// Claim interface
    if(libusb_claim_interface(handle, 0) < 0) 
    {
        fprintf(stderr, "Cannot Claim Interface\n");
        exit (EXIT_FAILURE);
    }

	return (handle);
}

void
printAllDevices()
{
	libusb_device 	**devices;
	libusb_device 	*device;
	int 			i = 0;
	int 			error;
	
	error = libusb_get_device_list(NULL, &devices);
	if(error < 0) 
	{
	    fprintf(stderr, "*** Error. Failed to get devices");
	    exit (EXIT_FAILURE);
	}

	while ((device = devices[i++]) != NULL) 
	{
		struct libusb_device_descriptor descriptor;
		if((error = libusb_get_device_descriptor(device, &descriptor)) != 0)
		{
			fprintf(stderr, "*** Error. Failed to get device descriptor");
			exit (EXIT_FAILURE);
		}

		printf("Bus %03d Device %03d: ID %04x:%04x\n",
			libusb_get_bus_number(device), 
			libusb_get_device_address(device), 
			descriptor.idVendor, 
			descriptor.idProduct);
	}

	libusb_free_device_list(devices, 1);
}
