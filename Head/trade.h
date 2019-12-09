#ifndef _TRADE_H_
#define _TRADE_H_
#include"control.h"
#include"database.h"


void trade(Ctr* cinfo,USER* u,short int t,int stk_num);      //交易函数
void input_trade(char* id, int x1, int y1, int charnum, int color);  //输入法
void trade_menu_draw(int flag);
void buy_draw(void);         //买入画图无cleardevice
void sale_draw(void);        //卖出同上
void history_draw(void);     //历史成交
void favorite_draw(void);    //自选股
int change_trdmenu(short int *trmenu_ord,short int *trmenu_ord_last);  //切换交易菜单
void float_to_str(char *str,float num,int bits);	//bits为要画的限定小数位数
void stk_turn_a2i(char* str, unsigned long* num);      //char to int
int stk_judge(unsigned long stk_afford, int divisor);
void stk_hundred(unsigned long stk_afford,char* STK_AFFORD);
void trd_history(short int *trmenu_ord,short int *trmenu_ord_last,Ctr* cinfo,USER* u);



#endif