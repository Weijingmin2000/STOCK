#ifndef _LOGIN_H
#define _LOGIN_H
#include"database.h"

void user_login(int *func,USER* u);	//用户登录界面
void drawlogin(void);
void lightbutton_login(int x, int y, int x1, int y1, int color1, int color2, int flag);
void recoverbutton_login(int status);

#endif
