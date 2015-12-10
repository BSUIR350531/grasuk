/********************************************************************
	Created:	2015/09/23   1:33:14
	Filename: 	GameConsole2.1\PlatformDrv\LayerSys\LayerSys.hpp

	Author:		SlavaGrasiuk
	
	purpose:	Graphic layers system
*********************************************************************/

#ifndef LAYERSYS_H_
#define LAYERSYS_H_

#include "Stack.hpp"
#include "../LCD/LcdDrv.hpp"

#define MAX_GRAPH_OBJ_NUM	8		//Максимальное колличество графических объектов на слое
#define MAX_LAYERS_COUNT	4		//Максимальное колличество слоёв

/*	Осуществляют управление фокусом ввода и отправку событий
элементам управления. Для работы необходимо вызывать циклически.	*/

//	С ожиданием событий клавиатуры
void GUIMainLoopW();

//	Без ожидания событий клавиатуры
void GUIMainLoopG();

/*	Классы layer, GraphObj, SimpleGraphObj и SelectableGraphObj тесно связаны между собой, 
плюс они должны быть доступны извне, поэтому их объявления нельзя выносить в разные файлы.	*/

/*Абстрактный класс графический объект.
Представляет общий интерфейс для всех графических объектов*/
class GraphObj {
	bool CanReciveFocus;

protected:
	bool Enabled;

public:
	//Перерисовка объекта. Будет вызываться каждый раз при удалении слоя над ним.
	virtual void redraw() = 0;
	
	/*	Возможно, правильнее было бы разместить эти функции только в классе SelectableGraphObj,
	а затем при сканировании менять тип указателя для доступа к ним, но с использованием RTTI
	на AVR связаны определённые трудности.	*/
	virtual void Select() = 0;	//Установка фокуса ввода
	virtual void Deselect() = 0;	//Снятие фокуса ввода
	virtual void SignalPush() = 0;		//Нажатие на объект
	virtual void SignalRelease() = 0;  //Отпускание объекта

	//Помещает указатель на графический объект в массив объектов самого верхнего слоя.
	//Type - может ли объект принимать фокус ввода.
	GraphObj(const bool type);

	bool IsSelectable() const { return CanReciveFocus; }

	friend void GUIMainLoopW();
	friend void GUIMainLoopG();
}; //GraphObj

typedef GraphObj *GraphObjPtr;

//	Базовый класс графических объектов, которые могут принимать фокус ввода
class SelectableGraphObj: public GraphObj {
public:
	SelectableGraphObj(): GraphObj(true) {}
	
	virtual void redraw() = 0;
	
	virtual void Select() = 0;	//Установка фокуса ввода
	virtual void Deselect() = 0;	//Снятие фокуса ввода
	virtual void SignalPush() = 0;		//Нажатие на объект
	virtual void SignalRelease() = 0;  //Отпускание объекта
	
	void enable(bool Update = true) { //Включает объект
		Enabled = true; 
		if(Update) {
			Enable();
		}
	}
	
	void disable(bool Update = true) { //Выключает объект
		Enabled = false; 
		if(Update) {
			Disable();
		}
	}	

protected:
	//Производят перерисовку при включении/отключении объекта
	virtual void Enable() = 0;		
	virtual void Disable() = 0;
};

typedef SelectableGraphObj *SelGraphObjPtr;

//	Базовый класс графических объектов, которые НЕ могут принимать фокус ввода
class SimpleGraphObj: public GraphObj {
public:
	SimpleGraphObj(): GraphObj(false) {}
	virtual void redraw() = 0;

private:
	//Достались по наследству, но не будут использованы наследниками
	void Select() {}
	void Deselect() {}
	void SignalPush() {}
	void SignalRelease() {}
};

class layer {
	GraphObjPtr GraphObjArr[MAX_GRAPH_OBJ_NUM];		//Массив указателей на объекты текущего слоя
	unsigned char SelObjInd[MAX_GRAPH_OBJ_NUM];		//Индексы элементов типа SelectableGraphObj
	unsigned char SelObjNum, ObjNum;	//Колличество объектов SelectableGraphObj и общее колличество объетов
	bool Filled, needRedraw;
	color8 LayerFill8;
	color16 LayerFill16;
	ColorMode LayerFillColorMode;
	
	friend class GraphObj;
	friend void GUIMainLoopW();
	friend void GUIMainLoopG();
	
public:
	layer();
	layer(const color8 color);
	layer(const color16 color);
	color8 GetColor() const { return LayerFill8; }
	bool filled() const { return Filled; }
	void DisableRedraw();	//Отключение автоматической перерисовки предыдущего слоя
	~layer();
	
private:
	bool PutObj(const GraphObjPtr obj);
	bool PutSelObj(const GraphObjPtr obj);
};

typedef layer *layerPtr;
extern stack<layerPtr, MAX_LAYERS_COUNT> LayerStack;
extern "C" void __cxa_pure_virtual();

#endif /* LAYERSYS_H_ */