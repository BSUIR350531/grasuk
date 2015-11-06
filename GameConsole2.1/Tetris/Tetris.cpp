/********************************************************************
	Created:	2015/09/18   15:32:39
	Filename: 	GameConsole2.1\Tetris\Tetris.cpp

	Author:		SlavaGrasiuk
	
	purpose:	Tetris game
*********************************************************************/

#include <avr/pgmspace.h>
#include <PlatformDrv.h>
#include "TetrisBoard.hpp"
#include "logo.h"
#include <stdio.h>
#include "FigInd.hpp"

#define MENU_BG_COLOR		static_cast<color8>(0xDB)		//Серый
#define GAME_BG_COLOR		static_cast<color8>(0x1B)		//Светло-голубой

const char	ButStartText[]		PROGMEM = "Старт";
const char  ButRecordsText[]	PROGMEM = "Рекорды";
const char	ButSettingsText[]	PROGMEM = "Настройки";
const char	PauseText[]			PROGMEM = "Пауза";
const char	YesText[]			PROGMEM = "Да";
const char	NoText[]			PROGMEM = "Нет";
const char	ExitText[]			PROGMEM = "Выйти?";

const char lines[] PROGMEM = "LINES:";
const char score[] PROGMEM = "SCORE:";
const char next[]  PROGMEM = "NEXT:";
const char fmt[] = "%06d";

void Game();
void Settings();
void ShowRecords();
void pause();

void RTOSMain() {
	LCD::SetColorMode(mode_8);
	
	layer menu_lay(MENU_BG_COLOR);
	
	picture tetris_logo(0, 0, logo);
	button start_but(10, 67, LCD::Width() - 20, 40);
	button records_but(15, 117, LCD::Width() - 30, 23);
	button settings_but(15, 145, LCD::Width() - 30, 23);
	
	start_but.SetText(ButStartText, LCD::font8x14);
	start_but.SetHandler(Game);
	
	records_but.SetText(ButRecordsText, LCD::font8x14);
	records_but.SetHandler(ShowRecords);
	
	settings_but.SetText(ButSettingsText, LCD::font8x14);
	settings_but.SetHandler(Settings);
	
    while(1) {
		GUIMainLoopW();
    }
}

void Game() {
	unsigned int cur_score = 0, lines_num = 0;
	char lines_num_buf[7], cur_score_buf[7];
	bool game_over = false;
	
	layer game_lay(GAME_BG_COLOR);
	
	label<TextInFlash> lines_lab(LCD::Width() - 40, 3);
	label<TextInFlash> score_lab(LCD::Width() - 40, 25);
	
	lines_lab.SetText(lines, LCD::font6x8, 0xFF, GAME_BG_COLOR);
	score_lab.SetText(score, LCD::font6x8, 0xFF, GAME_BG_COLOR);
	
	label<TextInRAM> lines_num_lab(LCD::Width() - 41, 14);
	label<TextInRAM> cur_score_lab(LCD::Width() - 41, 36);
	
	sprintf(lines_num_buf, fmt, lines_num);
	lines_num_lab.SetText(lines_num_buf, LCD::font6x8, 0xFF, GAME_BG_COLOR);
	sprintf(cur_score_buf, fmt, cur_score);
	cur_score_lab.SetText(cur_score_buf, LCD::font6x8, 0xFF, GAME_BG_COLOR);
	
	label<TextInFlash> next_lab(LCD::Width() - 38, 68);
	next_lab.SetText(next, LCD::font6x8, 0xFF, GAME_BG_COLOR);
	
	TetrisBoard game_board(1, 1);
	
	FigInd next_fig_ind(90, 80);
	
	while(!game_over) {
		switch(WaitKeySignal()) {
			case ButDel_release:
				game_over = true;
				break;
			
			case But0_release:
				pause();
				
			default:
				break;
		}
	}
}

void Settings() {
	layer settings_lay(MENU_BG_COLOR);
	WaitKeySignal();
}

void ShowRecords() {
	layer records_lay(MENU_BG_COLOR);
	WaitKeySignal();
}

void pause() {
	layer pause_lay;
	
	LCD::FillRect(20, (LCD::Height() >> 1) - 15, LCD::Width() - 40, 30, static_cast<color8>(0b00000011));
	
	label<TextInFlash> pause_lab((LCD::Width() >> 1) - 20, (LCD::Height() >> 1) - 7);
	pause_lab.SetText(PauseText, LCD::font8x14, 0xFF, 0b00000011);
	
	while(1) {
		if(WaitKeySignal() == But0_release) {
			break;
		}
	}
}

bool 