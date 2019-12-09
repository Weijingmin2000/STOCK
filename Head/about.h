#ifndef _ABOUT_H_
#define _ABOUT_H_

#include"control.h"
#include"database.h"


void About(Ctr* cinfo, USER* u);//软件信息总控函数
void About_show(void);//显示关于信息
void Help_show(void);//显示帮助信息
void lightbutton_about(int x, int y, int x1, int y1, int color1, int color2);//About按钮点亮
void recoverbutton_about(int status);//About按钮恢复

#endif
