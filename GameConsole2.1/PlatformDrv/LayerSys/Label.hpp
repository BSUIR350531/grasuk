/********************************************************************
	Created:	2015/10/16   0:22:09
	Filename: 	GameConsole2.1\PlatformDrv\LayerSys\Label.hpp

	Author:		SlavaGrasiuk
	
	purpose:	Text label class
*********************************************************************/


#ifndef __LABEL_H__
#define __LABEL_H__

#include "LayerSys.hpp"

class Label: public SimpleGraphObj {
//variables
public:
protected:
private:

//functions
public:
	Label();
	~Label();
protected:
private:
	Label( const Label &c );
	Label& operator=( const Label &c );

}; //Label

#endif //__LABEL_H__
