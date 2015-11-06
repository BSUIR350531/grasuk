/********************************************************************
	Created:	2015/09/09   15:22:31
	Filename: 	GameConsole2.1\PlatformDrv\LCD\fonts.hpp

	Author:		SlavaGrasiuk
	
	purpose:	Simply font engine for lcd
*********************************************************************/


#ifndef FONTS_H_
#define FONTS_H_

#include "LcdDrv.hpp"
#include <avr/pgmspace.h>		//Для PROGMEM

#define _USE_8BIT		//Используем только 8-ми битные цвета
//#define _USE_16BIT
//#define _USE_BGL		//Использование функций, печатающих без фона

namespace LCD {
	
	typedef void (*CharDrawFunc8)(const coord x, const coord y, const char c, const color8 color, const color8 bgcolor);
	typedef void (*CharDrawFunc16)(const coord x, const coord y, const char c, const color16 color, const color16 bgcolor);
	typedef void (*CharDrawFunc8BGL)(const coord x, const coord y, const char c, const color8 color);
	typedef void (*CharDrawFunc16BGL)(const coord x, const coord y, const char c, const color16 color);
	
	struct font {
		size char_w, char_h;
	#ifdef _USE_8BIT
		CharDrawFunc8 print8;
		#ifdef _USE_BGL
			CharDrawFunc8BGL print8BGL;
		#endif //_USE_BGL
	#endif //_USE_8BIT
	#ifdef _USE_16BIT
		CharDrawFunc16 print16;
		#ifdef _USE_BGL
			CharDrawFunc16BGL print16BGL;
		#endif	//_USE_BGL
	#endif	//_USE_16BIT
	};
	
	//Печатает строку str из Flash, шрифтом fnt 8-ми битным цветом color и фоном bgcolor
	void PutStr(const coord x, const coord y, const char *str PROGMEM, const font &fnt, const color8 color, const color8 bgcolor);
	
	//Печатает строку str из Flash, шрифтом fnt 8-ми битным цветом color без фона
	void PutStr(const coord x, const coord y, const char *str PROGMEM, const font &fnt, const color8 color);
	
	//Печатает строку str из Flash, шрифтом fnt 8-ми битным цветом color и фоном bgcolor по центру экрана
	void PutStr(const coord y, const char *str PROGMEM, const font &fnt, const color8 color, const color8 bgcolor);
	
	//Печатает строку str из Flash, шрифтом fnt 8-ми битным цветом color без фона по центру экрана
	void PutStr(const coord y, const char *str PROGMEM, const font &fnt, const color8 color);

	// Те же функции для 16-ти битного цвета ------------------------------------------------------------------
	void PutStr(const coord x, const coord y, const char *str PROGMEM, const font &fnt, const color16 color, const color16 bgcolor);
	void PutStr(const coord x, const coord y, const char *str PROGMEM, const font &fnt, const color16 color);
	void PutStr(const coord y, const char *str PROGMEM, const font &fnt, const color16 color, const color16 bgcolor);
	void PutStr(const coord y, const char *str PROGMEM, const font &fnt, const color16 color);
	//---------------------------------------------------------------------------------------------------------
	
	// Те же функции для печати строки из RAM -----------------------------------------------------------------
	void PutStrRAM(const coord x, const coord y, const char *str, const font &fnt, const color8 color, const color8 bgcolor);
	void PutStrRAM(const coord x, const coord y, const char *str, const font &fnt, const color8 color);
	void PutStrRAM(const coord y, const char *str, const font &fnt, const color8 color, const color8 bgcolor);
	void PutStrRAM(const coord y, const char *str, const font &fnt, const color8 color);
	
	void PutStrRAM(const coord x, const coord y, const char *str, const font &fnt, const color16 color, const color16 bgcolor);
	void PutStrRAM(const coord x, const coord y, const char *str, const font &fnt, const color16 color);
	void PutStrRAM(const coord y, const char *str, const font &fnt, const color16 color, const color16 bgcolor);
	void PutStrRAM(const coord y, const char *str, const font &fnt, const color16 color);
	//---------------------------------------------------------------------------------------------------------
	
	//Объявления доступных шрифтов
	extern const font font8x14;
	extern const font font6x8;
}


#endif /* FONTS_H_ */