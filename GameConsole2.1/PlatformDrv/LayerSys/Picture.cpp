/********************************************************************
	Created:	2015/10/16   0:21:11
	Filename: 	GameConsole2.1\PlatformDrv\LayerSys\Picture.cpp

	Author:		SlavaGrasiuk
	
	purpose:	Picture class
*********************************************************************/


#include "Picture.hpp"
#include "../LCD/LcdDrv.hpp"
#include <avr/pgmspace.h>

// default constructor
picture::picture(const coord x_pos, const coord y_pos, const image &img):
x(x_pos), y(y_pos), bitmap(&const_cast<image&>(img)) {
	redraw();
} //Picture

void picture::redraw() {
	LCD::SetWindow(x, y, bitmap->w, bitmap->h);
	LCD::SetColorMode(mode_16);
	LCD::SetDatMode();
	for(register unsigned int n = 0; n < static_cast<unsigned int>(bitmap->h * bitmap->w); n++) {
		LCD::send16(pgm_read_word(&bitmap->img[n]));
	}
	LCD::SetColorMode(mode_8);
}

void picture::SetBitMap(const image &img) {
	bitmap = &const_cast<image&>(img);
	redraw();
}
