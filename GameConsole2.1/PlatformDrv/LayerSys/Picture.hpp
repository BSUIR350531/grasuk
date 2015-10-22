/********************************************************************
	Created:	2015/10/16   0:20:28
	Filename: 	GameConsole2.1\PlatformDrv\LayerSys\Picture.h

	Author:		SlavaGrasiuk
	
	purpose:	Picture class
*********************************************************************/


#ifndef __PICTURE_H__
#define __PICTURE_H__

#include "LayerSys.hpp"

class Picture: public SimpleGraphObj {
//variables
public:
protected:
private:

//functions
public:
	Picture();
	~Picture();
protected:
private:
	Picture( const Picture &c );
	Picture& operator=( const Picture &c );

}; //Picture

#endif //__PICTURE_H__
