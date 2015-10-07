/********************************************************************
	Created:	2015/09/29   21:38:10
	Filename: 	GameConsole2.1\PlatformDrv\LayerSys\button.hpp

	Author:		SlavaGrasiuk
	
	purpose:	Button class
*********************************************************************/

#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "LayerSys.hpp"
#include "../LCD/LcdDrv.hpp"
#include "../LCD/fonts.hpp"

#define BUTTON_COLOR	(color8)0xDB

class button: public SelectableGraphObj {
	coord x_pos, y_pos;
	size w_val, h_val;
	void (*handler)();
	char *text;
	LCD::font *fnt;
	bool HaveFocus;
	
public:
	void redraw();
	void Select() {}
	void Deselect() {}
	void SignalPush() {}
	void SignalRelease() {}
	
	button(const coord x, const coord y, const size w, const size h);
	size width() const { return w_val; }
	size height() const { return h_val; }
	void SetText(const char * const string, const LCD::font &TextFnt);
	void SetHandler(void (*HandlerPtr)()) { handler = HandlerPtr; }
		
private:
	void DrawText();
		
}; //button

#endif //__BUTTON_H__
