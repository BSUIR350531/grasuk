/********************************************************************
	Created:	2015/09/20   15:27:27
	Filename: 	GameConsole2.1\Tetris\TetrisBoard.hpp

	Author:		SlavaGrasiuk
	
	purpose:	Tetris board class
*********************************************************************/

#ifndef __TETRISBOARD_H__
#define __TETRISBOARD_H__

#include <PlatformDrv.h>
#include "TetrisBase.hpp"

#define BOARD_BORDER_COLOR	static_cast<color8>(0x00)	//Чёрный

class TetrisBoard: public SimpleGraphObj, public TetrisBase {
	color8 board[21][10], cur_figure_color;
	unsigned char figure, figure_orient, figure_x, figure_y;
	signed char cur_figure[3][2];
	bool initDone;
	
public:
	TetrisBoard(const coord x_pos, const coord y_pos);
	TetrisBoard(const TetrisBoard&) = delete;	//Этот объект нельзя копировать
	void operator=(const TetrisBoard&) = delete;	//И присвоить
	void redraw();
	void DrawStack();
	void GameOverEffect();
	void MoveRight();	//Сдвиг текущей фигурки на одну клетку вправо, если это возможно
	void MoveLeft();	//Сдвиг текущей фигурки на одну клетку влево, если это возможно
	void Rotate();	//Поворот фигурки, если это возможно
	unsigned char FindAndDeleteLines();		// Ищет и удаляет заполненные строки, возвращает колличество удалённых строк
	bool insert(const unsigned char new_figure);	//Добавляет фигурку на доску. Возвращает true, если фигурка НЕ была добавлена
	bool CanStepDown();		//Возвращает true, если сдвиг вниз можно произвести.
	void StepDown();	//Сдвиг фигурки вниз на одну клетку.
	void Save();
	void Load();
	
private:
	//Можно ли разместить текущую фигурку с новыми координатами и ориентацией
	bool CanPlace(const signed char fig_x, const unsigned char fig_y, const unsigned char fig_or) const;
	/*	!TODO	Стирать нужно не всю фигурку, а только ту часть, которая не перекрывается новой позицией,
	иначе будет мерцание.	*/
	void EraseFig();	//Стереть текущую фигурку с экрана
	void UpdateFig();	//Перерисовать фигурку на экране
}; //TetrisBoard

/*	Используется для сохранения игры. Нельзя создавать ещё один экземпляр TetrisBoard в EEPROM,
т.к. произойдёт вызов конструктора тогда, когда этого не должно просходить, а создать TetrisBoard 
без конструктора нельзя, т.к. это графический объект.	*/
struct GameSaveStruct {
	color8 board[21][10], cur_figure_color;
	unsigned char figure, figure_orient, figure_x, figure_y;
	signed char cur_figure[3][2];
	bool initDone;
};

#endif //__TETRISBOARD_H__
