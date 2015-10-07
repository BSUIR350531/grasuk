/********************************************************************
	Created:	2015/08/04  18:21:54
	Filename: 	GameConsole2.1\PlatformDrv\PlatformDrv.cpp

	Author:		SlavaGrasiuk
	
	purpose:	Definition of os processes and keyboard driver
*********************************************************************/

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "PlatformDrv.h"

TEncProc EncProc;
TKeybProc KeybProc;
TUserProc UserProc;

//Канал для передачи сигналов клавиатуры, ёмкость - 64 сигнала
OS::channel<signal, 64> key_ev_qu;

//Флаг нажатия клавиши для подсветки дисплея
OS::TEventFlag BLFlag;

static const signal EncTransitTable[4][4] PROGMEM = {
	{none, Enc_Up, Enc_Down, none},
	
	{Enc_Down, none, none, Enc_Up},
	
	{Enc_Up, none, none, Enc_Down},
	
	{none, Enc_Down, Enc_Up, none}
};

static const unsigned char MKeyMasks[4] PROGMEM = {
	_BV(MKEY_X1), _BV(MKEY_X2), _BV(MKEY_X3), _BV(MKEY_X4)
};

static const unsigned char MKeyMasks_Y[3] PROGMEM = {
	_BV(MKEY_Y1), _BV(MKEY_Y2), _BV(MKEY_Y3)
};

static const signal MKeySignalsRelease[3][4] PROGMEM = {
	{But1_release, But4_release, But7_release, ButDel_release},
	
	{But2_release, But5_release, But8_release, But0_release},
	
	{But3_release, But6_release, But9_release, ButEnt_release}
};

static const signal MKeySignalsPress[3][4] PROGMEM = {
	{But1_press, But4_press, But7_press, ButDel_press},
	
	{But2_press, But5_press, But8_press, But0_press},
	
	{But3_press, But6_press, But9_press, ButEnt_press}
};

int main(void) {
	ACSR |= _BV(ACD);		//Отключаем аналоговый компаратор для экономии энергии
	MCUCR |= _BV(SE);		//Настраиваем спящий режим. Выбираем режим idle - отключение тактирования CPU и flash
	TIMER2_OC_REG = 0xF9;	//Значение сравнения для системного таймера
	TIMER2_CS_REG = _BV(WGM21)|_BV(CS21)|_BV(CS20);		//Режим CTC и деление входной частоты на 32
	TIMER2_IE_REG |= _BV(OCIE2);		//Разрешаем прерывание по совпадению системного таймера. интервал срабатывания - 0.0005 сек.
	OS::run();		//Запуск операционной системы
	return 0;		//Сюда никогда не вернёмся
}

signal GetKeySignal() {
	signal ret;
	if(!key_ev_qu.get_count()) {
		return none;
	}
	key_ev_qu.pop(ret);
	return ret;
}

signal WaitKeySignal() {
	signal ret;
	key_ev_qu.pop(ret);
	return ret;
}

template<> void TEncProc::exec() {
	unsigned char EncState = 0, div4 = 0;
	signal res;
	register unsigned char EncNewSignal;
	
	while(1) {
		//Извлекаем биты, к которым подключен энкодер и помещаем в младшие разряды по порядку
		asm volatile(	
		"in  __tmp_reg__, %[port]"  "\n\t"
		"mov %0, __zero_reg__"		"\n\t"
		"bst __tmp_reg__, %[enc_a]"	"\n\t"
		"bld %0, 1"					"\n\t"
		"bst __tmp_reg__, %[enc_b]"	"\n\t"
		"bld %0, 0"					"\n\t"
		: "=r" (EncNewSignal)
		:[port] "I" (_SFR_IO_ADDR(ENC_PIN)), [enc_a] "I" (ENC_A), [enc_b] "I" (ENC_B)
		);
		//То же самое на C
		//EncNewSignal = ((ENC_PIN_REG&(1<<ENC_B))>>ENC_B)|((ENC_PIN_REG&(1<<ENC_A))>>(ENC_A-1));
		
		//КА, обрабатывающий данные от энкодера.
		//Выбираем выходной сигнал по таблице.
		res = (signal)pgm_read_byte(&(EncTransitTable[EncNewSignal][EncState]));
		if(res!=none) {
			EncState = EncNewSignal;		//Переходим в новое состояние.
			if(++div4==4) {				//Сохраняем только каждое четвёртое событие.
				div4 = 0;
				if (!BLFlag.is_signaled()) {
					BLFlag.signal();	//Сигналим подсветке дисплея
				}
				key_ev_qu.push(res);
			}
		}
		
		sleep(2);
	}
}

template<> void TKeybProc::exec() {
	unsigned char Old=_BV(ENC_BUT);
	unsigned char mkey_read_old[3] = {_BV(MKEY_X1)|_BV(MKEY_X2)|_BV(MKEY_X3)|_BV(MKEY_X4), _BV(MKEY_X1)|_BV(MKEY_X2)|_BV(MKEY_X3)|_BV(MKEY_X4), _BV(MKEY_X1)|_BV(MKEY_X2)|_BV(MKEY_X3)|_BV(MKEY_X4) };

	while(1) {
		//Если предыдущее состояние порта было нулевым, а текущее - единичное,
		//то произошло отпускание кнопки
		if( (!(Old & _BV(ENC_BUT))) && (ENC_PIN & _BV(ENC_BUT)) ) {
			if (!BLFlag.is_signaled()) {
				BLFlag.signal();
			}
			key_ev_qu.push(ButEnc_release);
			Old = ENC_PIN;
		}
		//Если наоборот - нажатие
		if( (Old & _BV(ENC_BUT)) && (!(ENC_PIN & _BV(ENC_BUT))) ) {
			if (!BLFlag.is_signaled()) {
				BLFlag.signal();
			}
			key_ev_qu.push(ButEnc_press);
			Old = ENC_PIN;
		}
	
		//Для каждого столбца матричной клавиатуры
		for(register unsigned char mYi = 0; mYi<3; mYi++) {
			MKEY_PORT &= ~pgm_read_byte(&MKeyMasks_Y[mYi]);
			//И для каждой клавиши на столбце выполняем такую же проверку
			for(register unsigned char mXi = 0; mXi<4; mXi++) {
				//Перепад с низкого уровня на высокий соответствует отпусканию
				if( (!(mkey_read_old[mYi] & pgm_read_byte(&MKeyMasks[mXi]))) && (MKEY_PIN & pgm_read_byte(&MKeyMasks[mXi])) ) {	
					if (!BLFlag.is_signaled()) {
						BLFlag.signal();
					}
					key_ev_qu.push((signal)pgm_read_byte(&MKeySignalsRelease[mYi][mXi]));
					break;
				}
				if( (mkey_read_old[mYi] & pgm_read_byte(&MKeyMasks[mXi])) && (!(MKEY_PIN & pgm_read_byte(&MKeyMasks[mXi]))) ) {
					if (!BLFlag.is_signaled()) {
						BLFlag.signal();
					}
					key_ev_qu.push((signal)pgm_read_byte(&MKeySignalsPress[mYi][mXi]));
					break;
				}
			}
			mkey_read_old[mYi] = MKEY_PIN;
			MKEY_PORT |= pgm_read_byte(&MKeyMasks_Y[mYi]);
		}
	
		sleep(33);
	}
}

template<> void TUserProc::exec() {
	RTOSMain();	
}

void KeyInit() {
	ENC_PORT |= (1<<ENC_A)|(1<<ENC_B)|_BV(ENC_BUT);
	ENC_DDR &= ~(_BV(ENC_A)|_BV(ENC_B)|_BV(ENC_BUT));

	MKEY_DDR &= ~(_BV(MKEY_X1)|_BV(MKEY_X2)|_BV(MKEY_X3)|_BV(MKEY_X4));		//Строки - входы
	MKEY_DDR |= _BV(MKEY_Y1)|_BV(MKEY_Y2)|_BV(MKEY_Y3);						//Столбцы - выходы
	MKEY_PORT |= _BV(MKEY_Y1)|_BV(MKEY_Y2)|_BV(MKEY_Y3);						//Столбцы - в 1
	MKEY_PORT |= _BV(MKEY_X1)|_BV(MKEY_X2)|_BV(MKEY_X3)|_BV(MKEY_X4);		//Подтягивающие резисторы для строк
}