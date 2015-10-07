/********************************************************************
	Created:	2015/09/23   10:52:30
	Filename: 	GameConsole2.1\PlatformDrv\LayerSys\LayerSys.cpp

	Author:		SlavaGrasiuk
	
	purpose:	Graphic layers system
*********************************************************************/

#include "../PlatformDrv.h"
#include "LayerSys.hpp"
#include <stddef.h>

extern "C" void __cxa_pure_virtual() {
	while(1);
}

void stack::push(const layerP dat) {
	if(top < LAYERS_COUNT + 1) {
		arr[top++] = dat;
	}
}

layerP stack::pop() {
	layerP ret = arr[top-1];
	top--;
	return ret;
}

static stack LayerStack;		//Стек слоёв
TSelCtrlProc SelCtrlProc;		//Контроллер фокуса ввода

template<> void TSelCtrlProc::exec() {
	
}

GraphObj::GraphObj(const bool type) {
	CanReciveFocus = type;
	if( !LayerStack.IsEmpty() ) {	//Стек не пуст, создан хотя бы один слой.
		GraphObjP *TopLayerGraphList = LayerStack.pick()->GraphObjArr;
		register unsigned char i;
		for(i = 0; i < GRAPH_OBJ_NUM; i++) {	//Ищем свободное место
			if(TopLayerGraphList[i] == NULL) {
				break;
			}
		}
		if (i < GRAPH_OBJ_NUM-1) {	//Список объектов полон
			TopLayerGraphList[i] = this;
		}
	}
}

layer::layer() {
	LayerStack.push(this);
	for(register unsigned char i = 0; i < GRAPH_OBJ_NUM; GraphObjArr[i] = NULL, i++);
	Filled = false;
}

layer::layer(const color8 color) {
	LayerStack.push(this);
	for(register unsigned char i = 0; i < GRAPH_OBJ_NUM; GraphObjArr[i] = NULL, i++);
	Filled = true;
	LayerFill8 = color;
	LayerFillColorMode = mode_8;
	register coord x = 0, y = 0;
	register size w = LCD::Width(), h = LCD::Height();
	LCD::FillRect(x, y, w, h, color);
}

layer::layer(const color16 color) {
	LayerStack.push(this);
	for(register unsigned char i = 0; i < GRAPH_OBJ_NUM; GraphObjArr[i] = NULL, i++);
	Filled = true;
	LayerFill16 = color;
	LayerFillColorMode = mode_16;
	register coord x = 0, y = 0;
	register size w = LCD::Width(), h = LCD::Height();
	LCD::FillRect(x, y, w, h, color);
}

layer::~layer() {
	LayerStack.pop();	//Удаляем текущий слой
	//Получаемм указатель на предыдущий слой
	layerP PrevLayer = LayerStack.pick();
	//Заливаем предыдущий слой цветом, если требуется
	if(PrevLayer->Filled) {
		register coord x = 0, y = 0;
		register size w = LCD::Width(), h = LCD::Height();
		if(PrevLayer->LayerFillColorMode == mode_8) {
			LCD::FillRect(x, y, w, h, PrevLayer->LayerFill8);
		} else {
			LCD::FillRect(x, y, w, h, PrevLayer->LayerFill16);
		}
	}
	//Перерисовываем объекты предыдущего слоя
	for(register unsigned char i = 0; PrevLayer->GraphObjArr[i] != NULL && i < GRAPH_OBJ_NUM; PrevLayer->GraphObjArr[i]->redraw(), i++);
}