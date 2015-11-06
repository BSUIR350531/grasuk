/********************************************************************
	Created:	2015/11/06   13:51:23
	Filename: 	G:\GameConsole2.1\Tetris\TetrisBase.hpp

	Author:		SlavaGrasiuk
	
	purpose:	Base operations for tetris objects
*********************************************************************/


#ifndef __TETRISBASE_H__
#define __TETRISBASE_H__

#include <PlatformDrv.h>

class TetrisBase {
protected:
	coord x, y;
	
public:
	TetrisBase(const coord x_pos, const coord y_pos): x(x_pos), y(y_pos) {}
	
protected:
	//Рисует блок, принимая экранные координаты
	void DrawBlock(const coord x, const coord y, const color8 col);

	//Возвращают экранные координаты
	coord GetScreenCoordX(const unsigned char block_x);
	coord GetScreenCoordY(const unsigned char block_y);
}; //TetrisBase

#endif //__TETRISBASE_H__
