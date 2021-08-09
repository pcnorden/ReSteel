# ReSteel
My project working on reverse-engineering the USB protocol for the Arctis Pro Wireless headset made by SteelSeries.

The end result is hopefully a driver for Linux or some utility so I can get the charge percentage from the base station and later on include more features.

Surround sound support for this headset is not something that I will be developing since that includes reverse-engineering the software the steelseries engine installs, which is against the terms of service of the software. (Also, the really annoying interface of the SteelSeries GG engine is written in electron, which I just don't wanna touch)

This is just a project that doesn't have really anything since I'm working on capturing and just analyzing the USB data that wireshark captures between the USB connection on the base station and the computer. If you want to help, feel free to either open a pull request or issue.

This is just a side project for me, I work long hours on my feet all the time at work, so I'll put in as much effort I can muster, but it won't be a lot until I get a breakthrough and can start to write code.

# File structure

|path|summary|
|---|---|
|`Notes.md`|This is where I write what I find out about the USB protocol and generally any findings I make. This file will be updated once I find out something concrete about the protocol.|
|`test code src/`|This is where the code I produce to test my theories about how the protocol works and how to extract data. The code is a mess, but hopefully somewhat understandable.|
|`src/`|This is where code for the GUI would end up, however I struggle with wrapping my head around [imgui](https://github.com/ocornut/imgui) which is what I've chosen for the GUI to be written in since it's bloat-free with minimal dependencies. Currently it only contains some code to get CPU usage due to me (pcnorden) struggling with getting imgui to work.|
|`USB SteelSeries Pro Wireless Plugged in.pcapng`|This is a capture I took with Wireshark when I had unplugged my base station from USB, started a capture and then plugged the base station in so I could see all the packets that the software sends when it established communication. This is the main file I'm working on when reverse-engineering the protocol.|
|`audioInterfaceDump.txt`|Dump of `lsusb` command from when I was working out what interface did what since the base station shows 2 devices.|
|`InterfaceDump.txt`|The same as `audioInterfaceDump.txt`, was just for figuring out what interface did what.|
|`README.md`|This is the file that you're currently reading about the project since github shows it immediately when you open a repository underneath the files.

# Places where I would appreciate getting some help

Honestly, I would recommend looking at the milestones of this project, but I will list it here too.

* People that could test some of the code written.
* Helping setting up a imgui enviroment for compilation for both windows and linux targets.
* Reverse-engineering the protocol.