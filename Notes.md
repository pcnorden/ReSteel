This file contains notes about the USB capture that I captured and will be expanded upon as I go along.

First, there is so much GET DESCRIPTORS that it's quite confusing what data is beginning where, but hopefully I'll get to the bottom of what is happening.

Things that stand out in the capture:

|Frame Number|Time in capture|Protocol|Endpoint|Direction|Notes|
|---|---|---|---|---|---|
|156|3.407830|USBHID|0x00|Computer to base station|In the data sent from the computer to the device, "PROFILE 1" is written in plain text, which makes me think that it might tell the base station to select a profile or something that isn't available to see on the basestation, so it can't be a audio profile that it's selecting (I think)|
