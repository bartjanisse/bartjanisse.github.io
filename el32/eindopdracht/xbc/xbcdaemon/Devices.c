/*! \file Devices.c
  	\brief Implementation for the Devices.
 
   	This is the implementation of the Devices.xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
 
   	\author A.W Janisse
   	\bug No known bugs.

   	\version 1.0 	First release.
*/

#include <stdio.h>
#include <stdlib.h>  // for exit()
#include <sys/types.h>
#include "Devices.h"

#define REFR_TIME_MS	100	//! Refresh time in ms for udating USB devices

void *pollingThread();

typedef struct device {
	int8_t bus;
	int8_t address;
	libusb_device_handle *handle;
} device;

device devices[MAX_DEVS] = {{-1,-1,NULL},{-1,-1,NULL},{-1,-1,NULL},{-1,-1,NULL}};
device tmp[MAX_DEVS] = {{-1,-1,NULL},{-1,-1,NULL},{-1,-1,NULL},{-1,-1,NULL}};

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_t 		thread;

uint8_t
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
        syslog(LOG_ERR, "Cannot Claim Interface\n");
        return (1);
    }
    return (0);
}

uint8_t
compareDevices(device *d1, device *d2)
{
	return (d1->bus == d2->bus) && (d1->address == d2->address);
}

void 
initDevices(uint16_t vendor_id, uint16_t product_id)
{	
    if(libusb_init(NULL) < 0) 
    {
		syslog(LOG_ERR, "Failed to iniitialize USB lib.");
        exit(EXIT_FAILURE);
    }
    libusb_set_debug(NULL, 1);

	if(pthread_create( &thread, NULL, &pollingThread, NULL) != 0)
	{
		syslog(LOG_ERR, "Failed to create thread.");
	}
}

void
closeDevices()
{
	int 	i;
	void *res;
	
	printf("Start closing\n");
	
	// Stop polling thread and wait for it
	if(pthread_cancel(thread) != 0)
	{
		syslog(LOG_ERR, "Failed to cancel device polling thread.");
	}
	if(pthread_join(thread, &res) != 0)
	{
		syslog(LOG_ERR, "Failed to join device polling thread.");
	}
	if (res == PTHREAD_CANCELED)
	{
		syslog(LOG_INFO, "Device polling thread has stopped");
    }          

	// Close all USB devices
	printf("Closing devices\n");
	for(i=0; i<MAX_DEVS; i++)
	{
		if (devices[i].handle != NULL)
		{
			libusb_close(devices[i].handle); 
			devices[i].handle = NULL;
		}
	}
    libusb_exit(NULL);  
    printf("Closing done\n");
}


libusb_device_handle *
getDeviceHandle(uint8_t id)
{
	libusb_device_handle *handle;
	
	if(id >= 0 && id < MAX_DEVS -1)
	{
		/* begin critical section */
		pthread_mutex_lock(&mutex1);	
		handle = devices[id].handle;
		/* end critical section */
		pthread_mutex_unlock(&mutex1);
		return handle;
	}
	else
	{
		return NULL;
	}
}

void
printAllDevices()
{
	int i;
	
	for(i=0; i<MAX_DEVS;i++)
	{
		if(devices[i].handle != NULL)
		{
			printf("Bus number %03d Device address %03d",
				devices[i].bus, 
				devices[i].address);
		}
	}
}


// Searches for the given device in the given list
// if found return 1, else 0
uint8_t
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

// Remove the give device from the given devices list. Threadsafe
void
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
			pthread_mutex_lock(&mutex1);
	
			devices[i].bus     = -1;
			devices[i].address = -1;
			devices[i].handle  = NULL;
	
			pthread_mutex_unlock(&mutex1);
			// end critical section
			
			printf("Removed controller with id=%d, from bus=%03d and address=%03d\n", i, dev->bus, dev->address);
			syslog(LOG_INFO,"Removed controller with id=%d, from bus=%03d and address=%03d", i, dev->bus, dev->address);
			break;
		}
	}
}

// Remove device from devices list if is not available anymore 
void
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
void
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
				pthread_mutex_lock(&mutex1);	
				
				devs[i].bus = dev->bus;
				devs[i].address = dev->address;
				devs[i].handle = dev->handle;	
				claimHandle(devs[i].handle);
				
				pthread_mutex_unlock(&mutex1);
				/* end critical section */
				printf("Added controller with id=%d, from bus=%03d and address=%03d\n", i, devs[i].bus, devs[i].address);
				syslog(LOG_INFO,"Added controller with id=%d, from bus=%03d and address=%03d", i, devs[i].bus, devs[i].address);
				
				break;
			}
		}	
	}
}

// Put new found devives in the devices list
void
listUpdateDevices(device devs[])
{
	uint8_t 	i; 
	
	for(i=0; i<MAX_DEVS; i++)
	{
		listTryAdd(devs, &tmp[i]);
	}
}

void
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
		syslog(LOG_ERR,"Failed to get device list");
		libusb_free_device_list(devs, 1);
		return;
	}
	
	i=id=0;
	while ((dev = devs[i++]) != NULL) 
	{
		struct libusb_device_descriptor desc;
		if((error = libusb_get_device_descriptor(dev, &desc)) != 0)
		{
			syslog(LOG_ERR,"Failed to get device descriptor");
			libusb_free_device_list(devs, 1);
			return;
		}
		if(desc.idVendor == VENDOR_ID && desc.idProduct == PRODUCT_ID)
		{
			tmp[id].bus = libusb_get_bus_number(dev);
			tmp[id].address = libusb_get_device_address(dev);
			
			if(listContains(devices, &tmp[id]) == 0)
			{
				printf("Open USB device\n");
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


void *
pollingThread()
{
	syslog(LOG_INFO, "Device polling thread is started");
	
	/* Guarantees that thread resources are deallocated upon return */
	if (pthread_detach(pthread_self()) != 0) 
	{
		syslog(LOG_ERR, "Device polling thread error in detach");
	}
	
	if(pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL) !=0)
	{
		syslog(LOG_ERR, "Device polling thread error in setcancelstate");;
	}	
	
	while(1)
	{
		getDevices();
		listClean();
		listUpdateDevices(devices);
		//printAllDevices();
		//printf("\n\n");
		usleep(REFR_TIME_MS * 1000);
	}	
	return (NULL);
}
