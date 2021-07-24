#include <stdio.h>
#include <stdlib.h>
#include <hidapi/hidapi.h>
#include <iostream>

#define MAX_STR 255

int main(int argc, char* argv[]){
	int res;
	wchar_t wstr[MAX_STR];
	hid_device* handle;

	// Initialize the hidapi library
	res = hid_init();
	if(res){
		std::cerr << "Something went wrong during hid_init() which shouldn't happen since it can't return anything else than 0" << std::endl;
		return EXIT_FAILURE;
	}

	// Open the device using the VID & PID, no serial number.
	handle = hid_open(0x1038, 0x1290, NULL);

	std::cout << "If getting SEGFAULT after this, 99\% chance that this software is not ran as sudo!" << std::endl;
	if(hid_set_nonblocking(handle, 0)){
		std::cerr << "hid_set_nonblocking returned a error, aborting launch! Is udev rule installed or are you running the program as sudo?" << std::endl;
		return EXIT_FAILURE;
	}

	// Read the manufacturer string
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	wprintf(L"Manufacturer String: %s\n", wstr);

	// Get the product string
	res = hid_get_product_string(handle, wstr, MAX_STR);
	wprintf(L"Product String: %s\n", wstr);

	// Get the serial number string
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	wprintf(L"Serial Number String: (%d) %s\n", wstr[0], wstr);

	hid_close(handle);

	res = hid_exit();

	return EXIT_SUCCESS;
}