#ifndef _PERSON_H_
#define _PERSON_H_
#include"control.h"
#include"database.h"

void personinfo(Ctr* cinfo, USER* u);//资产控制函数
void draw_ownmenu(void);//画界面
void hold_draw(void);
void lightbutton_person(int x1, int y1, int x2, int y2, int color, int button);//按钮点亮
void recoverbutton_person(int button);//按钮恢复


#endif
