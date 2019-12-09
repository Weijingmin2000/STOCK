#ifndef _INITMENU_H
#define _INITMENU_H

void initmenu_set(int* func);	//初始化选择界面
void drawmenu(void);
void lightbutton_initmenu(int x, int y, int x1, int y1, int color1, int color2, int flag);
void recoverbutton_initmenu(int btnum);

#endif