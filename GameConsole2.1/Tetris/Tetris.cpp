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
#include "FigInd.hpp"
#include <stdlib.h>

#define MENU_BG_COLOR		static_cast<color8>(0xDB)		//�����

const char	ButStartText[]		PROGMEM = "�����";
const char  ButRecordsText[]	PROGMEM = "�������";
const char	ButSettingsText[]	PROGMEM = "���������";
const char	PauseText[]			PROGMEM = "�����";
const char	YesText[]			PROGMEM = "��";
const char	NoText[]			PROGMEM = "���";
const char	ExitText[]			PROGMEM = "�����?";

const char lines[] PROGMEM = "LINES:";
const char score[] PROGMEM = "SCORE:";
const char next[]  PROGMEM = "NEXT:";

void Game();
void Settings();
void ShowRecords();
void pause();
bool YesNoDialog(const char * const text);
void GetTextScore(char *str, int num);

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
	
	srand(TCNT0);		//���������� ������� ������� ���������� ���������� ��� �������� �������� ��������
	
    while(1) {
		GUIMainLoopW();
    }
}

void Game() {
	unsigned int cur_score = 0, lines_num = 0;
	char lines_num_buf[] = "000000", cur_score_buf[] = "000000";
	unsigned char cur_figure, next_figure, cur_lines;
	timeout_t remain, time = 1000;		//0,0005 * 1000 = 0.5 ��� - �������� ��� ������� ������ ����. ������� � �������, ����� - 0.0005 ���.
	bool fastDown = false;
	
	layer game_lay(GAME_BG_COLOR);
	
	label<TextInFlash> lines_lab(LCD::Width() - 40, 3);
	label<TextInFlash> score_lab(LCD::Width() - 40, 25);
	
	label<TextInRAM> lines_num_lab(LCD::Width() - 41, 14);
	label<TextInRAM> cur_score_lab(LCD::Width() - 41, 36);
	
	label<TextInFlash> next_lab(LCD::Width() - 38, 68);
	
	TetrisBoard game_board(1, 1);
	
	FigInd next_fig_ind(90, 80);
	
	lines_lab.SetText(lines, LCD::font6x8, 0xFF, GAME_BG_COLOR);
	score_lab.SetText(score, LCD::font6x8, 0xFF, GAME_BG_COLOR);
	
	GetTextScore(lines_num_buf, lines_num);
	lines_num_lab.SetText(lines_num_buf, LCD::font6x8, 0xFF, GAME_BG_COLOR);
	GetTextScore(cur_score_buf, cur_score);
	cur_score_lab.SetText(cur_score_buf, LCD::font6x8, 0xFF, GAME_BG_COLOR);
	
	next_lab.SetText(next, LCD::font6x8, 0xFF, GAME_BG_COLOR);
	
	next_figure = rand() % 7;
	
	while(1) {
		cur_figure = next_figure;	//�������� ����� ����� �������
		next_figure = rand() % 7;	//���������� ����� ��������� �������
		next_fig_ind.SetFigure(next_figure);	//��������� ��������� ������� � ���������
		if(game_board.insert(cur_figure)) {		//�������� �� ������� ����� ����� �������
			break;	//���� ������ �������� - ����� ����
		}
		fastDown = false;		//�� ����� ������ ����� ��� ����� �������
		
		while(1) {		//�� ����� ������ ���� ������� ����� �������� �����/������ � ������������
			while(1) {		//���� ��� �������� ������ �� ���� ������
				switch(TimeKeySignal(time, &remain)) {
					case But1_release:
						if(YesNoDialog(ExitText)) {
							return;
						}
						fastDown = false;	//�� ������ ������, ����� ����� �������� ����������� ���� ��� ��� ������ ����
						break;
				
					case But3_release:
						pause();
						fastDown = false;
						break;
			
					case Enc_Up:
					case But4_release:
						game_board.MoveLeft();
						break;
				
					case Enc_Down:
					case But6_release:
						game_board.MoveRight();
						break;
				
					case ButEnc_release:
					case But5_release:
						game_board.Rotate();
						break;
					
					case But8_press:
						fastDown = true;
						remain = 100; 
						break;
				
					case But8_release:
						fastDown = false;
				
					default:
						break;
				}
				if(remain > 2) {
					remain -= 2;
					time = remain;		//���� ������� ��������� ������, ��� ������ 0.5 ���, ������� ������� ����� � remain. ������ �� 1 ����� ������, �������� ������� �������� �������
				} else {
					break;
				}
			}
			if(fastDown) {
				time = 100;
			} else {
				time = 1000;
			}
			if(game_board.CanStepDown()) {
				game_board.StepDown();	//��������� ����� ������� ���� �� ��� ���, ����	��� �� �����
			} else {
				break;
			}
		}		
		
		cur_lines = game_board.FindAndDeleteLines();
		cur_score += 1 + cur_lines * 5;	//5 ����� �� ������ ����������� ���, 1 ���� �� ������������ �������
		lines_num += cur_lines;
		GetTextScore(lines_num_buf, lines_num);
		lines_num_lab.SetText(lines_num_buf, LCD::font6x8, 0xFF, GAME_BG_COLOR);
		GetTextScore(cur_score_buf, cur_score);
		cur_score_lab.SetText(cur_score_buf, LCD::font6x8, 0xFF, GAME_BG_COLOR);
	}
		
	game_board.GameOverEffect();
}

void Settings() {
	layer settings_lay(MENU_BG_COLOR);
	//��� ���� ���� ����������
	WaitKeySignal();
}

void ShowRecords() {
	layer records_lay(MENU_BG_COLOR);
	//��� ���� ���� ����������
	WaitKeySignal();
}

void GetTextScore(char *str, int num) {
	for(register unsigned char i = 5; i > 0; i--) {
		str[i] = 0x30 + num % 10;
		num/=10;
	}
}

void pause() {
	layer pause_lay;
	
	LCD::FillRect(20, (LCD::Height() >> 1) - 15, LCD::Width() - 40, 30, static_cast<color8>(0b00000011));
	
	label<TextInFlash> pause_lab((LCD::Width() >> 1) - 20, (LCD::Height() >> 1) - 7);
	pause_lab.SetText(PauseText, LCD::font8x14, 0xFF, 0b00000011);
	
	while(1) {
		if(WaitKeySignal() == But3_release) {
			break;
		}
	}
}

//----------------------	����� ��� �������� � stddiag	-------------------------------

bool ret;
volatile bool NotClick;
void yes_hndl();
void no_hndl();

bool YesNoDialog(const char * const text) {
	layer dialog_lay;
	
	NotClick = true;
	
	LCD::FillRect(10, (LCD::Height() >> 1) - 30, LCD::Width() - 20, 60, MENU_BG_COLOR);
	
	button no_but(72, (LCD::Height() >> 1), 40, 23);	//����� ����� ��������������� �� ������ ��������� ������
	button yes_but(20, (LCD::Height() >> 1), 40, 23);
	
	yes_but.SetText(YesText, LCD::font8x14);
	yes_but.SetHandler(yes_hndl);
	no_but.SetText(NoText, LCD::font8x14);
	no_but.SetHandler(no_hndl);
	
	LCD::PutStr((LCD::Height() >> 1) - 22, text, LCD::font8x14, 0x00, MENU_BG_COLOR);
	
	while(NotClick) {
		GUIMainLoopW();
	}
	
	return ret;
}

void yes_hndl() {
	NotClick = false;
	ret = true;
}

void no_hndl() {
	NotClick = false;
	ret = false;
}