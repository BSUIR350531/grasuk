/********************************************************************
	Created:	2015/09/09   19:43:36
	Filename: 	GameConsole2.1\PlatformDrv\LCD\fonts.cpp

	Author:		SlavaGrasiuk
	
	purpose:	Simply font engine for lcd
*********************************************************************/

#include "fonts.hpp"
#include <string.h>

using namespace LCD;

#ifdef _USE_8BIT

void LCD::PutStr(const coord x, const coord y, const char *str PROGMEM, const font &fnt, const color8 color, const color8 bgcolor) {
	register unsigned char i, x0;
	register char c;
	for(i = 0, x0 = 0, c = pgm_read_byte(&str[i++]); c; fnt.print8(x + x0, y, c, color, bgcolor), c = pgm_read_byte(&str[i++]), x0 += fnt.char_w);
}

void LCD::PutStr(const coord x, const coord y, const char *str PROGMEM, const font &fnt, const color8 color) {
	register unsigned char i, x0;
	register char c;
	for(i = 0, x0 = 0, c = pgm_read_byte(&str[i++]); c; fnt.print8BGL(x + x0, y, c, color), c = pgm_read_byte(&str[i++]), x0 += fnt.char_w);
}

void LCD::PutStr(const coord y, const char *str PROGMEM, const font &fnt, const color8 color, const color8 bgcolor) {
	register unsigned char i, x, x0;
	register char c;
	for (i = 0; pgm_read_byte(&str[i]); i++);
	x = (Width()>>1) - ((i*fnt.char_w)>>1);
	for(i = 0, x0 = 0, c = pgm_read_byte(&str[i++]); c; fnt.print8(x + x0, y, c, color, bgcolor), c = pgm_read_byte(&str[i++]), x0 += fnt.char_w);
}

void LCD::PutStr(const coord y, const char *str PROGMEM, const font &fnt, const color8 color) {
	register unsigned char i, x, x0;
	register char c;
	for (i = 0; pgm_read_byte(&str[i]); i++);
	x = (Width()>>1) - ((i*fnt.char_w)>>1);
	for(i = 0, x0 = 0, c = pgm_read_byte(&str[i++]); c; fnt.print8BGL(x + x0, y, c, color), c = pgm_read_byte(&str[i++]), x0 += fnt.char_w);
}

#endif

#ifdef _USE_16BIT

void LCD::PutStr(const coord x, const coord y, const char *str PROGMEM, const font &fnt, const color16 color) {
	register unsigned char i, x0;
	register char c;
	for(i = 0, x0 = 0, c = pgm_read_byte(&str[i++]); c; fnt.print16BGL(x + x0, y, c, color), c = pgm_read_byte(&str[i++]), x0 += fnt.char_w);
}

void LCD::PutStr(const coord x, const coord y, const char *str PROGMEM, const font &fnt, const color16 color, const color16 bgcolor) {
	register unsigned char i, x0;
	register char c;
	for(i = 0, x0 = 0, c = pgm_read_byte(&str[i++]); c; fnt.print16(x + x0, y, c, color, bgcolor), c = pgm_read_byte(&str[i++]), x0 += fnt.char_w);
}

void LCD::PutStr(const coord y, const char *str PROGMEM, const font &fnt, const color16 color, const color16 bgcolor) {
	register unsigned char i, x, x0;
	register char c;
	for (i = 0; pgm_read_byte(&str[i]); i++);
	x = (Width()>>1) - ((i*fnt.char_w)>>1);
	for(i = 0, x0 = 0, c = pgm_read_byte(&str[i++]); c; fnt.print16(x + x0, y, c, color, bgcolor), c = pgm_read_byte(&str[i++]), x0 += fnt.char_w);
}

void LCD::PutStr(const coord y, const char *str PROGMEM, const font &fnt, const color16 color) {
	register unsigned char i, x, x0;
	register char c;
	for (i = 0; pgm_read_byte(&str[i]); i++);
	x = (Width()>>1) - ((i*fnt.char_w)>>1);
	for(i = 0, x0 = 0, c = pgm_read_byte(&str[i++]); c; fnt.print16BGL(x + x0, y, c, color), c = pgm_read_byte(&str[i++]), x0 += fnt.char_w);
}

#endif