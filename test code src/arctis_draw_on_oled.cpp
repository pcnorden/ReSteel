#include "helper.hxx"

#pragma comment(lib, "hidapi.lib")

#define MAX_STR 255
#define BITMASK_CONNECTED 0x4

/** @brief Initalized the HIDAPI library.
 * Calling this function will initalize the HIDAPI library and try to open
 * device with ID 0x1038 0x1290, check for errors and also set the nonblocking.
 * @ingroup API
 * @param handle A pointer to a device handle to be opened.
 * @returns -1 on failure, 0 on success.
 */
int initalize_hid(hid_device* handle){
	int res;
	// Initialize the HIDAPI library and check if there was a problem setting locale
	res = hid_init();
	if(res != 0){
		wprintf(L"Failed setting locale while initializing HIDAPI library!\n");
		return -1;
	}
	// Try to open a handle to the device
	handle = hid_open(0x1038, 0x1290, NULL);
	if(hid_error(handle) != NULL){ // Check if we failed something?
		wprintf(hid_error(handle));
		return -1;
	}
	// We need to check also that the handle ain't NULL since apaprently that is a chance if the
	// user runs this program without a appropiate udev rule installed or not running this as sudo
	if(handle == NULL){
		wprintf(L"Failed to open device handle! Are sure you are running this as root or have a appropiate udev rule installed?\n");
		return -1;
	}
	hid_set_nonblocking(handle, 0); // This call shouldn't fail since it only returns 0 unless it SEGFAULT
	if(hid_error(handle) != NULL){ // But I think its best we check anyway
		wprintf(hid_error(handle));
		return -1;
	}
	return 0;
}

void exit_hid(hid_device* handle){
	hid_close(handle);
	if(hid_error(handle) != NULL){
		wprintf(hid_error(handle));
	}
	hid_exit();
}

/** @brief Sends a Feature report to the device and returns the answer.
 * 
 * Sends a Set_Report to the device in question and reads the data
 * it gets back from the device.
 * @ingroup API
 * @param returned_data A pointer to a pre-allocated array that can hold the reponse from the device. (Usually 32 bytes)
 * @param bytes A pointer to a array where it contains the commands to be sent
 * @param handle A device handle to the opened hid device.
 * @returns -1 if something failed, 0 if succeeded.
 */
int get_report(unsigned char *returned_data, unsigned char *bytes, hid_device* handle){
	unsigned char transmission_data[33] = {0};
	transmission_data[0] = 0x00; // We don't use a report ID
	if(sizeof(bytes) >= 30){
		wprintf(L"get_report() is called with more than 30 bytes of commands, that's not allowed!\n");
		return -1;
	}
	for(int i=0; i<sizeof(bytes); i++){
		// Iterate over and assign bytes to the command that will be sent over to the device.
		transmission_data[i+1] = bytes[i];
	}
	// Send the feature report to the device and check if we failed it
	hid_send_feature_report(handle, transmission_data, 33);
	if(hid_error(handle) != NULL){
		// Something happened, print and return with -1
		wprintf(hid_error(handle));
		return -1;
	}

	hid_read(handle, returned_data, 33); // Read the control in from the device and put it into the returned array.
	if(hid_error(handle) != NULL){
		wprintf(hid_error(handle));
		return -1;
	}
	return 0;
}

void print_vendor_data(){
	int res;
	wchar_t wstr[MAX_STR];
	hid_device* handle;

	// Initialize the hidapi library
	if(initalize_hid(handle))
		return;
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	wprintf(L"Manufacturer String: %s\n", wstr);
	res = hid_get_product_string(handle, wstr, MAX_STR);
	wprintf(L"Product String: %s\n", wstr);
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	wprintf(L"Serial Number String: (%d) %s\n", wstr[0], wstr);
	exit_hid(handle);
}

void draw_pixels(){
	int res;
	hid_device* handle;
	if(initalize_hid(handle))
		return;
	unsigned char data[1025] = {0};
	data[0] = 0x00; // Report type = 0;
	data[1] = 0xD2; // "Draw on screen" function
	drawpix(0, 0, 1, &data[2]);
	drawpix(127, 0, 1, &data[2]);
	drawpix(0, 39, 1, &data[2]);
	drawpix(127, 39, 1, &data[2]);
	res = hid_send_feature_report(handle, data, 1025);
	hid_close(handle);
	res = hid_exit();
}

/** @brief Prints battery information collected from the base station and if the headset is connected or not.
 */
void print_battery(){
	hid_device* handle;
	unsigned char received_data[32];
	unsigned char command_data[2] = {0};
	//get_report(received_data, command_data, handle);
	if(initalize_hid(handle))
		return;

	// Set the command bytes to be sent
	command_data[0] = 0x40; // Charge level on the battery in the headset
	command_data[1] = 0xAA; // Query for data
	if(get_report(received_data, command_data, handle)) // Send and receive data
		return;
	wprintf(L"Charge level in headset: %d/4\n", received_data[0]); // Print how much charge we have left

	// Set the next command bytes to be sent
	command_data[0] = 0x42; // Charge level on the battery in the charger
	command_data[1] = 0xAA; // Query for data
	if(get_report(received_data, command_data, handle)) // Send and receive data
		return;
	wprintf(L"Charge level in charger: %d/4\n", received_data[0]); // Print how charged the battery in the charger is

	// Set the next command bytes to be sent
	command_data[0] = 0x41; // Headset connected and some other data
	command_data[1] = 0xAA; // Query for data
	if(get_report(received_data, command_data, handle)) // Send and receive data
		return;
	wprintf(L"Headset connected: %s\n", (received_data[0] & BITMASK_CONNECTED ? "Yes" : "No"));
	exit_hid(handle); // Close hid and exit hid.
}

void print_version(){
	hid_device* handle;
	if(initalize_hid(handle)) // Initialize the HIDAPI library, try to open the file and set nonblocking.
		return; // If error occurs, just return straight away to main()

	unsigned char received_data[32]; // Create a 32 byte array that we will feed the data into when we call the get_report function.
	unsigned char command_data[2] = {0};

	command_data[0] = 0x10; // Get base station firmware versions and some more bytes
	command_data[1] = 0xAA; // Query for data.
	if(get_report(received_data, command_data, handle))
		return;
	wprintf(L"MCU Version: %d.%d.00\n", received_data[1], received_data[0]);
	wprintf(L"TX Version: %d.%d.00\n", received_data[3], received_data[2]);

	exit_hid(handle); // Close the connection to the hid device and exit HIDAPI library.
}

void print_usage(){
	printf("Arguments:\n");
	printf("\td\tTries to draw 4 pixels on the OLED display\n");
	printf("\tb\tTries to get the battery level of the headset if it's currently powered on\n");
	printf("\tp\tPrint various information about the Arctis Pro Wireless headset that is normal API usage\n");
	printf("\tv\tTries to retrieve the version data of the base station\n");
}

int main(int argc, char* argv[]){
	if(argc == 1){
		//printf("Arguments:\n\td\tTries to draw 4 pixels on the OLED display\n\tb\tTries to get the battery level of the headset if it's currently powered on\n\tp\tPrint various information about the Arctis wireless pro basestation that is normal API usage.\n");
		print_usage();
		return EXIT_SUCCESS;
	}else{
		if(strcmp(argv[1], "b") == 0){
			print_battery();
		}else if(strcmp(argv[1], "d") == 0){
			draw_pixels();
		}else if(strcmp(argv[1], "p") == 0){
			print_vendor_data();
		}else if(strcmp(argv[1], "v") == 0){
			print_version();
		}
	}
	return EXIT_SUCCESS;
}
