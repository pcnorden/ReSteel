This file contains notes about the USB capture that I captured and will be expanded upon as I go along.

First, there is so much `GET DESCRIPTOR` that it's quite confusing what data is beginning where, but hopefully I'll get to the bottom of what is happening.

Things that stand out in the capture:

|Frame Number|Time in Capture|Protocol|Endpoint|Direction|Notes|
|---|---|---|---|---|---|
|156|3.407830|USBHID|0x00|Computer to base station| In the data sent from the computer to the device, "PROFILE 1" is written in plain text, which tells me that the computer is asking the base station about the sound profile called "PROFILE 1" that I have appearing on my base station.|

General notes about communication protocol:

* Sending `0x40 0xAA` to the base station with what wireshark calls `SET_REPORT` request sends back the battery percentage in 25% increments. (Example reference in the package capture is packet no. 174 and respons being packet no. 175). Huge thanks to [@Spirit532](spirit) for discovering this! Also a note on this behaviour: the base station needs to communicate some before it actually gives any data to that response, so right now it's basically just reverse-engineering what is being said before so I can figure out how to make the base station much more happy to communicate.

* [@Spirit532](spirit) also dropped knowledge in [issue #1](#1) about how to write to the OLED screen in the issue, which is just awesome and from some headscratching from me (bit shifting I struggle with) I'll just leave spirit's bit-shifting code be alone since I really don't understand what is going on without digging for days at the answer.

[spirit]: https://github.com/Spirit532
[#1]: https://github.com/pcnorden/ReSteel/issues/1