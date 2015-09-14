/********************************************************************
	Created:	2015/09/05   23:53:52
	Filename: 	GameConsole2.1\test\test.cpp

	Author:		SlavaGrasiuk
	
	purpose:	Platform driver test program
*********************************************************************/

#include <avr/io.h>
#include <PlatformDrv.h>
#include <avr/pgmspace.h>

const char hello[] PROGMEM = "Hello, world!";
const char helloCyr[] PROGMEM = "Привет, мир!";

void redraw(size a) {
	register coord x, /*x2,*/ y/*, y2*/;
	register size w, h;
	register color8 color, bgcolor;
	
	x = y = 0;
	w = LCD::Width();
	h = LCD::Height();
	color = 0x92;
	LCD::FillRect(x, y, w, h, color);
	
	x = y = 25;
	w = LCD::Width() - 50;
	h = LCD::Height() - 50;
	color = 0x7B;
	LCD::Rect(x, y, w, h, color);
	
	x = y = 35;
	w = LCD::Width() - 70;
	h = LCD::Height() - 70;
	color = 0x6C;
	LCD::Rect(x, y, w, h, color);
	
/*	x = y = a;
	x2 = LCD::Width() - 1;
	y2 = LCD::Height() - 1;
	color = 0b11011000;
	LCD::Line(x, y, x2, y2, color);
	
	x = LCD::Width() - a;
	y = x2 = a;
	y2 = LCD::Height() - a;
	color = 0b11000010;
	LCD::Line(x2, y2, x, y, color);
	
	x = LCD::Width()/2;
	y = LCD::Height()/2;
	w = ( LCD::Width() < LCD::Height() ? LCD::Width() : LCD::Height() ) / 2 - 10;
	color = 0b11111100;
	LCD::Circle(x, y, w, color);*/
	
	x = LCD::Width()/2 - 12*8/2;
	y = LCD::Height()/2 - 14/2 - 10;
	color = 0b11000000;
	bgcolor = 0x92;
	LCD::PutStr(x, y, helloCyr, LCD::font8x14, color, bgcolor);
	
	x = LCD::Width()/2 - 12*8/2;
	y = LCD::Height()/2 - 14/2 - 34;
	color = 0b00000011;
	LCD::PutStr(x, y, helloCyr, LCD::font8x14, color);
	
	x = LCD::Width()/2 - 13*6/2;
	y = LCD::Height()/2 - 8/2 + 10;
	color = 0b11000000;
	bgcolor = 0x92;
	LCD::PutStr(x, y, hello, LCD::font6x8, color, bgcolor);
	
	x = LCD::Width()/2 - 13*6/2;
	y = LCD::Height()/2 - 8/2 + 28;
	color = 0b00000011;
	LCD::PutStr(x, y, hello, LCD::font6x8, color);
	
	x = y = 0;
	color = 0b11100000;
	LCD::FillRect(x, y, a, a, color);
	
	x = LCD::Width() - a;
	y = 0;
	color = 0b00011100;
	LCD::FillRect(x, y, a, a , color);
	
	x = 0;
	y = LCD::Height() - a;
	color = 0b00000011;
	LCD::FillRect(x, y, a, a , color);
}

void RTOSMain() {
	size a = 2;
	
	LCD::SetColorMode(mode_8);
	redraw(a);
	
	while(1) {
		switch(WaitKeySignal()) {
			case Enc_Up:
				if(a<20) {
					a++;
					redraw(a);
				}
				break;
				
			case Enc_Down:
				if(a>2) {
					a--;
					redraw(a);
				}
				break;
			
			case ButEnc_release:
				switch(LCD::Orient()) {
					case or_hrz:
						LCD::SetOrient(or_vrt);
						break;
					case or_vrt:
						LCD::SetOrient(or_hrz);
						break;
				}
				redraw(a);
			
			default:
				break;
		}
	}
}