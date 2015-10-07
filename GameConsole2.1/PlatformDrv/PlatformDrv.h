/********************************************************************
	Created:	2015/08/15 23:57:32
	Filename: 	GameConsole2.1\PlatformDrv\PlatformDrv.h

	Author:		SlavaGrasiuk
	
	purpose:	Definition of os processes and keyboard driver
*********************************************************************/


#ifndef PLATFORMDRV_H_
#define PLATFORMDRV_H_

#include "LayerSys/LayerSys.hpp"
#include "LayerSys/button.hpp"
#include "LCD/LcdDrv.hpp"
#include "LCD/fonts.hpp"
#include <scmRTOS.h>
#include "KeyDrv.h"

//Назначаем типам процессов короткие имена
typedef OS::process<OS::pr0, 40> TEncProc;
typedef OS::process<OS::pr1, 50> TKeybProc;
typedef OS::process<OS::pr2, 45> TBLProc;
typedef OS::process<OS::pr3, 50> TSelCtrlProc;
typedef OS::process<OS::pr4, 256> TUserProc;

//Объявления исполняемых функций процессов
namespace OS {
	template<> void TEncProc::exec();
	template<> void TKeybProc::exec();
	template<> void TUserProc::exec();
	template<> void TBLProc::exec();
	template<> void TSelCtrlProc::exec();
}

extern OS::TEventFlag BLFlag;

//главная функция пользовательского процесса
extern void RTOSMain();

//Возвращает сигнал клавиатуры без ожидания
//Если сигналов нет - возвращает сигнал none
signal GetKeySignal();

//Возвращает сигнал клавиатуры
//Если сигналов нет - ставит вызвавший процесс на ожидание до прихода сигнала
signal WaitKeySignal();

//Инициализация клавиатуры
//Вызывается автоматически до входа в main()
void KeyInit() __attribute__((naked)) __attribute__((section(".init8")));

#endif /* PLATFORMDRV_H_ */