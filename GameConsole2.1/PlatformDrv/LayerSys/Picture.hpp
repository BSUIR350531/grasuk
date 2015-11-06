/********************************************************************
	Created:	2015/10/16   0:20:28
	Filename: 	GameConsole2.1\PlatformDrv\LayerSys\Picture.h

	Author:		SlavaGrasiuk
	
	purpose:	Picture class
*********************************************************************/


#ifndef __PICTURE_H__
#define __PICTURE_H__

#include "LayerSys.hpp"
#include "../LCD/LcdDrv.hpp"

struct image{
	size w;
	size h;
	color16 *img;
};

class picture: public SimpleGraphObj {
	coord x, y;
	image *bitmap;
public:
	picture(const coord x_pos, const coord y_pos, const image &img);
	void redraw();
	void SetBitMap(const image &img);
}; //Picture

#endif //__PICTURE_H__
