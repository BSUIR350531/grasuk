/********************************************************************
	Created:	2015/10/16   0:04:58
	Filename: 	GameConsole2.1\PlatformDrv\LayerSys\GraphObj.cpp

	Author:		SlavaGrasiuk
	
	purpose:	Base class for all graphic objects
*********************************************************************/

#include "LayerSys.hpp"

GraphObj::GraphObj(const bool type): Enabled(true) {
	CanReciveFocus = type;
	if( !LayerStack.IsEmpty() ) {	//Стек не пуст, создан хотя бы один слой.
		layerPtr TopLayer = LayerStack.pick();
		if(CanReciveFocus) {
			TopLayer->PutSelObj(this);
		} else {
			TopLayer->PutObj(this);
		}
	}
}
