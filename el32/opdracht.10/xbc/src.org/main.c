/** @file main.c
 *  @brief Function prototypes for the console driver.
 *
 *  @details This contains the prototypes for xxxxxxxxxxxxxxxxxxxxxxx
 *
 *  @author A.W Janisse
 *	@version 1.0  @date 28-05-2015
 *  
 */

#include <stdio.h>
#include <unistd.h>     // for sleep(), usleep()
#include <pthread.h>
#include <libusb-1.0/libusb.h>
#include "Devices.h"
#include "HandleController.h"

#define THRESHOLD 150	//!< Treshold level for displaying analog values

static void *workerThread(void *arg);            /* thread that does the work */

void 
printUsage()
{
	printf("Press the Xbox logo button on a controller to quit that controller.\n");
}

/** @brief this main func....................
 *  
 *  @ return ??????
 */
int
main(int argc, char *argv[])
{
	int 					r;
	// ssize_t 				devCntr = 0;
	libusb_device_handle 	*handle;
	// libusb_device 			**devices;
	// libusb_context 			*ctx = NULL;
	// Buttons 				buttons;
	pthread_t   threadID;     /* Thread ID from pthread_create() */

	// // Get controller devices
	printUsage();
	r = libusb_init(NULL);
	if(r < 0) 
	{
        fprintf(stderr, "Init Error "); //there was an error
        return (1);
    }
    libusb_set_debug(NULL, 3); //set verbosity level to 3, as suggested in the documentation

	// devCntr = libusb_get_device_list(NULL, &devices);

	// if (devCntr < 0)
	// 	return (int) devCntr;

	// //printAllDevices(devices);
	// getDevices(devices);

	// libusb_free_device_list(devices, 1);
	


	handle = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, VENDOR_PROD);
	if (handle == NULL) {
		fprintf(stderr, "*** Failed to open device. Is it plugged in and did you run with sudo?\n");
		return (1);
	}

	//find out if kernel driver is attached
	if(libusb_kernel_driver_active(handle, 0) == 1)  
	{
        fprintf(stdout, "Kernel Driver Active\n");

        if(libusb_detach_kernel_driver(handle, 0) == 0) //detach it
        {
        	fprintf(stdout, "Kernel Driver Detached!\n");
        }
    }

	//claim interface 0 (the first) of device (mine had jsut 1)
	r = libusb_claim_interface(handle, 0); 
    if(r < 0) 
    {
        fprintf(stderr, "Cannot Claim Interface\n");
        return (1);
    }
    fprintf(stdout, "Claimed Interface\n");

    //HandleController(handle);
    r = pthread_create (&threadID, NULL, workerThread, (void *) handle);
    if (r == 0)
    {
        printf("Succesfully created new thread\n");
    }
    else
    {
        fprintf(stderr, "pthread_create()");
        return (1);
    }

    libusb_close(handle); //close the device we opened
	libusb_exit(NULL); //needs to be called to end the

    pthread_exit(NULL);
}

static void *
workerThread (void *threadArgs)
{
    // libusb_device_handle 	*handle;

    // handle = (libusb_device_handle)threadArgs;

    libusb_device_handle* handle = (libusb_device_handle*)threadArgs; /* Cast the void* to our struct type */

    printf("Thread started\n");

    if (pthread_detach (pthread_self ()) != 0)
    {
        printf("pthread_detach()");
    }

    // Do working
    HandleController(handle);

    printf("Thread exit\n");
    pthread_exit(NULL);
}