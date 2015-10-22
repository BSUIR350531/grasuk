/********************************************************************
	Created:	2015/09/05   23:53:52
	Filename: 	GameConsole2.1\test\test.cpp

	Author:		SlavaGrasiuk
	
	purpose:	Platform driver test program
*********************************************************************/

#include <avr/io.h>
#include <PlatformDrv.h>
#include <avr/pgmspace.h>

const char but1_txt[] PROGMEM = "Кнопка 1";
const char but2_txt[] PROGMEM = "Button 2";
const char back_text[] PROGMEM = "Назад";
bool NotExit = true;

void RetHndl() {
	NotExit = false;
}

void func1() {
	layer tst_lay2((color8)0b00000011);		//Создаём слой, залитый синим цветом
	
	button BackBtn(30, LCD::Height() - 35, LCD::Width() - 60, 25);
	
	BackBtn.SetText(back_text, LCD::font8x14);
	BackBtn.SetHandler(RetHndl);
	
	while(NotExit) {
		GUIMainLoopW();
	}
	
	NotExit = true;
}		//При выходе из функции деструктор tst_lay2 перерисует содержимое tst_lay. 

void func2() {
	layer tst_lay3((color8)0b11100000);
	
	button BackBtn(30, LCD::Height() - 35, LCD::Width() - 60, 25);
		
	BackBtn.SetText(back_text, LCD::font8x14);
	BackBtn.SetHandler(RetHndl);
		
	while(NotExit) {
		GUIMainLoopW();
	}
		
	NotExit = true;
}

void RTOSMain() {
	LCD::SetColorMode(mode_8);		//Переключем дисплей в 8-ми битный цвет 3-3-2 R-G-B
	
	layer tst_lay((color8)0xDB);	//Создаём слой, залитый серым цветом
	
	button btn1(20, 20, LCD::Width()-40, 30);		//Создаём на нём две кнопки
	button btn2(30, 70, LCD::Width()-60, 20);
	
	btn1.SetText(but1_txt, LCD::font8x14);
	btn2.SetText(but2_txt, LCD::font6x8);
	
	btn1.SetHandler(func1);
	btn2.SetHandler(func2);
	
	while(1) {
		GUIMainLoopW();
	}
}