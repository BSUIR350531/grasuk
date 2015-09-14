/* 
* ProgressBar.h
*
* Created: 26.04.2015 18:57:39
* Author: Администратор
*/

#ifndef __PROGRESSBAR_H__
#define __PROGRESSBAR_H__

class ProgressBar {
	float scale;		//Масштаб
	unsigned int logic_pos_num, cur_logic_pos;		//Колличество позиций всего и текущая логическая позиция
	unsigned char cur_pos, bar_color, border_color, bg_color, x, y, w, h;//, **BarBuf;
	//char *text;
public:
	ProgressBar();
	ProgressBar(const unsigned char x_i, const unsigned char y_i, const unsigned char w_i, const unsigned char h_i, const unsigned char bar_col, const unsigned char bord_col, const unsigned char bg_col, const unsigned int pos_num);
	ProgressBar(const unsigned char x_i, const unsigned char y_i, const unsigned char w_i, const unsigned char h_i, const unsigned char bar_col, const unsigned char bord_col, const unsigned char bg_col);
	void SetPosition(const unsigned int pos);
	void StepForward();
	void StepBack();
	void SetLogicPosNum(const unsigned int num);
	void SetBarColor(const unsigned char color);
	void SetBorderColor(const unsigned char color);
	unsigned int GetCurPos() const;
/*
protected:
	void FlushBuf();
	void ClearBar();
	void DrawTextCenter(const char * const text);
	void ClearText();
private:
	void pb_draw_virt_rect(const unsigned char x_i, const unsigned char y_i, const unsigned char w_i, const unsigned char h_i, const unsigned char color);*/
};

#endif //__PROGRESSBAR_H__
