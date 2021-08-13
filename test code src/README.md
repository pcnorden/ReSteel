# Getting dev code to compile

> Notes: This is instructions for Ubuntu 20.04 LTS, where in my case is using TUXEDO_OS from tuxedo computers, so maybe some more packages than those listed here needs to be installed.
> This is also instructions for linux, have no idea about how to compile on Windows or Mac.


* Open a terminal window, non-sudo.
* `git clone https://github.com/pcnorden/ReSteel`
* `sudo apt install build-essentials libhidapi-dev`
* `cd "ReSteel/test code src"`
* `make`

This should output a file that you should be able to run called `draw_on_oled`, but be warned that you need to run the program as sudo due to hidapi library otherwise segfaulting!

Also, the program will list operations that you can run when you just run the program without arguments.
