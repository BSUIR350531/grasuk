/********************************************************************
	Created:	2015/11/06   12:31:06
	Filename: 	GameConsole2.1\Tetris\FigInd.cpp

	Author:		SlavaGrasiuk
	
	purpose:	Tetris figure indicator
*********************************************************************/


#include "FigInd.hpp"

FigInd::FigInd(const coord x_pos, const coord y_pos): TetrisBase(x_pos, y_pos) {
	for(register unsigned char i = 0; i<4; i++) {
		for(register unsigned char j = 0; j<4; j++) {
			ind[i][j] = BOARD_EMPTY_BLOCK_COLOR;
		}
	}
	redraw();
}

void FigInd::redraw() {
	LCD::Rect(x, y, 37, 37, BOARD_BORDER_COLOR);
	for(register unsigned char i = 0; i<4; i++) {
		for(register unsigned char j = 0; j<4; j++) {
			DrawBlock(GetScreenCoordX(j), GetScreenCoordY(i), ind[i][j]);
		}
	}
}
