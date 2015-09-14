/* 
* ProgressBar.cpp
*
* Created: 26.04.2015 18:57:39
* Author: Администратор
*/

#include "ProgressBar.h"
#include "glcd.h"
#include <math.h>
#include <stdlib.h>

// default constructor
ProgressBar::ProgressBar() {
} //ProgressBar

ProgressBar::ProgressBar(const unsigned char x_i, const unsigned char y_i, const unsigned char w_i, const unsigned char h_i, const unsigned char bar_col, const unsigned char bord_col, const unsigned char bg_col, const unsigned int pos_num) {
	x = x_i;
	y = y_i;
	w = w_i;
	h = h_i;
	bar_color = bar_col;
	border_color = bord_col;
	bg_color = bg_col;
	logic_pos_num = pos_num;
	cur_pos = cur_logic_pos = 0;
	
	glcd_rect8(x, y, x+h-1, y+w-1, border_color);
	
	scale = (float)(w-4) / (float)logic_pos_num;
}

ProgressBar::ProgressBar(const unsigned char x_i, const unsigned char y_i, const unsigned char w_i, const unsigned char h_i, const unsigned char bar_col, const unsigned char bord_col, const unsigned char bg_col) {
		x = x_i;
		y = y_i;
		w = w_i;
		h = h_i;
		bar_color = bar_col;
		border_color = bord_col;
		bg_color = bg_col;
		logic_pos_num = w_i;
		cur_pos = cur_logic_pos = 0;
		
		glcd_rect8(x, y, x+h-1, y+w-1, border_color);
		
		scale = 1.0;
}

void ProgressBar::SetPosition(const unsigned int pos) {
	cur_logic_pos = pos;
	cur_pos = floor(scale * cur_logic_pos);
	ls020_rectangle8(x+2, y+2, x+h-3, y+w-3, bg_color);
	ls020_rectangle8(x+2, y+2, x+h-3, y+cur_pos+1, bar_color);
}

void ProgressBar::StepForward() {
	if(cur_logic_pos < logic_pos_num) {
		cur_pos = floor(scale * ++cur_logic_pos);
		ls020_rectangle8(x+2, y+2, x+h-3, y+cur_pos+1, bar_color);
	}
}

void ProgressBar::StepBack() {
	if(cur_logic_pos > 0) {
		cur_pos = floor(scale * --cur_logic_pos);
		ls020_rectangle8(x+2, y+2+cur_pos, x+h-3, y+w-3, bg_color);
	}
}

void ProgressBar::SetLogicPosNum(const unsigned int num) {
	logic_pos_num = num;
	scale = (float)(w-4) / (float)logic_pos_num;
}

void ProgressBar::SetBarColor(const unsigned char color) {
	bar_color = color;
	if(cur_logic_pos > 0) {
		ls020_rectangle8(x+2, y+2, x+h-3, y+cur_pos+1, bar_color);
	}
}

void ProgressBar::SetBorderColor(const unsigned char color) {
	border_color = color;
	glcd_rect8(x, y, x+h-1, y+w-1, border_color);
}

unsigned int ProgressBar::GetCurPos() const {
	return cur_logic_pos;
}