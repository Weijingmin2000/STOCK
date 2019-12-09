#ifndef _SLCTFUNC_H_
#define _SLCTFUNC_H_
#include"control.h"
#include"database.h"


void T_stoselect1(Ctr* cinfo, USER* u, int* page, int opt[][6], int* sto_list);//本页显示K线形态选股，MA均线形态选股
void T_select1_check(int opt[][6]);//寻找kline标记，打标记并给对应的opt数组赋值
void MA_select1_check(int* precolor, int* color, int opt[][6]);//寻找ma标记，打标记并给对应的opt数组赋值
void find_Klinebutton(int* tech, int* index, int* x, int* y);//找到是多少根K线形态中的某一指标
void find_lastbutton(int tech, int last, int* pre_x, int* pre_y);//找到上一个标记的坐标
void find_MAbutton(int* tech, int* index, int* x, int* y);//找到MA中的某一形态指标

void T_stoselect2(Ctr* cinfo, USER* u, int* page, int opt[][6], int* sto_list);//本页显示MA, KDJ, MACD, RSI, VOL/OBV, BIAS, BOLL
void T_select2_check(int opt[][6]);//寻找标记，打标记并给对应的opt数组赋值
void find_button2(int tech, int last, int* pre_x, int* pre_y);//找到上一个标记的坐标和这个标记的坐标
void find_Kstobutton2(int* tech, int* index, int* x, int* y);//找到是那个类型的某一指标

int T_searchresult(Ctr* cinfo, USER* u, int* p_page, int* sto_list,int return_page);//显示搜索结果
void draw_T_result(void);//画搜索结果界面
void show_searchsto(STODAYCHAR* info, int coordY, char* sto);//打印搜索的股票信息
void show_failpop(void);//没找到股票的返回弹窗

#endif