/********************************************************************
	Created:	2015/09/18   15:32:39
	Filename: 	GameConsole2.1\Tetris\Tetris.cpp

	Author:		SlavaGrasiuk
	
	purpose:	Tetris game
*********************************************************************/

#include <avr/pgmspace.h>
#include <PlatformDrv.h>
#include <avr/io.h>
//#include "logo.h"

#define MENU_BG_COLOR		static_cast<color8>(0x92)		//Gray
#define MENU_SELECT_COLOR	static_cast<color8>(0b11100000)	//Red
#define MENU_TEXT_COLOR		static_cast<color8>(0x00)		//Black

const char	MenuTite[]			PROGMEM = "Главное меню";
const char	ButStartText[]		PROGMEM = "1. Старт";
const char  ButRecordsText[]	PROGMEM = "2. Рекорды";
const char	ButSettingsText[]	PROGMEM = "3. Настройки";

const char lines[] PROGMEM = "LINES:";
const char score[] PROGMEM = "SCORE:";
const char next[]  PROGMEM = "NEXT:";
const char fmt[] = "%05d";

enum MenuRet {
	start = 1,
	records,
	settings
};

MenuRet Menu();
void Game();
void Settings();
void ShowRecords();

void RTOSMain() {
	LCD::SetColorMode(mode_8);
    while(1) {
        switch(Menu()) {
			case start:
				Game();
				break;
				
			case settings:
				Settings();
				break;
				
			case records:
				ShowRecords();
		}
    }
}

void Game() {
	register coord x = 0, y = 0;
	LCD::FillRect(x, y, LCD::Width(), LCD::Height(), (color8)0b00011100);
	WaitKeySignal();
}

void Settings() {
	register coord x = 0, y = 0;
	LCD::FillRect(x, y, LCD::Width(), LCD::Height(), (color8)0b00000011);
	WaitKeySignal();
}

void ShowRecords() {
	register coord x = 0, y = 0;
	LCD::FillRect(x, y, LCD::Width(), LCD::Height(), (color8)0b11100011);
	WaitKeySignal();
}

void MenuRedraw(const unsigned char CurSel);

MenuRet Menu() {
	register coord x = 0, y = 0;
	unsigned char select = 1;
	
	LCD::FillRect(x, y, LCD::Width(), LCD::Height(), MENU_BG_COLOR);
	y = 20; x = 18;
	LCD::PutStr(y, MenuTite, LCD::font8x14, MENU_TEXT_COLOR, MENU_BG_COLOR);
	MenuRedraw(select);
	
	while(1) {
		switch(WaitKeySignal()) {
			case But1_press:
				select = 1;
				MenuRedraw(select);
				break;
				
			case But2_press:
				select = 2;
				MenuRedraw(select);
				break;
				
			case But3_press:
				select = 3;
				MenuRedraw(select);
				break;
			
			case ButEnt_release:
			case ButEnc_release:
				return (MenuRet)select;
				
			case But1_release:
				return start;
			case But2_release:
				return records;
			case But3_release:
				return settings;
				
			case Enc_Down:
				if( !(--select) ) {
					select = 3;
				}
				MenuRedraw(select);
				break;
				
			case Enc_Up:
				if ( ++select>3 ) {
					select = 1;
				}
				MenuRedraw(select);

			default:
				break;
		}
	}
}

void MenuRedraw(const unsigned char CurSel) {
	register coord x = 15, y = 44;
	
	switch(CurSel) {
		case 1:
			LCD::PutStr(x, y, MenuItem1, LCD::font8x14, MENU_SELECT_COLOR, MENU_BG_COLOR);
			y += LCD::font8x14.char_h + 10;
			LCD::PutStr(x, y, MenuItem2, LCD::font8x14, MENU_TEXT_COLOR, MENU_BG_COLOR);
			y += LCD::font8x14.char_h + 10;
			LCD::PutStr(x, y, MenuItem3, LCD::font8x14, MENU_TEXT_COLOR, MENU_BG_COLOR);
			break;
			
		case 2:
			LCD::PutStr(x, y, MenuItem1, LCD::font8x14, MENU_TEXT_COLOR, MENU_BG_COLOR);
			y += LCD::font8x14.char_h + 10;
			LCD::PutStr(x, y, MenuItem2, LCD::font8x14, MENU_SELECT_COLOR, MENU_BG_COLOR);
			y += LCD::font8x14.char_h + 10;
			LCD::PutStr(x, y, MenuItem3, LCD::font8x14, MENU_TEXT_COLOR, MENU_BG_COLOR);
			break;
			
		case 3:
			LCD::PutStr(x, y, MenuItem1, LCD::font8x14, MENU_TEXT_COLOR, MENU_BG_COLOR);
			y += LCD::font8x14.char_h + 10;
			LCD::PutStr(x, y, MenuItem2, LCD::font8x14, MENU_TEXT_COLOR, MENU_BG_COLOR);
			y += LCD::font8x14.char_h + 10;
			LCD::PutStr(x, y, MenuItem3, LCD::font8x14, MENU_SELECT_COLOR, MENU_BG_COLOR);
	}
}