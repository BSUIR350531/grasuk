/********************************************************************
	Created:	2015/11/06   13:50:57
	Filename: 	G:\GameConsole2.1\Tetris\TetrisBase.cpp

	Author:		SlavaGrasiuk
	
	purpose:	Base operations for tetris objects
*********************************************************************/


#include "TetrisBase.hpp"

coord TetrisBase::GetScreenCoordX(const unsigned char block_x) {
	return x + 3 + (block_x << 3);
}

coord TetrisBase::GetScreenCoordY(const unsigned char block_y) {
	return y + 3 + (block_y << 3);
}

void TetrisBase::DrawBlock(const coord x, const coord y, const color8 col) {
	LCD::FillRect(x, y, 7, 7, col);
}
