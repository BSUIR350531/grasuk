/********************************************************************
	Created:	2015/09/20   15:27:27
	Filename: 	GameConsole2.1\Tetris\TetrisBoard.cpp

	Author:		SlavaGrasiuk
	
	purpose:	Tetris board class
*********************************************************************/


#include "TetrisBoard.hpp"

TetrisBoard::TetrisBoard(const coord x_pos, const coord y_pos): TetrisBase(x_pos, y_pos) {
	initDone = false;
	
	for(register unsigned char i = 0; i<21; i++) {
		for(register unsigned char j = 0; j<10; j++) {
			board[i][j] = BOARD_EMPTY_BLOCK_COLOR;
		}
	}
	redraw();
	initDone = true;
}

void TetrisBoard::redraw() {
	LCD::FillRect(x, y, 1, LCD::Height() - y - 1, BOARD_BORDER_COLOR);
	LCD::FillRect(x + 1, LCD::Height() - y - 1, 84, 1, BOARD_BORDER_COLOR);
	LCD::FillRect(x + 84, y, 1, LCD::Height() - y - 1, BOARD_BORDER_COLOR);
	DrawStack();
	if(initDone) {	//При создании новой доски от старой в памяти остаётся вектор последней фигурки, которая окажется на дисплее
		UpdateFig();	//Фигурка на самом деле не помещается в стакан, пока не упадёт, поэтому рисуем её отдельно
	}
}

void TetrisBoard::DrawStack() {
	for(register unsigned char i = 0; i<21; i++) {
		for(register unsigned char j = 0; j<10; j++) {
			DrawBlock(GetScreenCoordX(j), GetScreenCoordY(i, 1), board[i][j]);
		}
	}
}

void TetrisBoard::GameOverEffect() {
	int const n = 21;
	int const m = 10;

	int i = 0, j, k = 0;

	while (i < n*m) {	//Для каждого витка
		k++;
		for (j=k-1; j<m-k+1; j++, i++) {		// Заполнение верхнего горизонтального столбца
			DrawBlock(GetScreenCoordX(j), GetScreenCoordY(k-1, 1), 0xFF);
			OS::sleep(20);
		}   
 
		for (j=k; j<n-k+1; j++, i++) {			// Заполнение правого вертикального столбца
			DrawBlock(GetScreenCoordX(m-k), GetScreenCoordY(j, 1), 0xFF);
			OS::sleep(20);
		}  
 
		for (j=m-k-1; j>=k-1; j--, i++) {		// Заполнение нижнего горизонтального столбца
			DrawBlock(GetScreenCoordX(j), GetScreenCoordY(n-k, 1), 0xFF);
			OS::sleep(20);
		}   
 
		for (j=n-k-1; j>=k; j--, i++) {			// Заполнение левого вертикального столбца
			DrawBlock(GetScreenCoordX(k-1), GetScreenCoordY(j, 1), 0xFF);
			OS::sleep(20);
		}   
	}			
}

void TetrisBoard::MoveRight() {
	if(CanPlace(figure_x + 1, figure_y, figure_orient)) {
		EraseFig();
		figure_x++;
		UpdateFig();
	}
}

void TetrisBoard::MoveLeft() {
	if(CanPlace(figure_x - 1, figure_y, figure_orient)) {
		EraseFig();
		figure_x--;
		UpdateFig();
	}
}

void TetrisBoard::Rotate() {
	unsigned char tmp_orient = figure_orient;
	
	if(++tmp_orient == 4) {		//4 возможных ориентации
		tmp_orient = 0;
	}
	if(CanPlace(figure_x, figure_y, tmp_orient)) {
		EraseFig();
		figure_orient = tmp_orient;
		GetFigure(cur_figure, figure, figure_orient);
		UpdateFig();
	}
}

unsigned char TetrisBoard::FindAndDeleteLines() {
	register unsigned char j, linesCnt = 0;
	for(register signed char i = 20; i>=0; i--) {
		for(j = 0; j<10; j++) {
			if(board[i][j] == BOARD_EMPTY_BLOCK_COLOR) {
				break;
			}
		}
		if(j < 10) {
			continue;
		}
	// сейчас в i индекс строки, которую нужно удалить
		for(j = i; j > 0; j--) {
			for(register unsigned char k = 0; k<10; k++) {
				board[j][k] = board[j-1][k];
			}
		}
		for(register unsigned char k = 0; k<10; k++) {
			board[0][k] = BOARD_EMPTY_BLOCK_COLOR;
		}
		linesCnt++;
		i++;	//Строка, опустившеяся сверху, также может быть заполнена, заставим цикл проверить её тоже
	}
	if(linesCnt) {
		DrawStack();
	}
	return linesCnt;
}

bool TetrisBoard::insert(const unsigned char new_figure) {
	figure = new_figure;
	figure_orient = 0;
	figure_x = 4;	//Начальные координаты для всех фигурок
	figure_y = 0;	//
	cur_figure_color = GetColor(figure);
	GetFigure(cur_figure, figure, figure_orient);
	
	if(!CanPlace(figure_x, figure_y, figure_orient)) {
		return true;	//Геймовер детектед
	}
	UpdateFig();
	return false;	//Геймовера нет
}

bool TetrisBoard::CanStepDown() {
	if(CanPlace(figure_x, figure_y + 1, figure_orient)) {
		return true;	//Фигура ещё не упала
	} else {
		board[figure_y][figure_x] = cur_figure_color;
		for(register unsigned char i = 0; i<3; i++) {		//Теперь фигурка становится неподвижным содержимым стакана
			board[figure_y + cur_figure[i][1]][figure_x + cur_figure[i][0]] = cur_figure_color;
		}
		return false;	//Фигура уже упала
	}
}

void TetrisBoard::StepDown() {
	//Проверка не нужна, перед вызовом этой функции точно известно, что можно спуститься вниз
	EraseFig();
	figure_y++;
	UpdateFig();
}

bool TetrisBoard::CanPlace(const signed char fig_x, const unsigned char fig_y, const unsigned char fig_or) const {
	signed char tmp_fig_vect[3][2];
//Проверяем, не выйдем ли за границы доски при попытке вставить фигурку
	if(fig_x < 0 || fig_x > 9 || fig_y > 20) {
		return false;	//fig_x может оказаться отрицательным при вызове, если пытаемся сдвинуть влево фигурку, которая и так на самом левом краю
	}
	GetFigure(tmp_fig_vect, figure, fig_or);	//Получим нужную фигурку
	for(register unsigned char i = 0; i<3; i++) {
		//Нельзя вставить фигурку, если какой-либо её блок вышел за границу стакана
		if( (fig_x + tmp_fig_vect[i][0]) < 0 || (fig_x + tmp_fig_vect[i][0]) > 9 ) {
			return false;
		}
		if( (fig_y + tmp_fig_vect[i][1]) < 0 || (fig_y + tmp_fig_vect[i][1]) > 20) {
			return false;
		}
		//Collision detect
		if(board[fig_y + tmp_fig_vect[i][1]][fig_x + tmp_fig_vect[i][0]] != BOARD_EMPTY_BLOCK_COLOR) {
			return false;
		}
	}
	if(board[fig_y][fig_x] != BOARD_EMPTY_BLOCK_COLOR) {	//Колижн для первого блока проверяется отдельно, его нет в векторе
		return false;
	}
	return true;
}

//Зачем нужны две функции, делающие одно и то же? Дело в том, что такой вариант EraseFig() временный
void TetrisBoard::EraseFig() {
	//	TODO	убрать мерцание
	DrawBlock(GetScreenCoordX(figure_x), GetScreenCoordY(figure_y, 1), BOARD_EMPTY_BLOCK_COLOR);
	for(register unsigned char i = 0; i<3; i++) {
		DrawBlock(GetScreenCoordX(figure_x + cur_figure[i][0]), GetScreenCoordY(figure_y + cur_figure[i][1], 1), BOARD_EMPTY_BLOCK_COLOR);
	}
}

void TetrisBoard::UpdateFig() {
	DrawBlock(GetScreenCoordX(figure_x), GetScreenCoordY(figure_y, 1), cur_figure_color);
	for(register unsigned char i = 0; i<3; i++) {
		DrawBlock(GetScreenCoordX(figure_x + cur_figure[i][0]), GetScreenCoordY(figure_y + cur_figure[i][1], 1), cur_figure_color);
	}
}