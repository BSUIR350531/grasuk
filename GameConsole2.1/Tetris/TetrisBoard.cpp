/********************************************************************
	Created:	2015/09/20   15:27:27
	Filename: 	GameConsole2.1\Tetris\TetrisBoard.cpp

	Author:		SlavaGrasiuk
	
	purpose:	Tetris board class
*********************************************************************/


#include "TetrisBoard.hpp"

TetrisBoard::TetrisBoard(const coord x_pos, const coord y_pos): TetrisBase(x_pos, y_pos) {
	for(register unsigned char i = 0; i<21; i++) {
		for(register unsigned char j = 0; j<10; j++) {
			board[i][j] = BOARD_EMPTY_BLOCK_COLOR;
		}
	}
	redraw();
}

void TetrisBoard::redraw() {
	LCD::FillRect(x, y, 1, LCD::Height() - y - 1, BOARD_BORDER_COLOR);
	LCD::FillRect(x + 1, LCD::Height() - y - 1, 84, 1, BOARD_BORDER_COLOR);
	LCD::FillRect(x + 84, y, 1, LCD::Height() - y - 1, BOARD_BORDER_COLOR);
	for(register unsigned char i = 0; i<21; i++) {
		for(register unsigned char j = 0; j<10; j++) {
			DrawBlock(GetScreenCoordX(j), GetScreenCoordY(i), board[i][j]);
		}
	}
}