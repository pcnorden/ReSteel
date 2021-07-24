This file contains notes about the USB capture that I captured and will be expanded upon as I go along.

First, there is so much `GET DESCRIPTOR` that it's quite confusing what data is beginning where, but hopefully I'll get to the bottom of what is happening.

Things that stand out in the capture:

|Frame Number|Time in Capture|Protocol|Endpoint|Direction|Notes|
|---|---|---|---|---|---|
|156|3.407830|USBHID|0x00|Computer to base station| In the data sent from the computer to the device, "PROFILE 1" is written in plain text, which tells me that the computer is asking the base station about the sound profile called "PROFILE 1" that I have appearing on my base station.|

General notes about communication protocol:

* Sending `0x40 0xAA` to the base station with what wireshark calls `SET_REPORT` request sends back the battery percentage in 25% increments. (Example reference in the package capture is packet no. 174 and respons being packet no. 175). Huge thanks to [@Spirit532](spirit) for discovering this! Also a note on this behaviour: the base station needs to communicate some before it actually gives any data to that response, so right now it's basically just reverse-engineering what is being said before so I can figure out how to make the base station much more happy to communicate.

* [@Spirit532](spirit) also dropped knowledge in [issue #1](#1) about how to write to the OLED screen in the issue, which is just awesome and from some headscratching from me (bit shifting I struggle with) I'll just leave spirit's bit-shifting code be alone since I really don't understand what is going on without digging for days at the answer.

What I've uncovered so far about the protocol:

To query about the battery in the headset, send a feature report to the basestation with either `0x40 0xAA` or `0x42 0xAA`. Then, you immediately read the control input as the basestation will send back 32 bytes of data. Some proof of concept code so far using [hidapi](hidapi)
```c++
#include <hidapi/hidapi.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
	int res;
	hid_device* handle;
	res = hid_init();
	handle = hid_open(0x1038, 0x1290, NULL);
	hid_set_nonblocking(handle, 0);
	unsigned char data[33] = {0};
	data[0] = 0x00; // Report ID, which we don't use.
	data[1] = 0x40; // Battery in the headset
	data[2] = 0xAA; // Request information
	res = hid_send_feature_report(handle, data, 33);
	unsigned char received_data[33];
	res = hid_read(handle, recevied_data, 33);
	wprintf(L"Headset battery level: %d\n", received_data[0]);
	data[1] = 0x42; // Battery in the charging slot
	res = hid_send_feature_report(handle, data, 33);
	res = hid_read(handle, received_data, 33);
	wprintf(L"Battery in charging slot: %d\n", received_data[0]);
	hid_close(handle);
	res = hid_exit();
	return EXIT_SUCCESS;
}
```

Note about the battery percentage in the headset if you request it: While the headset is connected wirelessly to the base station, we get the correct percentage, but if we power off the headset, we will just get the old value returned. However, the battery in the charging slot, the values goes to 0 when you remove it from the slot.

Due to the USB capture that I made with wireshark setting the second byte of the feature report to `0xAA` a lot when it wants to get information, I believe that the first byte of the feature report is some variable that you can access and the second byte maybe telling the base station if you are setting the value, or just getting it.

[spirit]: https://github.com/Spirit532
[#1]: https://github.com/pcnorden/ReSteel/issues/1
[hidapi]: https://github.com/libusb/hidapi