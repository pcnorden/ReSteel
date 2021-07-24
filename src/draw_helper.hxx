#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <atomic>
#include <iostream>
#include <vector>
#include <algorithm>

// TODO: Implement, well, everything here that I would want for drawing
// without having to write each pixel by hand, so implementing line, box
// and some other methods that I might need.

class draw_helper{
	private:
	static std::vector<unsigned char> drawbuf;
	//static unsigned char drawbuf[1025];
	void draw_pixel(int x, int y, int on);
	public:
	void init();
	void get_framebuffer(unsigned char * buffer);
};