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

void func() {
	layer tst_lay((color8)0b00000011);
	WaitKeySignal();
}

void RTOSMain() {
	LCD::SetColorMode(mode_8);
	
	layer tst_lay((color8)0xDB);
	
	button btn1(20, 20, LCD::Width()-40, 30);
	button btn2(30, 70, LCD::Width()-60, 20);
	
	btn1.SetText(but1_txt, LCD::font8x14);
	btn2.SetText(but2_txt, LCD::font6x8);
	
	while(1) {
		WaitKeySignal();
		func();
	}
}