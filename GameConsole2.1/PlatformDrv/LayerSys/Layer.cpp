/********************************************************************
	Created:	2015/10/16   0:13:00
	Filename: 	F:\GameConsole2.1\PlatformDrv\LayerSys\Layer.cpp

	Author:		SlavaGrasiuk
	
	purpose:	Layer - very simplified analog of windows in Windows or X11
*********************************************************************/


#include "LayerSys.hpp"

layer::layer(): needRedraw(true) {
	LayerStack.push(this);
	for(register unsigned char i = 0; i < MAX_GRAPH_OBJ_NUM; i++) {
		GraphObjArr[i] = NULL;
	}
	Filled = false;
	SelObjNum = ObjNum = 0;
}

layer::layer(const color8 color): needRedraw(true) {
	LayerStack.push(this);
	for(register unsigned char i = 0; i < MAX_GRAPH_OBJ_NUM; i++) {
		GraphObjArr[i] = NULL;
	}
	Filled = true;
	LayerFill8 = color;
	LayerFillColorMode = mode_8;
	register coord x = 0, y = 0;
	register size w = LCD::Width(), h = LCD::Height();
	LCD::FillRect(x, y, w, h, color);
	SelObjNum = ObjNum = 0;
}

layer::layer(const color16 color): needRedraw(true) {
	LayerStack.push(this);
	for(register unsigned char i = 0; i < MAX_GRAPH_OBJ_NUM; i++) {
		GraphObjArr[i] = NULL;
	}
	Filled = true;
	LayerFill16 = color;
	LayerFillColorMode = mode_16;
	register coord x = 0, y = 0;
	register size w = LCD::Width(), h = LCD::Height();
	LCD::FillRect(x, y, w, h, color);
	SelObjNum = ObjNum = 0;
}

layer::~layer() {
	LayerStack.pop();	//Удаляем текущий слой
	if(needRedraw) {
		layerPtr PrevLayer = LayerStack.pick();		//Получаемм указатель на предыдущий слой
		if(PrevLayer->Filled) {		//Заливаем предыдущий слой цветом, если требуется
			register coord x = 0, y = 0;
			register size w = LCD::Width(), h = LCD::Height();
			if(PrevLayer->LayerFillColorMode == mode_8) {
				LCD::FillRect(x, y, w, h, PrevLayer->LayerFill8);
			} else {
				LCD::FillRect(x, y, w, h, PrevLayer->LayerFill16);
			}
		} /* TODO: Если над слоем без заливки был создан, а затем удалён ещё один слой, то
		необходимо сначала восстановить слой, который находился под слоем без заливки, и имел заливку. */
	
		//Перерисовываем объекты предыдущего слоя
		for(register unsigned char i = 0; PrevLayer->GraphObjArr[i] != NULL && i < MAX_GRAPH_OBJ_NUM; i++) {
			PrevLayer->GraphObjArr[i]->redraw();
		}
	}
} //~Layer

void layer::DisableRedraw() {
	needRedraw = false;
}

bool layer::PutObj(const GraphObjPtr obj) {
	if(ObjNum == MAX_GRAPH_OBJ_NUM) {
		return false;
	}
	GraphObjArr[ObjNum++] = obj;
	return true;
}

bool layer::PutSelObj(const GraphObjPtr obj) {
	if(ObjNum == MAX_GRAPH_OBJ_NUM) {
		return false;
	}
	GraphObjArr[ObjNum++] = obj;
	SelObjInd[SelObjNum++] = ObjNum-1;
	return true;
}