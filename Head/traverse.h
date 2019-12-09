#ifndef _TRAVERSE_H_
#define _TRAVERSE_H_
#include"database.h"

int get_stkname(char *stk_code,char *stk_name,char *stk_price); //返回值是stock的位置
void get_markettime(struct tm* tp, int* minute, int flag);//获得行情里的对应时间
void get_time(struct tm *tp, int *minute,int flag);
void get_buy(int stk_locat,char* stk_code,char* stk_name, char* stk_price);
void get_balance1(USER* us,float* stk_balance);
void input_balance(USER* us,float stk_balance);
void read_balance(float stk_balance);
int get_hst(USER* u, int page);
int get_hold_stk(Hold* hold, char* STK_CODE, char* STK_ADR);
int get_hold_day(int* prime,int* now);
int get_hold(USER* u,int page);
void get_asset(USER* u);
void update_hold_day(struct tm* tp, USER* u);

#endif