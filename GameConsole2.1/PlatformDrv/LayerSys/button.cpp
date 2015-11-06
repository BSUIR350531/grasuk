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
text(NULL) {
	ButColor = 0xDB;
	redraw();
}

void button::DrawText() {
	register unsigned char i;
	register coord X, Y;
	for (i = 0; pgm_read_byte(&text[i]); i++);
	X = x_pos + (w_val>>1) - ((i*fnt->char_w)>>1);
	Y = y_pos + (h_val>>1) - (fnt->char_h>>1);
	LCD::PutStr(X, Y, text, *fnt, (color8)0x00, ButColor);
}

void button::redraw() {
	register coord X = x_pos+1, Y = y_pos+1;
	register size W = w_val-2, H = h_val-2;
	LCD::FillRect(X, Y, W, H, ButColor);
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

void button::Select() {
	register coord x = x_pos+4, y = y_pos+4;
	register size w = w_val-7, h = h_val-7;
	LCD::SetWindow(x, y, w, 1);
	LCD::SetDatMode();
	for(register unsigned char i = 0; i < w; i++) {
		LCD::send8(ButColor);
		LCD::send8(0x49);
	}
	LCD::SetWindow(x, y, 1, h);
	LCD::SetDatMode();
	for(register unsigned char i = 0; i < h; i++) {
		LCD::send8(ButColor);
		LCD::send8(0x49);
	}
	LCD::SetWindow(x + w - 1, y, 1, h);
	LCD::SetDatMode();
	for(register unsigned char i = 0; i < h; i++) {
		LCD::send8(ButColor);
		LCD::send8(0x49);
	}
	LCD::SetWindow(x, y + h - 1, w, 1);
	LCD::SetDatMode();
	for(register unsigned char i = 0; i < w; i++) {
		LCD::send8(ButColor);
		LCD::send8(0x49);
	}
	LCD::SetPixel(x, y, (color8)0x49);
	LCD::SetPixel(x+w-1, y, (color8)0x49);
	LCD::SetPixel(x, y+h-1, (color8)0x49);
	LCD::SetPixel(x+w-1, y+h-1, (color8)0x49);
	
	have_focus = true;
}

void button::Deselect() {
	register coord X = x_pos+4, Y = y_pos+4;
	register size W = w_val-7, H = h_val-7;
	LCD::Rect(X, Y, W, H, ButColor);
	have_focus = false;
	if(Pressed) {
		SignalRelease();
	}
}

void button::SignalPush() {
	register coord X, Y;
	register size W, H;
	X = x_pos; Y = y_pos; W = w_val; H = 1;
	LCD::FillRect(X, Y, W, H, (color8)0x49);
	Y = y_pos+1; W = 1; H = h_val-1;
	LCD::FillRect(X, Y, W, H, (color8)0x49);
	X = x_pos+w_val-1; Y = y_pos+1;
	LCD::FillRect(X, Y, W, H, ButColor);
	X = x_pos+1; Y = y_pos+h_val-1; W = w_val-2; H = 1;
	LCD::FillRect(X, Y, W, H, ButColor);
	X = x_pos+1; Y = y_pos+1; W = 1; H = h_val-1;
	LCD::FillRect(X, Y, W, H, (color8)0x92);
	X = x_pos+2; Y = y_pos+1; W = w_val-2; H = 1;
	LCD::FillRect(X, Y, W, H, (color8)0x92);
	X = x_pos+w_val; Y = y_pos; W = 1; H = h_val+1;
	LCD::FillRect(X, Y, W, H, (color8)0xFF);
	X = x_pos; Y = y_pos+h_val; W = w_val; H = 1;
	LCD::FillRect(X, Y, W, H, (color8)0xFF);
	Pressed = true;
}

void button::SignalRelease() {
	register coord X, Y;
	register size W, H;
	if(Pressed) {
		X = x_pos; Y = y_pos; W = w_val; H = 1;
		LCD::FillRect(X, Y, W, H, (color8)0xFF);
		Y = y_pos+1; W = 1; H = h_val-1;
		LCD::FillRect(X, Y, W, H, (color8)0xFF);
		X = x_pos+w_val-1; Y = y_pos+1;
		LCD::FillRect(X, Y, W, H, (color8)0x92);
		X = x_pos+1; Y = y_pos+h_val-1; W = w_val-2; H = 1;
		LCD::FillRect(X, Y, W, H, (color8)0x92);
		X = x_pos+1; Y = y_pos+1; W = 1; H = h_val-1;
		LCD::FillRect(X, Y, W, H, ButColor);
		X = x_pos+2; Y = y_pos+1; W = w_val-2; H = 1;
		LCD::FillRect(X, Y, W, H, ButColor);
		X = x_pos+w_val; Y = y_pos; W = 1; H = h_val+1;
		LCD::FillRect(X, Y, W, H, (color8)0x49);
		X = x_pos; Y = y_pos+h_val; W = w_val; H = 1;
		LCD::FillRect(X, Y, W, H, (color8)0x49);
		if(handler != NULL && have_focus) {
			handler();
		}
		Pressed = false;
	}
}