#ifndef _TRANSFER_H_
#define _TRANSFER_H_

void draw_transfer(void);//画转账界面
void transfer_money(USER *u);//转账函数
void lightbutton_transfer(int x, int y, int x1, int y1, int color1, int color2, int flag);//转账点亮
void recoverbutton_transfer(int status);//转账按钮恢复

#endif