#ifndef _QSEARCH_H_
#define _QSEARCH_H_

#include"database.h"
#include"control.h"

//在行情里打印哈希查找对应的股票名
void output_stoname(char* name);

//哈希顺序查找，sto_num关键字全匹配
void search_name(int sto_num, char* search_name);

//改变显示的股票
void change_sto(int* sto_num, int* pos, int No_date, STODAYFLOAT info[], UPLINE* upinfo, DOWNLINE* downinfo, float* max, float* min);

//给用于搜索的sto_No赋每一页的值
void queueup(int* sto, int* sto_No, int queuesize);

//给用于搜索的sto_No赋每一页的值
void queuedown(int* sto, int* sto_No, int queuesize);

//点亮标记，按两下搜索并跳转
int queuesearch(int* sto_No, int* pre_k, int* k);

#endif
