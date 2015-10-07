/********************************************************************
	Created:	2015/09/30   9:22:31
	Filename: 	GameConsole2.1\PlatformDrv\LayerSys\button.cpp

	Author:		SlavaGrasiuk
	
	purpose:	Button class
*********************************************************************/

#include <avr/pgmspace.h>
#include "button.hpp"

button::button(const coord x, const coord y, const size w, const size h):
x_pos(x), y_pos(y),
w_val(w), h_val(h),
text(NULL), HaveFocus(false) {
	register coord X = x+1, Y = y+1;
	register size W = w-2, H = h-2;
	LCD::FillRect(X, Y, W, H, BUTTON_COLOR);
	X = x; Y = y; W = w; H = 1;
	LCD::FillRect(X, Y, W, H, (color8)0xFF);
	Y = y+1; W = 1; H = h-1;
	LCD::FillRect(X, Y, W, H, (color8)0xFF);
	X = x+w-1; Y = y+1;
	LCD::FillRect(X, Y, W, H, (color8)0x92);
	X = x+1; Y = y+h-1; W = w-2; H = 1;
	LCD::FillRect(X, Y, W, H, (color8)0x92);
	X = x+w; Y = y; W = 1; H = h+1;
	LCD::FillRect(X, Y, W, H, (color8)0x49);
	X = x; Y = y+h; W = w; H = 1;
	LCD::FillRect(X, Y, W, H, (color8)0x49);
}

void button::DrawText() {
	register unsigned char i;
	register coord X, Y;
	for (i = 0; pgm_read_byte(&text[i]); i++);
	X = x_pos + (w_val>>1) - ((i*fnt->char_w)>>1);
	Y = y_pos + (h_val>>1) - (fnt->char_h>>1);
	LCD::PutStr(X, Y, text, *fnt, (color8)0x00, BUTTON_COLOR);
}

void button::redraw() {
		register coord X = x_pos+1, Y = y_pos+1;
		register size W = w_val-2, H = h_val-2;
		LCD::FillRect(X, Y, W, H, BUTTON_COLOR);
		X = x_pos; Y = y_pos; W = w_val; H = 1;
		LCD::FillRect(X, Y, W, H, (color8)0xFF);
		Y = y_pos+1; W = 1; H = h_val-1;
		LCD::FillRect(X, Y, W, H, (color8)0xFF);
		X = x_pos+w_val-1; Y = y_pos+1;
		LCD::FillRect(X, Y, W, H, (color8)0x92);
		X = x_pos+1; Y = y_pos+h_val-1; W = w_val-2; H = 1;
		LCD::FillRect(X, Y, W, H, (color8)0x92);
		X = x_pos+w_val; Y = y_pos; W = 1; H = h_val+1;
		LCD::FillRect(X, Y, W, H, (color8)0x49);
		X = x_pos; Y = y_pos+h_val; W = w_val; H = 1;
		LCD::FillRect(X, Y, W, H, (color8)0x49);
		if(text) {
			DrawText();
		}
}

void button::SetText(const char * const string, const LCD::font &TextFnt) {
	text = const_cast<char*>(string);
	fnt = const_cast<LCD::font*>(&TextFnt);
	DrawText();
}