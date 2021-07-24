#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "hidapi/hidapi.h"

#pragma comment(lib, "hidapi.lib")

#define MAX_STR 255

void drawpx(int x, int y, int on, unsigned char * buffer){
	if(on)
		buffer[(x-1)+(y/8)*128] |= (1<<(y&7));
	else
		buffer[(x-1)+(y/8)*128] &= ~(1<<(y&7));
}

int main(int argc, char* argv[]){
	int res;
	wchar_t wstr[MAX_STR];
	hid_device* handle;

	// Initialize the hidapi library
	res = hid_init();
	if(res){
		printf("Something went wrong during hid_init() which shouldn't happen since it can't return anything else than 0\n");
		return EXIT_FAILURE;
	}

	// Open the device using the VID & PID, no serial number
	handle = hid_open(0x1038, 0x1290, NULL);

	hid_set_nonblocking(handle, 0);

	// Read the manufacturer string
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	wprintf(L"Manufacturer String: %s\n", wstr);

	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	wprintf(L"Manufacturer String: %s\n", wstr);

	// Read the Product String
	res = hid_get_product_string(handle, wstr, MAX_STR);
	wprintf(L"Product String: %s\n", wstr);

	// Read the Serial Number string
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	wprintf(L"Serial Number String: (%d) %s\n", wstr[0], wstr);

	char data[1025] = {0};
	data[0] = 0x00; // Report type = 0;
	data[1] = 0xD2; // "Draw on screen" function

	drawpx(0, 0, 1, &data[2]);
	drawpx(127, 0, 1, &data[2]);
	drawpx(0, 39, 1, &data[2]);
	drawpx(127, 39, 1, &data[2]);
	res = hid_send_feature_report(handle, data, 1025);

	// Close the device
	hid_close(handle);

	// Finalize the hidapi library
	res = hid_exit();

	return EXIT_SUCCESS;
}
