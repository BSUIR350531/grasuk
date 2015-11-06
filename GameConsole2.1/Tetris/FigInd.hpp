/********************************************************************
	Created:	2015/11/06   12:30:36
	Filename: 	GameConsole2.1\Tetris\FigInd.hpp

	Author:		SlavaGrasiuk
	
	purpose:	Tetris figure indicator
*********************************************************************/

#ifndef __FIGIND_H__
#define __FIGIND_H__

#include <PlatformDrv.h>
#include "TetrisBoard.hpp"
#include "TetrisBase.hpp"

class FigInd: public SimpleGraphObj, public TetrisBase {
	color8 ind[4][4];
	
public:
	FigInd(const coord x_pos, const coord y_pos);
	void redraw();
	void SetFigure();
}; //FigInd

#endif //__FIGIND_H__
