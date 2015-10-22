/********************************************************************
	Created:	2014/12/27   13:10:37
	Filename: 	GameConsole2.1\PlatformDrv\KeyDrv.h

	Author:		SlavaGrasiuk
	
	purpose:	Keyboard connection options
*********************************************************************/

#ifndef KEYDRV_H_
#define KEYDRV_H_

/**************************************************************************\
|**				НАСТРОЙКА ПОДКЛЮЧЕНИЯ КЛАВИАТУРЫ И ЭНКОДЕРА				 **|
\**************************************************************************/

#define MKEY_PORT_NAME			C			//Буква порта
#define MKEY_X1					0			//Контакт строки 1
#define MKEY_X2					2			//Контакт строки 2
#define MKEY_X3					5			//Контакт строки 3
#define MKEY_X4					4			//Контакт строки 4
#define MKEY_Y1					1			//Контакт столбца 1
#define MKEY_Y2					3			//Контакт столбца 2
#define MKEY_Y3					6			//Контакт столбца 3

#define ENC_PORT_NAME			D			//Буква порта
#define ENC_BUT					6			//Кнопка энкодера
#define ENC_A					3			//Первый контакт энкодера
#define ENC_B					1			//Второй контакт энкодера

/**************************************************************************/

//Казалось бы, зачем нужен PRECOMP2 когда есть PRECOMP, но по другому просто не будет работать
#define PRECOMP2(x, y)			y ## x			
#define PRECOMP(x, y)			PRECOMP2(x, y)

#define ENC_PORT				PRECOMP(ENC_PORT_NAME, PORT)
#define ENC_DDR					PRECOMP(ENC_PORT_NAME, DDR)
#define ENC_PIN					PRECOMP(ENC_PORT_NAME, PIN)

#define MKEY_PORT				PRECOMP(MKEY_PORT_NAME, PORT)
#define MKEY_DDR				PRECOMP(MKEY_PORT_NAME, DDR)
#define MKEY_PIN				PRECOMP(MKEY_PORT_NAME, PIN)

enum signal {
	ButEnc_press = 0,
	ButEnc_release,
	Enc_Up,
	Enc_Down,
	But1_press,
	But1_release,
	But2_press,
	But2_release,
	But3_press,
	But3_release,
	But4_press,
	But4_release,
	But5_press,
	But5_release,
	But6_press,
	But6_release,
	But7_press,
	But7_release,
	But8_press,
	But8_release,
	But9_press,
	But9_release,
	But0_press,
	But0_release,
	ButDel_press,
	ButDel_release,
	ButEnt_press,
	ButEnt_release,
	none
};

//void KeyInit() __attribute__((naked)) __attribute__((section(".init8")));
//signal GetKeySignal();

#endif /* KEYDRV_H_ */