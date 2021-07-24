#include <cwchar>
#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "hidapi/hidapi.h"
#include <string.h>

#pragma comment(lib, "hidapi.lib")

#define MAX_STR 255

#define BITMASK_CONNECTED 0x4

void drawpx(int x, int y, int on, unsigned char * buffer){
	if(on)
		buffer[(x-1)+(y/8)*128] |= (1<<(y&7));
	else
		buffer[(x-1)+(y/8)*128] &= ~(1<<(y&7));
}

void print_vendor_data(){
	int res;
	wchar_t wstr[MAX_STR];
	hid_device* handle;

	// Initialize the hidapi library
	res = hid_init();
	handle = hid_open(0x1038, 0x1290, NULL);
	hid_set_nonblocking(handle, 0);
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	wprintf(L"Manufacturer String: %s\n", wstr);
	res = hid_get_product_string(handle, wstr, MAX_STR);
	wprintf(L"Product String: %s\n", wstr);
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	wprintf(L"Serial Number String: (%d) %s\n", wstr[0], wstr);
	hid_close(handle);
	res = hid_exit();
}

void draw_pixels(){
	int res;
	hid_device* handle;
	res = hid_init();
	hid_set_nonblocking(handle, 0);
	unsigned char data[1025] = {0};
	data[0] = 0x00; // Report type = 0;
	data[1] = 0xD2; // "Draw on screen" function
	drawpx(0, 0, 1, &data[2]);
	drawpx(127, 0, 1, &data[2]);
	drawpx(0, 39, 1, &data[2]);
	drawpx(127, 39, 1, &data[2]);
	res = hid_send_feature_report(handle, data, 1025);
	hid_close(handle);
	res = hid_exit();
}

void print_battery(){
	int res;
	hid_device* handle;
	res = hid_init();
	handle = hid_open(0x1038, 0x1290, NULL);
	hid_set_nonblocking(handle, 0);

	unsigned char data[33] = {0};

	data[0] = 0x00;
	data[1] = 0x40; // Battery in the headset
	data[2] = 0xAA; // Ask for data
	res = hid_send_feature_report(handle, data, 33);
	unsigned char received_data[33];
	res = hid_read(handle, received_data, 33);
	wprintf(L"Battery level is %d out of 4 in the headset\n", received_data[0]);

	data[0] = 0x00; // Report ID
	data[1] = 0x42; // Battery in the charger?
	data[2] = 0xAA; // Ask for data
	res = hid_send_feature_report(handle, data, 33);
	res = hid_read(handle, received_data, 33);
	wprintf(L"Battery level is %d out of 4 in the charger\n", received_data[0]);

	data[0] = 0x00; // Report ID
	data[1] = 0x41; // Headset connected or not and some other data?
	data[2] = 0xAA; // Ask for data
	res = hid_send_feature_report(handle, data, 33);
	res = hid_read(handle, received_data, 33);
	wprintf(L"Connected? %s\n", (received_data[0] & BITMASK_CONNECTED ? "Yes" : "No"));
	wprintf(L"??? %d\t%d\n", received_data[0], received_data[1]);

	hid_close(handle);
	res = hid_exit();
}

int main(int argc, char* argv[]){
	if(argc == 1){
		printf("Arguments:\n\td\tTries to draw 4 pixels on the OLED display\n\tb\tTries to get the battery level of the headset if it's currently powered on\n\tp\tPrint various information about the Arctis wireless pro basestation that is normal API usage.\n");
		return EXIT_SUCCESS;
	}else{
		if(strcmp(argv[1], "b") == 0){
			print_battery();
		}else if(strcmp(argv[1], "d") == 0){
			draw_pixels();
		}else if(strcmp(argv[1], "p") == 0){
			print_vendor_data();
		}
	}
	return EXIT_SUCCESS;
}
