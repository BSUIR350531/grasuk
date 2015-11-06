/********************************************************************
	Created:	2015/09/20   15:27:27
	Filename: 	GameConsole2.1\Tetris\TetrisBoard.hpp

	Author:		SlavaGrasiuk
	
	purpose:	Tetris board class
*********************************************************************/

#ifndef __TETRISBOARD_H__
#define __TETRISBOARD_H__

#include <PlatformDrv.h>
#include "TetrisBase.hpp"

#define BOARD_EMPTY_BLOCK_COLOR					0x1A	//Слегка темнее фона игры
#define BOARD_BORDER_COLOR	static_cast<color8>(0x00)	//Чёрный

class TetrisBoard: public SimpleGraphObj, public TetrisBase {
	color8 board[21][10];
	
public:
	TetrisBoard(const coord x_pos, const coord y_pos);
	void redraw();

}; //TetrisBoard

#endif //__TETRISBOARD_H__
