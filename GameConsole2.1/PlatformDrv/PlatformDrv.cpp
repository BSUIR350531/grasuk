/********************************************************************
	Created:	2015/08/04  18:21:54
	Filename: 	GameConsole2.1\PlatformDrv\PlatformDrv.cpp

	Author:		SlavaGrasiuk
	
	purpose:	Definition of os processes and keyboard driver
*********************************************************************/

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "PlatformDrv.h"

TEncProc EncProc;
TKeybProc KeybProc;
TUserProc UserProc;

//Канал для передачи сигналов клавиатуры, ёмкость - 64 сигнала
OS::channel<signal, 64> key_ev_qu;

//Флаг нажатия клавиши для подсветки дисплея
OS::TEventFlag BLFlag;

//Таблица выходов КА для обработки событий энкодера
//4 возможных состояния и 4 возможных входных сигнала
static const signal EncTransitTable[4][4] PROGMEM = {
	{none, Enc_Up, Enc_Down, none},
	
	{Enc_Down, none, none, Enc_Up},
	
	{Enc_Up, none, none, Enc_Down},
	
	{none, Enc_Down, Enc_Up, none}
};

//Таблица масок строк матричной клавиатуры
//#define _BV(x)	(1<<x) 
static const unsigned char MKeyMasks[4] PROGMEM = {
	_BV(MKEY_X1), _BV(MKEY_X2), _BV(MKEY_X3), _BV(MKEY_X4)
};

//Таблица масок столбцов матричной клавиатуры
static const unsigned char MKeyMasks_Y[3] PROGMEM = {
	_BV(MKEY_Y1), _BV(MKEY_Y2), _BV(MKEY_Y3)
};

//Таблица выходных сигналов отпускания клавиш
static const signal MKeySignalsRelease[3][4] PROGMEM = {
	{But1_release, But4_release, But7_release, ButDel_release},
	
	{But2_release, But5_release, But8_release, But0_release},
	
	{But3_release, But6_release, But9_release, ButEnt_release}
};

//Таблица выходных сигналов нажатия клавиш
static const signal MKeySignalsPress[3][4] PROGMEM = {
	{But1_press, But4_press, But7_press, ButDel_press},
	
	{But2_press, But5_press, But8_press, But0_press},
	
	{But3_press, But6_press, But9_press, ButEnt_press}
};

int main(void) {
	ACSR |= _BV(ACD);		//Отключаем аналоговый компаратор для экономии энергии
	MCUCR |= _BV(SE);		//Настраиваем спящий режим. Выбираем режим idle - отключение тактирования CPU и flash
	TIMER2_OC_REG = 0xF9;	//Значение сравнения для системного таймера. О том, как оно расчитывается - см. datasheet
	TIMER2_CS_REG = _BV(WGM21)|_BV(CS21)|_BV(CS20);		//Режим CTC и деление входной частоты на 32
	TIMER2_IE_REG |= _BV(OCIE2);		//Разрешаем прерывание по совпадению системного таймера. Интервал срабатывания - 0.0005 сек.
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
	key_ev_qu.pop(ret);		//При чтении из пустого канала вызвавший процесс ставится на ожидание до прихода данных.
	return ret;
}

template<> void TEncProc::exec() {
	signal res;
	register unsigned char EncNewSignal, EncState = 0, div4 = 0;
	
	while(1) {
		/*	По условию задачи, энкодер может быть подключен к любым разрядам порта, однако для быстрой 
		обработки поступающих от него данных с помощью конечного автомата с выбором выходных сигналов
		по таблице (вместо традиционного switch), необходимо, чтобы его контакты были подключены к 
		двум младшим разрядам порта. Тогда можно было бы обойтись простым чтением порта и наложением
		маски 0x03. 
			Приведённый ниже код выполняет требуемую операцию, не требуя при этом обязательного
		подключения энкодера к младшим разрядам. Единственное ограничение: энкодер должен быть подключён
		к любым двум разрядам ОДНОГО порта. Однако даже это ограничение легко снять, если потребуется.
		Это должно облегчить разводку платы.	*/
		
		//Извлекаем биты, к которым подключен энкодер и помещаем в младшие разряды по порядку		
		asm volatile(	
			"in  __tmp_reg__, %[port]"  "; Читаем порт, к которому подлючён энкодер во временный регистр процессора""\n\t"
			"clr %0"					"; Обнуляем регистр, в который поместим результат"							"\n\t"
			"bst __tmp_reg__, %[enc_a]"	"; Сохраняем бит, к которому подключён контакт А энкодера во флаг T"		"\n\t"
			"bld %0, 1"					"; Копируем флаг T в первый бит регистра результата"						"\n\t"
			"bst __tmp_reg__, %[enc_b]"	"; Сохраняем бит, к которому подключён контакт B энкодера во флаг T"		"\n\t"
			"bld %0, 0"					"; Копируем флаг T в нулевой бит регистра результата"						"\n\t"
			: "=r" (EncNewSignal)
			:[port] "I" (_SFR_IO_ADDR(ENC_PIN)), [enc_a] "I" (ENC_A), [enc_b] "I" (ENC_B)
		);

		//То же самое на C

		/* 	Данный код после компиляции будет занимать больше места, т.к. не будут использованы специальные
		возможности процессора AVR для работы с битами (см. AVR Instruction Set). Соответственно, он будет 
		иметь большее время выполенения. Поскольку данный процесс является самым высокоприоритетным, т.к. 
		пропуск событий энкодера может привести к тому, что энкодером невозможно будет пользоваться, этот
		процесс получает управление каждую милисекунду. При этом он не должен сильно нагружать процессор.
			Именно поэтому, используется фрагмент кода ассеблера, приведённый выше.	*/
		   
		//EncNewSignal = ((ENC_PIN_REG&(1<<ENC_B))>>ENC_B)|((ENC_PIN_REG&(1<<ENC_A))>>(ENC_A-1));
		
		//КА, обрабатывающий данные от энкодера.
		//За идею такого КА спасибо автору этой статьи:	http://habrahabr.ru/post/241941
		//Выбираем выходной сигнал из таблицы.
		res = static_cast<signal>(pgm_read_byte(&(EncTransitTable[EncNewSignal][EncState])));
		if(res!=none) {
			EncState = EncNewSignal;		//Переходим в новое состояние.

		/*	Для полноценного понимания того, как это работает, настоятельно рекомендуется ознакомиться 
		с тем, что такое датчик угла поворота, или энкодер. Вкратце: у энкодера есть два контакта, которые 
		при вращении движка поочерёдно замыкаются с общим. По тому, в какой последовательности они замыкаются
		и размыкаются, можно сделать вывод о том, в какую сторону вращается энкодер. Один контакт может 
		находится в двух состояниях: замкнут или разомкнут.
			За один щелчёк контакты успевают побывать во всех возможных состояниях. Поскольку контактов 2, и
		каждый контакт может находиться в 2 состояниях, за один щелчёк происходит 4 события. Следовательно, 
		необходимо сообщать системе только о каждом 4-ом событии.	*/

			if(++div4==4) {				//Сохраняем только каждое четвёртое событие.
				div4 = 0;
				if (!BLFlag.is_signaled()) {
					BLFlag.signal();	//Сигналим подсветке дисплея
				}
				key_ev_qu.push(res);	/*Если канал полон, процесс встанет на ожидание до появления свободного места.
										Естественно, получение и обработка данных энкодера в это время будет невозможна.
										Однако для корректной работы критична только обработка тех четырёх событий во время 
										одного щелчка, а во время их обработки процесс не может встать на ожидание.		*/
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
			MKEY_PORT &= ~pgm_read_byte(&MKeyMasks_Y[mYi]);	//Выделяем очередной столбец
			//И для каждой клавиши на столбце выполняем такую же проверку
			for(register unsigned char mXi = 0; mXi<4; mXi++) {
				//Перепад с низкого уровня на высокий соответствует отпусканию
				if( (!(mkey_read_old[mYi] & pgm_read_byte(&MKeyMasks[mXi]))) && (MKEY_PIN & pgm_read_byte(&MKeyMasks[mXi])) ) {	
					if (!BLFlag.is_signaled()) {
						BLFlag.signal();
					}
					key_ev_qu.push(static_cast<signal>(pgm_read_byte(&MKeySignalsRelease[mYi][mXi])));
					break;
				}
				if( (mkey_read_old[mYi] & pgm_read_byte(&MKeyMasks[mXi])) && (!(MKEY_PIN & pgm_read_byte(&MKeyMasks[mXi]))) ) {
					if (!BLFlag.is_signaled()) {
						BLFlag.signal();
					}
					key_ev_qu.push(static_cast<signal>(pgm_read_byte(&MKeySignalsPress[mYi][mXi])));
					break;
				}
			}
			mkey_read_old[mYi] = MKEY_PIN;
			MKEY_PORT |= pgm_read_byte(&MKeyMasks_Y[mYi]);  //Снимаем выделение со столбца
		}
	/*	Сканирование клавиатуры нет необходимости производить очень часто, т.к.:
			1. Человек, который ей пользуется, физически не может нажимать на кнопки слишком часто;
			2. У каждой кнопки есть дребезг, и для его програмного подавления нужно понижать частоту сканирования
	   	Учитывая всё вышесказанное, достаточно производить сканирование с частотой около 60 Гц. Именно поэтому
		сканирование клавиатуры и энкодера происходит в разных процессах, получающих управление с разной частотой.
		О подробностях расчёта величины сна, см. LcdDrv.cpp		*/

		sleep(33);
	}
}

template<> void TUserProc::exec() {
	RTOSMain();
	sleep();		//Если вышли из пользовательской функции, погружаем пользовательский процесс в бесконечный сон, чтобы не поломать систему.
}

void KeyInit() {
	ENC_PORT |= _BV(ENC_A)|_BV(ENC_B)|_BV(ENC_BUT);
	ENC_DDR &= ~(_BV(ENC_A)|_BV(ENC_B)|_BV(ENC_BUT));

	MKEY_DDR &= ~(_BV(MKEY_X1)|_BV(MKEY_X2)|_BV(MKEY_X3)|_BV(MKEY_X4));		//Строки - входы
	MKEY_DDR |= _BV(MKEY_Y1)|_BV(MKEY_Y2)|_BV(MKEY_Y3);						//Столбцы - выходы
	MKEY_PORT |= _BV(MKEY_Y1)|_BV(MKEY_Y2)|_BV(MKEY_Y3);					//Снимаем выделение со всех столбцов
	MKEY_PORT |= _BV(MKEY_X1)|_BV(MKEY_X2)|_BV(MKEY_X3)|_BV(MKEY_X4);		//Pull-down для строк
}
