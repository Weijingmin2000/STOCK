#ifndef _TECHSELECT_H_
#define _TECHSELECT_H_
#include"control.h"
#include"database.h"


int technical_control(Ctr* cinfo, USER* u, int* sto_list, int* page);//两页选股的总控函数
void lightbutton_Tselect(int num);//按钮点亮
void recoverbutton_Tselect(int num);//恢复点亮
void draw_T_stoselect1(void);
void draw_T_stoselect2(void);
void draw_element(int x, int y, int state, int color);

#endif