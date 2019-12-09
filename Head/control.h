#ifndef _CONTROL_H_
#define _CONTROL_H_
#include"database.h"

typedef struct controlinfo
{
	short int ord;         //子菜单号
	short int ord_last;    //子菜单_last
	short int itemnum;     //父菜单号
	short int itemnum_last;//防止重复刷新
	short int func;        //功能函数号  0沪深、8行情、9基本面、10技术面、11快速选股、12交易、13换股、14个人、15关于
	int key;               //键盘
} Ctr;


void control_main(int* func,USER* u);
void control_mouse(Ctr* cinfo,USER* u);
void control_key(Ctr* cinfo);
int control_func(Ctr* cinfo, USER* u, int* sto_list, int* page);
int control_menu(Ctr* cinfo);
void submenu_draw(short int n);
void mainmenu_draw(void);
void lightbutton_menu(short int ord);
void recoverbutton_menu(short int ord_last);
void draw_stoqueue(void);
void control_judge(USER* u);
void cpop(int i);


#endif