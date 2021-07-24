#include "draw_helper.hxx"

void draw_helper::draw_pixel(int x, int y, int on){
	if(on)
		drawbuf[(x-1)+(y/8)*128] |= (1<<(y&7));
	else
		drawbuf[(x-1)+(y/8)*128] &= ~(1<<(y&7));
}

void draw_helper::init(){
	drawbuf.resize(1025);
	drawbuf[0] = 0x00; // Report type = 0;
	drawbuf[1] = 0xD2; // "Draw on screen" function
}

void draw_helper::get_framebuffer(unsigned char * buffer){
	std::cout << "[get_framebuffer] This is yet to be implemented!" << std::endl;
}