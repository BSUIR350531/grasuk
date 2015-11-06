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
	static void print(const coord x, const coord y, const char * const txt, const LCD::font &fnt, const color8 col, const color8 bgcol) {
		LCD::PutStr(x, y, txt, fnt, col, bgcol);
	}
};

struct TextInRAM {
	static void print(const coord x, const coord y, const char * const txt, const LCD::font &fnt, const color8 col, const color8 bgcol) {
		LCD::PutStrRAM(x, y, txt, fnt, col, bgcol);
	}
};


#endif /* POLICY_H_ */