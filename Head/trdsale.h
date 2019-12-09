#ifndef _TRDSALE_H_
#define _TRDSALE_H_

#include"control.h"
#include"database.h"

void sale(short int *trmenu_ord,short int *trmenu_ord_last,Ctr* cinfo,USER* u);

void lightbutton_trade_sale(short int num);

void recoverbutton_trade_sale(short int num);

int get_sale_hold(USER* u, char* stk_name, unsigned long* stk_afford);

int stk_judgesum2(unsigned long stk_num,unsigned long stk_afford,float stk_price,char* STK_SUM,char* STK_NUM, double* stk_sum);

int stk_judgesale(char* stk_code,char* stk_name, char* stk_num, char* stk_sum, char* stk_price, char* stk_price_mar);

void new_saleafford(USER* u, unsigned long* stk_afford, char* STK_NAME, char* STK_AFFORD);

#endif