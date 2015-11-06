/********************************************************************
	Created:	2015/10/16   0:22:09
	Filename: 	GameConsole2.1\PlatformDrv\LayerSys\Label.hpp

	Author:		SlavaGrasiuk
	
	purpose:	Text label class
*********************************************************************/


#ifndef __LABEL_H__
#define __LABEL_H__

#include "LayerSys.hpp"
#include "policy.hpp"

template<class TextStoragePolicy>
class label: public SimpleGraphObj, public TextStoragePolicy {
	coord x, y;
	char *text;
	LCD::font *fnt;
	color8 text_col, text_bg_col;
	
public:
	label(const coord x_pos, const coord y_pos): x(x_pos), y(y_pos), text(NULL), fnt(NULL) {}
	void redraw();
	void SetText(const char * const txt, const LCD::font &text_fnt, const color8 col, const color8 bgcol);
}; //Label

template<class TextStoragePolicy>
void label<TextStoragePolicy>::redraw() {
	if(text) {
		TextStoragePolicy::print(x, y, text, *fnt, text_col, text_bg_col);
	}
}

template<class TextStoragePolicy>
void label<TextStoragePolicy>::SetText(const char * const txt, const LCD::font &text_fnt, const color8 col, const color8 bgcol) {
	text = const_cast<char*>(txt);
	fnt = const_cast<LCD::font*>(&text_fnt);
	text_col = col;
	text_bg_col = bgcol;
	redraw();
}

#endif //__LABEL_H__
