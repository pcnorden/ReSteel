#pragma once // Make sure that the compiler won't complain incase we include this header file in more than 1 source file

#include <hidapi/hidapi.h>
#include <cwchar>
#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void drawpix(int x, int y, u_char on, u_char *buffer){
	if(on)
		buffer[(x-1)+(y/8)*128] |= (1<<(y&7));
	else
		buffer[(x-1)+(y/8)*128] &= ~(1<<(y&7));
}

int check_for_errors_and_exit(hid_device* handle){
	if(hid_error(handle) != NULL){
		wprintf(hid_error(handle));
		if(handle != NULL)
			hid_close(handle);
		hid_exit();
		return -1;
	}
	return 0;
}