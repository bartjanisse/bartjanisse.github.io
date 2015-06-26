/** \file devices.c
 *  \ingroup xbcdeamon
 * 	\brief Implementation for the devices
 *
 * 	\author A.W Janisse
 * 	\bug No known bugs.
 *
 * 	\version 1.0 	First release.
*/

#include <stdio.h>
#include <stdlib.h>  // for exit()
#include <sys/types.h>
#include "devices.h"

static uint16_t vendor_id;
static uint16_t product_id;

static uint8_t running = 1;

static void *pollingThread();

/** \ingroup xbcdeamon
 *  \brief structure which defines a USB device
 */
typedef struct{
	int8_t bus;						//!<  Bus number to which the USB device is conected
	int8_t address;					//!<  Address to which the USB device is connected
	libusb_device_handle *handle;	//!<  Handle to the USB device
} device;

static device devices[MAX_DEVS];
static device tmp[MAX_DEVS];

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_t 		thread;

static uint8_t
compareDevices(device *d1, device *d2)
{
	return (d1->bus == d2->bus) && (d1->address == d2->address);
}

static uint8_t
claimHandle(libusb_device_handle *handle)
{
	// Find out if kernel driver is attached and if so detach it
	if(libusb_kernel_driver_active(handle, 0) == 1)  
	{
        libusb_detach_kernel_driver(handle, 0);
    }

	// Claim interface
    if(libusb_claim_interface(handle, 0) < 0) 
    {
        LOGERR("Cannot Claim Interface");
        return (1);
    }
    return (0);
}

void 
initDevices(uint16_t vendorID, uint16_t productID)
{	  
	int8_t i;
	
	vendor_id  = vendorID;
	product_id = productID;
		
	for(i=0; i<MAX_DEVS;i++)
	{
		devices[i].bus     = -1;
		devices[i].address = -1;
		devices[i].handle  = NULL;
		
		tmp[i].bus     = -1;
		tmp[i].address = -1;
		tmp[i].handle  = NULL;
	}
	
    if(libusb_init(NULL) < 0) 
    {
		LOGERR("Failed to iniitialize USB lib.");
    }
    libusb_set_debug(NULL, 1);

	if(pthread_create( &thread, NULL, &pollingThread, NULL) != 0)
	{
		LOGERR("Failed to create thread.");
	}
}

void
closeDevices()
{
	int 	i;
	
	syslog(LOG_INFO, "Start closing devices");
	running = 0;
	
	/* Wait for the polling thread to stop */
	pthread_join(thread, NULL);          
	syslog(LOG_INFO, "Polling thread has stopped");
	
	// Close all USB devices
	for(i=0; i<MAX_DEVS; i++)
	{
		if (devices[i].handle != NULL)
		{
			libusb_close(devices[i].handle); 
			devices[i].handle = NULL;
		}
	}
    libusb_exit(NULL);  
    
    syslog(LOG_INFO, "Devices are closed");
}


libusb_device_handle *
getDeviceHandle(uint8_t id)
{
	libusb_device_handle *handle;
		
	if(id >= 0 && id < MAX_DEVS)
	{
		/* begin critical section */
		pthread_mutex_lock(&mutex);	
		handle = devices[id].handle;
		/* end critical section */
		pthread_mutex_unlock(&mutex);
		
		return handle;
	}
	else
	{
		return NULL;
	}
}

// Searches for the given device in the given list
// if found return 1, else 0
/** \ingroup xbcdeamon
 *  \brief function to chek if the given device is in the given list
 */
static uint8_t
listContains(device devs[], device *dev)
{
	int i;

	for(i=0; i<MAX_DEVS;i++)
	{
		if(compareDevices(&devs[i], dev))
		{
			return (1);
		}
	}
	return (0);
}

// Remove the given device from the given devices list. Threadsafe
static void
listTryRemove(device devs[], device *dev)
{
	int i;
	
	if(dev->handle != NULL)
	{
		libusb_close(dev->handle);
	}
	for(i=0;i<MAX_DEVS;i++)
	{
		if(compareDevices(&devs[i], dev))
		{
			// begin critical section
			pthread_mutex_lock(&mutex);
	
			devices[i].bus     = -1;
			devices[i].address = -1;
			devices[i].handle  = NULL;
	
			pthread_mutex_unlock(&mutex);
			// end critical section
			
			syslog(LOG_INFO,"Removed controller with id=%d, from bus=%03d and address=%03d", i, dev->bus, dev->address);
			break;
		}
	}
}

// Remove device from devices list if is not available anymore 
static void
listClean()
{
	uint8_t 	i;
	device		dev;
	
	for(i=0; i<MAX_DEVS; i++)
	{
		dev = devices[i];
		if(dev.bus != -1 && dev.address != -1)
		{	
			if(listContains(tmp, &dev) == 0)
			{
				listTryRemove(devices, &dev);
			}	
		} 
	}
}

// Add unique device at the first availlable position in the give list.
// This is threadsafe and the USB handle will be claimed
static void
listTryAdd(device devs[], device *dev)
{
	uint8_t 	i;
	
	if(listContains(devs, dev))
	{
		//Already in the list...return
		return;
	}
	
	if(dev->handle != NULL)
	{	
		for(i=0;i<MAX_DEVS;i++)
		{
			if(devs[i].handle == NULL)
			{					
				/* begin critical section */
				pthread_mutex_lock(&mutex);	
				
				devs[i].bus = dev->bus;
				devs[i].address = dev->address;
				devs[i].handle = dev->handle;
				
				/* Make sure the handle is claimend before other threads use this device */	
				claimHandle(devs[i].handle);
				
				pthread_mutex_unlock(&mutex);
				/* end critical section */

				syslog(LOG_INFO,"Added controller with id=%d, bus=%03d, address=%03d", i, devs[i].bus, devs[i].address);
				
				break;
			}
		}	
	}
}

// Put new found devives in the devices list
static void
listUpdateDevices(device devs[])
{
	uint8_t 	i; 
		
	for(i=0; i<MAX_DEVS; i++)
	{
		listTryAdd(devs, &tmp[i]);
	}
}

static void
getDevices()
{
	libusb_device 	**devs;
	libusb_device 	*dev;
	uint8_t			i, id;
	int 			error;

	for(i=0;i<MAX_DEVS;i++)
	{
		tmp[i].bus =-1;
		tmp[i].address = -1;
		tmp[i].handle = NULL;
	}

	if(libusb_get_device_list(NULL, &devs) < 0) 
	{
		LOGERR("Failed to get device list");
		libusb_free_device_list(devs, 1);
		return;
	}
	
	i=id=0;
	while ((dev = devs[i++]) != NULL) 
	{
		struct libusb_device_descriptor desc;
		if((error = libusb_get_device_descriptor(dev, &desc)) != 0)
		{
			LOGERR("Failed to get device descriptor");
			libusb_free_device_list(devs, 1);
			return;
		}
		if(desc.idVendor == vendor_id && desc.idProduct == product_id)
		{
			tmp[id].bus = libusb_get_bus_number(dev);
			tmp[id].address = libusb_get_device_address(dev);
			
			if(listContains(devices, &tmp[id]) == 0)
			{
				libusb_open(dev, &tmp[id].handle);
			}

			id++;

			if (id == MAX_DEVS)
			{
				// Limit is reached, stop
				break;
			}		
		}
	}
	libusb_free_device_list(devs, 1);
}

static void *
pollingThread()
{
	syslog(LOG_INFO, "Device polling thread is started");

	while(running)
	{
		getDevices();
		listClean();
		listUpdateDevices(devices);
		usleep(REFR_TIME_MS * 1000);
	}	

	syslog(LOG_INFO, "Device polling thread is stopped");
	return (NULL);
}
