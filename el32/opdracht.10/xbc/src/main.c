/*! \file main.c
    \brief Starting point of the application.
 
    \details This file contains #main routine which is the starting point of the apllication. From within this main
    several subroutines are called to demonstrate the capabilities of the Xbox controller.
 
    \author A.W Janisse
    \bug No known bugs.

    \version 1.0    First release.
*/

#include <stdio.h>      // for stderr
#include <stdlib.h>     // for exit_succes
#include <unistd.h>     // for sleep(), usleep()
#include <libusb-1.0/libusb.h>
#include "Devices.h"
#include "DemoController.h"

/*! \fn void initUSB()
    \brief Stuff to initialize the USB.
*/
void initUSB()
{
    if(libusb_init(NULL) < 0) 
    {
        fprintf(stderr, "*** Init Error ");
        exit(EXIT_FAILURE);
    }
    libusb_set_debug(NULL, 3);
}

/*! \fn void closeUSB(libusb_device_handle *handle)
    \brief Stuff to close and cleanup the USB.
*/
void closeUSB(libusb_device_handle *handle)
{
    libusb_close(handle);   //close the device we opened.
    libusb_exit(NULL);      //needs to be called to the end.  
}

/*! \fn int main()
    \brief Application main loop.
    
    \return exit_succes indicating a normal exit.
*/
int main()
{
	libusb_device_handle *handle;

    initUSB();

    printAllDevices();

	handle = getAndClaimHandle(VENDOR_ID, PRODUCT_ID);

    demoController(handle);
    
    closeUSB(handle);

    return (EXIT_SUCCESS);
}
