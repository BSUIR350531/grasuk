/********************************************************************
	Created:	2015/09/05   12:50:12
	Filename: 	GameConsole2.1\PlatformDrv\LCD\LcdDrv.cpp

	Author:		SlavaGrasiuk
	
	purpose:	LS020 dispaly low-level driver
*********************************************************************/

#include "../PlatformDrv.h"
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "LcdDrv.hpp"
#include <avr/io.h>

using namespace LCD;

static const unsigned int init1[10] PROGMEM = {
	0xEF00, 0xEE04, 0x1B04, 0xFEFE, 0xFEFE, 0xEF90, 0x4A04, 0x7F3F, 0xEE04, 0x4306
};

static const unsigned int init2[20] PROGMEM = {
	0xEF90, 0x0983, 0x0800, 0x0BAF, 0x0A00, 0x0500, 0x0600, 0x0700, 0xEF00, 0xEE0C,
	0xEF90, 0x0080, 0xEFB0, 0x4902, 0xEF00, 0x7F01, 0xE181, 0xE202, 0xE276, 0xE183
};
	
static size width = 132, height = 176;		//текущая ширина и высота дисплея в пикселях
static orient CurOrient = or_vrt;
static ColorMode CurColorMode = mode_16;
static unsigned char BLTimeout = 30;		//Sec
static unsigned char BLLevel = 180;		//PWM DC

TBLProc BLProc;

template<> void TBLProc::exec() {
	register unsigned char delay = 0;
	register bool prem;
	while(1) {
		//Ждём нажатия клавиш 1 секунду
		if( !BLFlag.wait(2000) ) {
			//Если не дождались, увеличиваем таймер
			if(++delay == BLTimeout) {	//Если таймер досчитал
				prem = false;
				while(OCR0) {	//Плавно уменьшаем яркость
					OCR0--;
					if(BLFlag.wait(12)) {	//Если во время этого произощло нажатие
						prem = true;		//Запоминаем это
						break;		//И прекращаем
					}
				}
				if(prem) {			//Если нажатие произошло во время уменьшения яркости
					OCR0 = BLLevel;		//Восстанавливаем яркость
					delay = 0;		//Сбрасываем таймер
				} else {
					TCCR0 = 0;
					BLFlag.wait();		//Иначе ждем нажатия клавиши
					OCR0 = BLLevel;
					TCCR0 = (1<<WGM01)|(1<<WGM00)|(1<<COM01)|(1<<CS00);
					delay = 0;
				}
			}
		} else {
			delay = 0;	//если дождались нажатия, сбрасываем таймер
		}
	}
}

void LCD::SetBLTimeout(const unsigned char time) {
	BLTimeout = time;
}

void LCD::SetBLLevel(const unsigned char level) {
	BLLevel = level;
	OCR0 = level;
}

void LCD::DisableBLTimeout() {
	BLProc.terminate();
}

void LCD::EnableBLTimeout() {
	BLProc.start();
}

void LCD::send8(const unsigned char data) {
	SPDR=data;
	while(!(SPSR & _BV(SPIF)));
}

void LCD::send16(const unsigned int data) {
	send8(data>>8);
	send8(data);
}

void LCD::init() {
#if LCD_PORT_NAME == B
	DDRB |= _BV(LCD_RESET)|_BV(LCD_CS)|_BV(LCD_RS)|_BV(PB5)|_BV(PB7)|_BV(DDB3);
	PORTB |= _BV(LCD_RESET)|_BV(LCD_CS)|_BV(LCD_RS)|_BV(PB6);
	PORTB &= ~(_BV(LCD_CS)|_BV(PB7)|_BV(PB3)|_BV(LCD_RESET));
#else
	LCD_DDR |= _BV(LCD_RESET)|_BV(LCD_CS)|_BV(LCD_RS);
	DDRB |= _BV(PB5)|_BV(PB7)|_BV(DDB3);
	LCD_PORT |= _BV(LCD_RESET)|_BV(LCD_CS)|_BV(LCD_RS);
	PORTB |= _BV(PB6);
	LCD_PORT &= ~(_BV(LCD_CS)|_BV(LCD_RESET));
	PORTB &= ~(_BV(PB7)|_BV(PB3));
#endif

	//Настройка аппаратного SPI
	SPCR=0x50;
	SPSR=0x01;
	
	//Hard reset дисплея
	_delay_ms(5);
	LCD_PORT|=_BV(LCD_RESET);			//Reset = 1
	
	//Soft reset дисплея
	send16(0xFDFD);
	send16(0xFDFD);
	_delay_ms(68);
	
	//Инициализация дисплея. Блоки комманд через определённые интервалы
	for (register unsigned char i=0; i<10; send16(pgm_read_word(&init1[i])), i++);
	_delay_ms(7);
	for (register unsigned char i=0; i<20; send16(pgm_read_word(&init2[i])), i++);
	_delay_ms(50);
	send16(0x8001);
	_delay_ms(5);
	send16(0xEF90);
	send16(0x0000);
	
	//Настройка ШИМ для управления подсветкой дисплея
	TCCR0 = (1<<WGM01)|(1<<WGM00)|(1<<COM01)|(1<<CS00);
	OCR0 = BLLevel;
}

void LCD::SetCmdMode() {
	LCD_PORT|=_BV(LCD_RS);		//RS = 1, команды
	LCD_PORT|=_BV(LCD_CS);		//Импульс на линии CS
	LCD_PORT&=~_BV(LCD_CS);		//
}

size LCD::Width() {
	return width;
}

size LCD::Height() {
	return height;
}

orient LCD::Orient() {
	return CurOrient;
}

ColorMode LCD::ClrMode() {
	return CurColorMode;
}


void LCD::SetColorMode(const ColorMode clm) {
	SetCmdMode();
	CurColorMode = clm;
	switch(clm) {
		case mode_16:
			send16(0xE80C);
			break;
		case mode_8:
			send16(0xE800);
			break;
	}
}

void LCD::SetOrient(const orient orientation) {
	SetCmdMode();
	CurOrient = orientation;
	switch(CurOrient) {
		case or_hrz:
			send16(0x0180);
			send16(0x0504);
			width = 176;
			height = 132;
			break;
		case or_vrt:
			send16(0x0100);
			send16(0x0500);
			width = 132;
			height = 176;
			break;
	}
}

void LCD::SetWindow(const coord x, const coord y, const size w, const size h) {
	SetCmdMode();
	switch(CurOrient) {
		case or_vrt:
			send16(0x0800 + x);
			send16(0x0A00 + y);
			send16(0x0900 + (x + w - 1));
			send16(0x0B00 + (y + h - 1));
			break;
		case or_hrz:
			send16(0x0A00 + x);
			send16(0x0800 + (height - 1 - y));
			send16(0x0B00 + (x + w - 1));
			send16(0x0900 + (height - 1 - (y + h - 1)));
			break;
	}
}

void LCD::Rect(const coord x, const coord y, const size w, const size h, const color8 color) {
	FillRect(x, y, w, 1, color);
	FillRect(x, y, 1, h, color);
	FillRect(x + w - 1, y, 1, h, color);
	FillRect(x, y + h - 1, w, 1, color);
}

void LCD::Rect(const coord x, const coord y, const size w, const size h, const color16 color) {
	FillRect(x, y, w, 1, color);
	FillRect(x, y, 1, h, color);
	FillRect(x + w - 1, y, 1, h, color);
	FillRect(x, y + h - 1, w, 1, color);
}

void LCD::FillRect(const coord x, const coord y, const size w , const size h, const color8 color) {
	SetWindow(x, y, w, h);
	SetDatMode();
	for(register unsigned char i = 0; i<w; i++) {
		for (register unsigned char j = 0; j<h; send8(color), j++);
	}
}

void LCD::FillRect(const coord x, const coord y, const size w , const size h, const color16 color) {
	SetWindow(x, y, w, h);
	SetDatMode();
	for(register unsigned char i = 0; i<w; i++) {
		for (register unsigned char j = 0; j<h; send16(color), j++);
	}
}

void LCD::SetPixel(const coord x, const coord y, const color8 color) {
	SetWindow(x, y);
	SetDatMode();
	send8(color);
}

void LCD::SetPixel(const coord x, const coord y, const color16 color) {
	SetWindow(x, y);
	SetDatMode();
	send16(color);
}

void LCD::Line(coord x1, coord y1, const coord x2, const coord y2, const color8 color ) {
	int dx, dy, stepx, stepy, fraction;
	dy = y2 - y1;
	dx = x2 - x1;
	if ( dy < 0 ) {
		dy    = -dy;
		stepy = -1;
	} else {
		stepy = 1;
	}

	if ( dx < 0 ) {
		dx    = -dx;
		stepx = -1;
	} else {
		stepx = 1;
	}
	dx <<= 1;
	dy <<= 1;
	
	if ( dx > dy ) {
		fraction = dy - (dx >> 1);
		while ( x1 != x2 ) {
			if ( fraction >= 0 ) {
				y1 += stepy;
				fraction -= dx;
			}
			x1 += stepx;
			fraction += dy;
			SetPixel(x1, y1, color);
		}
	} else {
		fraction = dx - (dy >> 1);
		while ( y1 != y2 ) {
			if ( fraction >= 0 ) {
				x1 += stepx;
				fraction -= dy;
			}
			y1 += stepy;
			fraction += dx;
			SetPixel(x1,y1,color);
		}
	}
}

void LCD::Line(coord x1, coord y1, const coord x2, const coord y2, const color16 color ) {
	int dx, dy, stepx, stepy, fraction;
	dy = y2 - y1;
	dx = x2 - x1;
	if ( dy < 0 ) {
		dy    = -dy;
		stepy = -1;
	} else {
		stepy = 1;
	}

	if ( dx < 0 ) {
		dx    = -dx;
		stepx = -1;
	} else {
		stepx = 1;
	}
	dx <<= 1;
	dy <<= 1;
	
	if ( dx > dy ) {
		fraction = dy - (dx >> 1);
		while ( x1 != x2 ) {
			if ( fraction >= 0 ) {
				y1 += stepy;
				fraction -= dx;
			}
			x1 += stepx;
			fraction += dy;
			SetPixel(x1, y1, color);
		}
	} else {
		fraction = dx - (dy >> 1);
		while ( y1 != y2 ) {
			if ( fraction >= 0 ) {
				x1 += stepx;
				fraction -= dy;
			}
			y1 += stepy;
			fraction += dx;
			SetPixel(x1,y1,color);
		}
	}
}

void LCD::Circle(const coord xcenter, const coord ycenter, const size rad, const color8 color) {
	int tswitch, y1, x1 = 0;
	unsigned char d;
	d = ycenter - xcenter;
	y1 = rad;
	tswitch = 3 - 2 * rad;
	while (x1 <= y1) {
		SetPixel(xcenter + x1, ycenter + y1, color);
		SetPixel(xcenter + x1, ycenter - y1, color);
		SetPixel(xcenter - x1, ycenter + y1, color);
		SetPixel(xcenter - x1, ycenter - y1, color);
		SetPixel(ycenter + y1 - d, ycenter + x1, color);
		SetPixel(ycenter + y1 - d, ycenter - x1, color);
		SetPixel(ycenter - y1 - d, ycenter + x1, color);
		SetPixel(ycenter - y1 - d, ycenter - x1, color);
		if (tswitch < 0) {
			tswitch += (4 * x1 + 6);
		} else {
			tswitch += (4 * (x1 - y1) + 10);
			y1--;
		}
		x1++;
	}
}

void LCD::Circle(const coord xcenter, const coord ycenter, const size rad, const color16 color) {
	int tswitch, y1, x1 = 0;
	unsigned char d;
	d = ycenter - xcenter;
	y1 = rad;
	tswitch = 3 - 2 * rad;
	while (x1 <= y1) {
		SetPixel(xcenter + x1, ycenter + y1, color);
		SetPixel(xcenter + x1, ycenter - y1, color);
		SetPixel(xcenter - x1, ycenter + y1, color);
		SetPixel(xcenter - x1, ycenter - y1, color);
		SetPixel(ycenter + y1 - d, ycenter + x1, color);
		SetPixel(ycenter + y1 - d, ycenter - x1, color);
		SetPixel(ycenter - y1 - d, ycenter + x1, color);
		SetPixel(ycenter - y1 - d, ycenter - x1, color);
		if (tswitch < 0) {
			tswitch += (4 * x1 + 6);
		} else {
			tswitch += (4 * (x1 - y1) + 10);
			y1--;
		}
		x1++;
	}
}