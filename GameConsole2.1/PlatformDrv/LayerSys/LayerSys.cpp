/********************************************************************
	Created:	2015/09/23   10:52:30
	Filename: 	GameConsole2.1\PlatformDrv\LayerSys\LayerSys.cpp

	Author:		SlavaGrasiuk
	
	purpose:	Graphic layers system
*********************************************************************/

#include "LayerSys.hpp"
#include "../PlatformDrv.h"
#include <stddef.h>

//Для AVR нет стандартной библиотеки C++ и ABI
extern "C" void __cxa_pure_virtual() {}

stack<layerPtr, MAX_LAYERS_COUNT> LayerStack;		//Стек слоёв

void GUIMainLoopW() {
	static unsigned char CurObj = 0;
	static layerPtr Prevlay = NULL;
	layerPtr CurLay = LayerStack.pick();
	
	if(Prevlay != CurLay) {
		Prevlay = CurLay;
		CurObj = 0;
	}
	
	register unsigned char ObjNum = CurLay->SelObjNum;
	unsigned char *SelObjInd = CurLay->SelObjInd;
	GraphObjPtr *tmp = CurLay->GraphObjArr;
	
	tmp[SelObjInd[CurObj]]->Select();
	
	switch(WaitKeySignal()) {
		case Enc_Up:
			tmp[SelObjInd[CurObj]]->Deselect();
			for(;;) {
				if(++CurObj == ObjNum) {
					CurObj = 0;
				}
				if(tmp[SelObjInd[CurObj]]->Enabled) {
					break;
				}
			}
			tmp[SelObjInd[CurObj]]->Select();
			break;
				
		case Enc_Down:
			tmp[SelObjInd[CurObj]]->Deselect();
			for(;;) {
				if(--CurObj == 0xFF) {
					CurObj = ObjNum-1;
				}
				if(tmp[SelObjInd[CurObj]]->Enabled) {
					break;
				}
			}
			tmp[SelObjInd[CurObj]]->Select();
			break;
			
		case ButEnt_press:
		case ButEnc_press:
			tmp[SelObjInd[CurObj]]->SignalPush();
			break;
				
		case ButEnc_release:
		case ButEnt_release:
			tmp[SelObjInd[CurObj]]->SignalRelease();
				
		default:
			break;
	}
}

void GUIMainLoopG() {
	static unsigned char CurObj = 0;
	static layerPtr Prevlay = NULL;
	layerPtr CurLay = LayerStack.pick();
	
	if(Prevlay != CurLay) {
		Prevlay = CurLay;
		CurObj = 0;
	}
	
	register unsigned char ObjNum = CurLay->SelObjNum;
	unsigned char *SelObjInd = CurLay->SelObjInd;
	GraphObjPtr *tmp = CurLay->GraphObjArr;
	
	tmp[SelObjInd[CurObj]]->Select();
	
	switch(GetKeySignal()) {
		case Enc_Up:
			tmp[SelObjInd[CurObj]]->Deselect();
			for(;;) {
				if(--CurObj == 0xFF) {
					CurObj = ObjNum-1;
				}
				if(tmp[SelObjInd[CurObj]]->Enabled) {
					break;
				}
			}
			tmp[SelObjInd[CurObj]]->Select();
			break;
		
		case Enc_Down:
			tmp[SelObjInd[CurObj]]->Deselect();
			for(;;) {
				if(--CurObj == 0xFF) {
					CurObj = ObjNum-1;
				}
				if(tmp[SelObjInd[CurObj]]->Enabled) {
					break;
				}
			}
			tmp[SelObjInd[CurObj]]->Select();
			break;
		
		case ButEnt_press:
		case ButEnc_press:
			tmp[SelObjInd[CurObj]]->SignalPush();
			break;
		
		case ButEnc_release:
		case ButEnt_release:
			tmp[SelObjInd[CurObj]]->SignalRelease();
		
		default:
		break;
	}
}