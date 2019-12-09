#ifndef _TRDBUY_H_
#define _TRDBUY_H_
#include"control.h"
#include"database.h"

void buy(short int *trmenu_ord,short int *trmenu_ord_last,Ctr* cinfo,USER* u,int stk_locat);

void lightbutton_trade_buy(short int num);

void recoverbutton_trade_buy(short int num);

void stk_input_hst(USER* u,char* stk_time,char* stk_code,char* stk_name,char* stk_num,char* stk_price,char* stk_sum,int i);   
//输入历史记录数据库

void stk_input_hold(USER* u, char* stk_time,char* stk_code, char* stk_name,char* stk_num,char* stk_price,int i);
//输入持仓数据库

void get_new_price(char* STK_PRICE_MAR, float* stk_price_mar, int color);                     //获取新的市价

int stk_judgebuy(char* stk_code,char* stk_name,char* stk_num,char* stk_price,char* stk_sum,char* stk_price_mar);
//判断是否在市价范围内

int stk_judgesum(unsigned long stk_num,unsigned long stk_afford,float stk_price, char* STK_SUM,char* STK_NUM,double* stk_sum);

void new_afford(unsigned long* stk_afford,float stk_balance, float stk_price, char* STK_AFFORD);
#endif