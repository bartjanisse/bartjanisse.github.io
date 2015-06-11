/*
 * de volgende code laat het ledje op de gamepad rondspinnen: 
 * 045e:028e Microsoft Corp. Xbox360 Controller
*/
 
#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <unistd.h>     // for sleep()

int
main(int argc, char *argv[])
{
	libusb_device_handle *h;
	unsigned char data[] = { 1, 3, 0x0a };
	unsigned char rumbleMax[] = { 0x00, 0x08, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00 };
	unsigned char rumbleOff[] = { 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	int error, transferred;

	libusb_init(NULL);
	h = libusb_open_device_with_vid_pid(NULL, 0x045e, 0x028e);
	if (h == NULL) {
		fprintf(stderr, "Failed to open device\n");
		return (1);
	}

	if ((error = libusb_interrupt_transfer(h, 0x02, data, sizeof data, &transferred, 0)) != 0) {
		fprintf(stderr, "Transfer blink failed: %d\n", error);
		return (1);
	}

	if ((error = libusb_interrupt_transfer(h, 0x02, rumbleMax, sizeof rumbleMax, &transferred, 0)) != 0) {
		fprintf(stderr, "Transfer rumble max failed: %d\n", error);
		return (1);
	}

	sleep(1);

	if ((error = libusb_interrupt_transfer(h, 0x02, rumbleOff, sizeof rumbleOff, &transferred, 0)) != 0) {
		fprintf(stderr, "Transfer rumble off failed: %d\n", error);
		return (1);
	}

	return (0);
}