/********************************************************************
	Created:	2015/11/06   13:51:23
	Filename: 	GameConsole2.1\Tetris\TetrisBase.hpp

	Author:		SlavaGrasiuk
	
	purpose:	Base operations for tetris objects
*********************************************************************/


#ifndef __TETRISBASE_H__
#define __TETRISBASE_H__

#include <PlatformDrv.h>

#define GAME_BG_COLOR		static_cast<color8>(0x1B)		//������-�������
#define BOARD_EMPTY_BLOCK_COLOR					0x1A	//������ ������ ���� ����

class TetrisBase {
protected:
	coord x, y;
	
	//��������� ������� ����� ������ ������
	TetrisBase(const coord x_pos, const coord y_pos): x(x_pos), y(y_pos) {}
	
	//������ ����, �������� �������� ����������
	void DrawBlock(const coord x, const coord y, const color8 col) const;

	//���������� �������� ����������
	coord GetScreenCoordX(const unsigned char block_x) const;
	coord GetScreenCoordY(const unsigned char block_y, const unsigned char s = 0) const;
	
	//��������� � buf ������ figure � ���������� figure_orient
	void GetFigure(signed char buf[3][2], const unsigned char figure, const unsigned char figure_orient) const;
	
	//���������� ���� ������ figure
	color8 GetColor(const unsigned char figure) const;
}; //TetrisBase

#endif //__TETRISBASE_H__
