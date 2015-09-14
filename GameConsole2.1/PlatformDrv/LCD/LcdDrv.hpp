/********************************************************************
	Created:	2015/09/03   16:43:26
	Filename: 	GameConsole2.1\PlatformDrv\LCD\LcdDrv.hpp

	Author:		SlavaGrasiuk
	
	purpose:	LS020 dispaly low-level driver header
*********************************************************************/

#ifndef LCDDRV_H_
#define LCDDRV_H_

/************************************************************************/
/****						Подключение LCD							 ****/
/************************************************************************/
#define LCD_PORT_NAME	B
#define	LCD_RS			1
#define	LCD_RESET		2
#define	LCD_CS			4
/************************************************************************/

#include "../KeyDrv.h"		//Для PRECOMP()

#define LCD_PORT				PRECOMP(LCD_PORT_NAME, PORT)
#define LCD_DDR					PRECOMP(LCD_PORT_NAME, DDR)

typedef unsigned char coord;		//Тип для экранных координат
typedef unsigned char size;
typedef unsigned char color8;		//8-ми битный цвет 3-3-2 R-G-B
typedef unsigned int color16;		//16-ти битный цвет 5-6-5 R-G-B
	
enum orient {
	or_vrt,
	or_hrz
};
	
enum ColorMode {
	mode_8,
	mode_16
};

namespace LCD {
	void send8(const unsigned char data);	//Отправляет 8 бит данных дисплею
	void send16(const unsigned int data);	//Отправляет 16 бит данных дисплею
	
	//Инициализация дисплея. Выполняется автоматически до входа в main()
	void init() __attribute__((naked)) __attribute__((section(".init7")));
	
	void SetCmdMode();			//Установливает режим отправки комманд
	void SetDatMode();			//Устанавливает режим отправки данных
	
	size Width();
	size Height();
	orient Orient();
	ColorMode ClrMode();
	
	void SetBLTimeout(const unsigned char time);
	void SetBLLevel(const unsigned char level);
	void DisableBLTimeout();
	void EnableBLTimeout();
	
	void SetColorMode(const ColorMode clm);		//Установливает цветовой режим	
	void SetOrient(const orient orientation);	//Установливает ориентацию дисплея
	
	/*Устанавливает окно вывода с координатами верхнего левого угла x, y и размерами w x h*/
	void SetWindow(const coord x, const coord y, const size w = 1, const size h = 1);
	
	/*Рисует НЕзакрашенный прямоугольник 
		с координатами левого верхнего угла  x, y и размерами w x h цветом color*/
	void Rect(const coord x, const coord y, const size w , const size h, const color8 color);
	void Rect(const coord x, const coord y, const size w , const size h, const color16 color);
	
	/*Рисует закрашенный прямоугольник
		с координатами левого верхнего угла  x, y и размерами w x h цветом color*/
	void FillRect(const coord x, const coord y, const size w , const size h, const color8 color);
	void FillRect(const coord x, const coord y, const size w , const size h, const color16 color);
	
	/*закрашивает пиксель с координатами x, y цветом color*/
	void SetPixel(const coord x, const coord y, const color8 color);
	void SetPixel(const coord x, const coord y, const color16 color);
	
	/*Отрезок с координатами концов x1, y1 и x2, y2*/
	void Line(coord x1, coord y1, const coord x2, const coord y2, const color8 color);
	void Line(coord x1, coord y1, const coord x2, const coord y2, const color16 color);
	
	/*Окружность с координатами xcenter, ycenter и радиусом rad*/
	void Circle(const coord xcenter, const coord ycenter, const size rad, const color8 color);
	void Circle(const coord xcenter, const coord ycenter, const size rad, const color16 color);
}

#endif /* LCDDRV_H_ */