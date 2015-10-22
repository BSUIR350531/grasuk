/********************************************************************
	Created:	2015/10/16   16:35:20
	Filename: 	GameConsole2.1\PlatformDrv\LayerSys\policy.h

	Author:		SlavaGrasiuk
	
	purpose:	policy library for graphic objects
*********************************************************************/


#ifndef POLICY_H_
#define POLICY_H_


#include "../LCD/fonts.hpp"

struct TextInFlash {
	static void print();
};

struct TextInRAM {
	static void print();
};


#endif /* POLICY_H_ */