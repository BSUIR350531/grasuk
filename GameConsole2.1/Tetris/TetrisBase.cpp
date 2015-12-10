/********************************************************************
	Created:	2015/11/06   13:50:57
	Filename: 	GameConsole2.1\Tetris\TetrisBase.cpp

	Author:		SlavaGrasiuk
	
	purpose:	Base operations for tetris objects
*********************************************************************/


#include "TetrisBase.hpp"
#include <avr/pgmspace.h>

//Шаблон заливки блока. 1 - цвет фигуры, 0 - цвет фона
static const unsigned char block_pat[7] PROGMEM = {
	0b11111110,
	0b10000010,
	0b10111010,
	0b10111010,
	0b10111010,
	0b10000010,
	0b11111110
};

//4 ориентации, 7 фигурок, 4 блока на фигурку, 2 координаты на блок.
//Координаты блоков заданы относительно первого. Координаты первого блока по умолчанию 0, 0 и не указаны.
static const signed char figure_vectros[4][7][3][2] PROGMEM = {
	//			O							L							T								S							Z							I							J
	{{{1, 0}, {1, 1}, {0, 1}}, {{-1, 1},  {-1, 0}, {1, 0} }, {{-1, 0}, {0, 1},  {1, 0}}, {{1, 0},   {0, 1},  {-1, 1}}, {{-1, 0}, {0, 1}, {1, 1}}, {{-1, 0}, {1, 0}, {2, 0}}, {{-1, 0},  {1, 0},  {1, 1}} },//0
	{{{1, 0}, {1, 1}, {0, 1}}, {{-1, -1}, {0, -1}, {0, 1} }, {{0, -1}, {-1, 0}, {0, 1}}, {{-1, -1}, {-1, 0}, {0, 1} }, {{1, -1}, {1, 0}, {0, 1}}, {{0, -1}, {0, 1}, {0, 2}}, {{0, -1},  {0, 1},  {-1, 1}}},//90
	{{{1, 0}, {1, 1}, {0, 1}}, {{-1, 0},  {1, 0},  {1, -1}}, {{-1, 0}, {0, -1}, {1, 0}}, {{1, 0},   {0, 1},  {-1, 1}}, {{-1, 0}, {0, 1}, {1, 1}}, {{-1, 0}, {1, 0}, {2, 0}}, {{-1, -1}, {-1, 0}, {1, 0}} },//180
	{{{1, 0}, {1, 1}, {0, 1}}, {{0, -1},  {0, 1},  {1, 1} }, {{0, -1}, {1, 0},  {0, 1}}, {{-1, -1}, {-1, 0}, {0, 1} }, {{1, -1}, {1, 0}, {0, 1}}, {{0, -1}, {0, 1}, {0, 2}}, {{0, -1},  {1, -1}, {0, 1}} } //270
};

//Цвета фигурок
static const color8 figure_colors[7] PROGMEM = {
//   O	   L	 T	   S	 Z	   I	 J
	0xDC, 0xF4, 0xA2, 0x30, 0xE1, 0x90, 0x4B
//Жёлтый, оранжевый, фиолетовый, салатовый, тёмно-розовый, бежевый, ярко-синий
};

coord TetrisBase::GetScreenCoordX(const unsigned char block_x) const {
	return x + 3 + (block_x << 3);
}

coord TetrisBase::GetScreenCoordY(const unsigned char block_y, const unsigned char s) const {
	return y + 3 + s + (block_y << 3);
}

void TetrisBase::DrawBlock(const coord x, const coord y, const color8 col) const {
//		LCD::FillRect(x, y, 7, 7, col);
	LCD::SetWindow(x, y, 7, 7);
	LCD::SetDatMode();
	for(register unsigned char i = 0; i<7; i++) {
		for(register unsigned char mask = 0b10000000; mask > 1; mask>>=1) {
			if(pgm_read_byte(&block_pat[i]) & mask) {
				LCD::send8(col);
			} else {
				LCD::send8(GAME_BG_COLOR);
			}
		}
	}
}

void TetrisBase::GetFigure(signed char buf[3][2], const unsigned char figure, const unsigned char figure_orient) const {
	for(register unsigned char i = 0; i < 3; i++) {
		buf[i][0] = pgm_read_byte(&figure_vectros[figure_orient][figure][i][0]);
		buf[i][1] = pgm_read_byte(&figure_vectros[figure_orient][figure][i][1]);
	}
}

color8 TetrisBase::GetColor(const unsigned char figure) const {
	return pgm_read_byte(&figure_colors[figure]);
}