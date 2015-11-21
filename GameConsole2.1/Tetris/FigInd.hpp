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

#define FIGURE_X		1
#define FIGURE_Y		1

class FigInd: public SimpleGraphObj, public TetrisBase {
	color8 ind[4][4];
	signed char fig_vect[3][2];
	unsigned char fig_no;
	
public:
	FigInd(const coord x_pos, const coord y_pos);
	void redraw();
	void SetFigure(const unsigned char figno);

private:
	void DrawInd() const;
	void ClearBuf();
}; //FigInd

#endif //__FIGIND_H__
