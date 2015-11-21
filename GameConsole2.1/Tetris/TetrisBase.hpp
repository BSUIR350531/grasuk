/********************************************************************
	Created:	2015/11/06   13:51:23
	Filename: 	GameConsole2.1\Tetris\TetrisBase.hpp

	Author:		SlavaGrasiuk
	
	purpose:	Base operations for tetris objects
*********************************************************************/


#ifndef __TETRISBASE_H__
#define __TETRISBASE_H__

#include <PlatformDrv.h>

#define GAME_BG_COLOR		static_cast<color8>(0x1B)		//Светло-голубой
#define BOARD_EMPTY_BLOCK_COLOR					0x1A	//Слегка темнее фона игры

class TetrisBase {
protected:
	coord x, y;
	
	//Создавать объекты этого класса нельзя
	TetrisBase(const coord x_pos, const coord y_pos): x(x_pos), y(y_pos) {}
	
	//Рисует блок, принимая экранные координаты
	void DrawBlock(const coord x, const coord y, const color8 col) const;

	//Возвращают экранные координаты
	coord GetScreenCoordX(const unsigned char block_x) const;
	coord GetScreenCoordY(const unsigned char block_y, const unsigned char s = 0) const;
	
	//Загружает в buf фигуру figure в ориентации figure_orient
	void GetFigure(signed char buf[3][2], const unsigned char figure, const unsigned char figure_orient) const;
	
	//Возвращает цвет фигуры figure
	color8 GetColor(const unsigned char figure) const;
}; //TetrisBase

#endif //__TETRISBASE_H__
