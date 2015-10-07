/********************************************************************
	Created:	2015/09/23   1:33:14
	Filename: 	GameConsole2.1\PlatformDrv\LayerSys\LayerSys.hpp

	Author:		SlavaGrasiuk
	
	purpose:	Graphic layers system
*********************************************************************/

#ifndef LAYERSYS_H_
#define LAYERSYS_H_

#include "../LCD/LcdDrv.hpp"

#define GRAPH_OBJ_NUM	8		//Колличество графических объектов на слое
#define LAYERS_COUNT	4		//Колличество слоёв

/*Абстрактный класс графический объект.
Представляет общий интерфейс для всех графических объектов*/
class GraphObj {
	bool CanReciveFocus;
	
public:
	//Перерисовка объекта. Будет вызываться каждый раз при удалении слоя над ним.
	virtual void redraw() = 0;
	
	//Помещает указатель на графический объект в массив объектов самого верхнего слоя.
	//Type - может ли объект принимать фокус ввода.
	GraphObj(const bool type);
	
	//Может ли объект принимать фокус ввода
	bool IsSelectable() const { return CanReciveFocus; }
};

//Абстрактный класс графических объектов, которые могут принимать фокус ввода.
class SelectableGraphObj: public GraphObj {
public:
	SelectableGraphObj(): GraphObj(true) {}
	
	virtual void redraw() = 0;	
	
	//Для доступа к этим функциям через указатель на базовый класс нужно привести его тип к указателю на SelectableGraphObj
	virtual void Select() = 0;	//Установка фокуса ввода
	virtual void Deselect() = 0;	//Снятие фокуса ввода
	virtual void SignalPush() = 0;		//Нажатие на объект
	virtual void SignalRelease() = 0;  //Отпускание объекта
};

//Абстрактный класс для простых объектов, которые не могут получать фокус ввода
class SimpleGraphObj: public GraphObj {
public:
	SimpleGraphObj(): GraphObj(false) {}
	virtual void redraw() = 0;
};

typedef GraphObj *GraphObjP;
class layer;
typedef layer *layerP;
class stack;

//Слой - крайне упрощённое подобие окон в windows или X11
class layer {
	GraphObjP GraphObjArr[GRAPH_OBJ_NUM];		//Массив указателей на объекты текущего слоя
	bool Filled;
	color8 LayerFill8;
	color16 LayerFill16;
	ColorMode LayerFillColorMode;
	friend class GraphObj;
	
public:
	layer();
	layer(const color8 color);
	layer(const color16 color);
	~layer();
};

//Статический стек для хранения указателей на слои
class stack {
	layerP arr[LAYERS_COUNT];
	uint8_t top;
public:
	stack(): top(0) {}
	void push(const layerP dat);
	layerP pop();
	layerP pick() const { return arr[top-1]; }
	bool IsEmpty () const { return !top ? true : false; }
};

#endif /* LAYERSYS_H_ */