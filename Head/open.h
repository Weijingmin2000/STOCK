#ifndef _OPACCOUNT_H_
#define _OPACCOUNT_H_
#include"database.h"
#include"control.h"

void opaccount(Ctr* cinfo,USER* u);
void draw_opaccount(void);
void draw_exist(void);
int opaccount_complete(char* b,char* bp,char* bcp,USER* u);
void lightbutton_opa(int x, int y, int x1, int y1, int color1, int color2, int flag);
void recoverbutton_opa(int status);
int checkright_bankcard(char* str);
int checkright_bankpassword(char* str);
int checkright_bankconfirmedpassword(char* str1,char* str2);
int judge_samecredit(char* new_credit,int flag);
int input_database_bk(char* bankcard,char* bankcode,USER* us);

#endif