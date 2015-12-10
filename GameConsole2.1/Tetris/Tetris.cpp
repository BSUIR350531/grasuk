/********************************************************************
	Created:	2015/09/18   15:32:39
	Filename: 	GameConsole2.1\Tetris\Tetris.cpp

	Author:		SlavaGrasiuk
	
	purpose:	Tetris game
*********************************************************************/

#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <PlatformDrv.h>
#include "TetrisBoard.hpp"
#include "logo.h"
#include "FigInd.hpp"
#include <stdlib.h>

#define MENU_BG_COLOR		static_cast<color8>(0xDB)		//Серый

const char	ButStartText[]		PROGMEM = "Старт";
const char  ButLoadText[]		PROGMEM = "Загрузить";
const char	ButSettingsText[]	PROGMEM = "Настройки";
const char	PauseText[]			PROGMEM = "Пауза";
const char	YesText[]			PROGMEM = "Да";
const char	NoText[]			PROGMEM = "Нет";
const char	ExitText[]			PROGMEM = "Выйти?";
const char  OverWriteText[]		PROGMEM = "Перезаписать?";
const char	DeleteSave[]		PROGMEM = "Очистить?";
const char  LoadQ[]				PROGMEM = "Загрузить?";

const char high[] PROGMEM = "HIGH:";
const char score[] PROGMEM = "SCORE:";
const char next[]  PROGMEM = "NEXT:";

unsigned int EEMEM HighScore = 0;		//Рекорд, логично, хранится в eeprom
unsigned char EEMEM HaveSavedGame = 0;	//Есть ли сохранённая игра

unsigned int EEMEM ee_cur_score = 0;
unsigned char EEMEM ee_cur_figure = 0;
unsigned char EEMEM ee_next_figure = 0; 

unsigned char EEMEM BacklightTimeoutEnabled = 0xDA;
unsigned char EEMEM Brigitness = 180;
unsigned char EEMEM BacklightTimeout = 30;

void Game(bool needLoad);
void Start_NoLoad();
void Start_Load();
void Settings();
void pause();
bool YesNoDialog(const char * const text);
void GetTextScore(char *str, unsigned int num);

button *BtnLoad;

void RTOSMain() {
	LCD::SetColorMode(mode_8);
	
	layer menu_lay(MENU_BG_COLOR);
	
	picture tetris_logo(0, 0, logo);
	button start_but(10, 67, LCD::Width() - 20, 40);
	button load_but(15, 117, LCD::Width() - 30, 23);
	button settings_but(15, 145, LCD::Width() - 30, 23);
	
	start_but.SetText(ButStartText, LCD::font8x14);
	start_but.SetHandler(Start_NoLoad);
	
	if(eeprom_read_byte(&HaveSavedGame) != 0xDA) {
		load_but.disable();
	}
	load_but.SetText(ButLoadText, LCD::font8x14);
	load_but.SetHandler(Start_Load);
	BtnLoad = &load_but;
	
	settings_but.disable();	// Временно
	settings_but.SetText(ButSettingsText, LCD::font8x14);
	settings_but.SetHandler(Settings);
	
    while(1) {                 
		GUIMainLoopW();
    }
}

void Start_NoLoad() {
	Game(false);
}

void Start_Load() {
	Game(true);
}

void Game(bool needLoad) {
	unsigned int cur_score = 0, high_score = eeprom_read_word(&HighScore);
	char high_score_buf[] = "000000", cur_score_buf[] = "000000";
	unsigned char cur_figure, next_figure, cur_lines;
	timeout_t remain, time = 1000;		//0,0005 * 1000 = 0.5 сек - задержка при обычном спуске вниз. Задаётся в квантах, квант - 0.0005 сек.
	bool fastDown = false, wasLoaded;
	
	srand(TCNT0);		//Используем счётчик таймера управления подсветкой и несогласованность тактового генератора с действиями человека как неплохой источник энтропии
	
	layer game_lay(GAME_BG_COLOR);
	
	label<TextInFlash> high_lab(LCD::Width() - 40, 25);
	label<TextInFlash> score_lab(LCD::Width() - 40, 3);
	
	label<TextInRAM> high_score_lab(LCD::Width() - 41, 36);
	label<TextInRAM> cur_score_lab(LCD::Width() - 41, 14);
	
	label<TextInFlash> next_lab(LCD::Width() - 38, 68);
	
	TetrisBoard game_board(1, 1);
	
	FigInd next_fig_ind(90, 80);
	
	high_lab.SetText(high, LCD::font6x8, 0xFF, GAME_BG_COLOR);
	score_lab.SetText(score, LCD::font6x8, 0xFF, GAME_BG_COLOR);
	next_lab.SetText(next, LCD::font6x8, 0xFF, GAME_BG_COLOR);
	
	GetTextScore(high_score_buf, high_score);
	high_score_lab.SetText(high_score_buf, LCD::font6x8, 0xFF, GAME_BG_COLOR);
	
	if(needLoad) {
		game_board.Load();
		game_board.redraw();
		next_figure = eeprom_read_byte(&ee_next_figure);
		next_fig_ind.SetFigure(next_figure);
		next_fig_ind.redraw();
		cur_figure = eeprom_read_byte(&ee_cur_figure);
		cur_score = eeprom_read_word(&ee_cur_score);
		wasLoaded = true;
	} else {
		wasLoaded = false;
		next_figure = rand() % 7;
	}
	
	GetTextScore(cur_score_buf, cur_score);
	cur_score_lab.SetText(cur_score_buf, LCD::font6x8, 0xFF, GAME_BG_COLOR);
	
	while(1) {
		if(!wasLoaded) {
			cur_figure = next_figure;	//Получаем номер новой фигурки
			next_figure = rand() % 7;	//Генерируем номер следующей фигурки
			next_fig_ind.SetFigure(next_figure);	//Загружаем следующую фигурку в индикатор
			if(game_board.insert(cur_figure)) {		//Добавяем на игровую доску новую фигурку
				break;	//Если нельзя добавить - конец игры
			}
		}
		wasLoaded = false;
		fastDown = false;		//Не нужен бытрый спуск для новой фигурки
		time = 1000;
		
		while(1) {		//Во время сдвига вниз фигурку можно сдвигать влево/вправо и поворачивать
			while(1) {		//Цикл для ожидания спуска на одну клетку
				switch(TimeKeySignal(time, &remain)) {
					case But1_release:
						if(YesNoDialog(ExitText)) {
							return;
						}
						fastDown = false;	//На всякий случай, лучше после закрытия диалогового окна ещё раз нажать вниз
						break;
						
					case But2_release:
						if(eeprom_read_byte(&HaveSavedGame) == 0xDA) {
							if(YesNoDialog(OverWriteText)) {
								game_board.Save();
								eeprom_write_word(&ee_cur_score, cur_score);
								eeprom_write_byte(&ee_cur_figure, cur_figure);
								eeprom_write_byte(&ee_next_figure, next_figure);
							}
						} else {
							game_board.Save();
							eeprom_write_byte(&HaveSavedGame, 0xDA);	//Помечаем, что есть сохранение
							eeprom_write_word(&ee_cur_score, cur_score);
							eeprom_write_byte(&ee_cur_figure, cur_figure);
							eeprom_write_byte(&ee_next_figure, next_figure);
							BtnLoad->enable(false);
						}
						break;
				
					case But3_release:
						pause();
						fastDown = false;
						break;
			
					case Enc_Up:
					case But4_press:
						game_board.MoveLeft();
						break;
				
					case Enc_Down:
					case But6_press:
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
						break;
						
					case ButDel_release:
						if(eeprom_read_byte(&HaveSavedGame) && YesNoDialog(DeleteSave)) {
							eeprom_write_byte(&HaveSavedGame, 0);
							BtnLoad->disable(false);
						}
						break;
						
					default:
						break;
				}
				if(remain > 2) {
					remain -= 2;
					time = remain;		//Если нажатие произошло прежде, чем прошло 0.5 сек, остаток времени будет в remain. Возьмём на 2 кванта меньше, примерно столько рисуется фигурка
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
				game_board.StepDown();	//Повторяем сдвиг фигурки вниз до тех пор, пока	она не упадёт
			} else {
				break;
			}
		}		
		
		cur_lines = game_board.FindAndDeleteLines();
		cur_score += 1 + cur_lines * 5;	//5 очков за каждый заполненный ряд, 1 очко за поставленную фигурку
		if(cur_score > high_score) {
			high_score = cur_score;
			eeprom_write_word(&HighScore, high_score);
			GetTextScore(high_score_buf, high_score);
			high_score_lab.SetText(high_score_buf, LCD::font6x8, 0xFF, GAME_BG_COLOR);
		}
		GetTextScore(cur_score_buf, cur_score);
		cur_score_lab.SetText(cur_score_buf, LCD::font6x8, 0xFF, GAME_BG_COLOR);
	}
	
	game_board.GameOverEffect();
}

void Settings() {
	layer settings_lay(MENU_BG_COLOR);
	//Это меню пока недоступно
	WaitKeySignal();
}

void GetTextScore(char *str, unsigned int num) {
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

//----------------------	Позже это переедет в stddiag	-------------------------------

bool ret;
volatile bool NotClick;
void yes_hndl();
void no_hndl();

bool YesNoDialog(const char * const text) {
	layer dialog_lay;
	
	NotClick = true;
	
	LCD::FillRect(10, (LCD::Height() >> 1) - 30, LCD::Width() - 20, 60, MENU_BG_COLOR);
	
	button no_but(72, (LCD::Height() >> 1), 40, 23);	//Фокус ввода устанавливается на первой созданной кнопке
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