#ifndef _STODAYREAD_H_
#define _STODAYREAD_H_

#include"database.h"
#include"public.h"

//int check_database(int sto_number)//返回该数据库的最大值
void sto_daybyday(int sto_number, long int start_date,long int days, STODAYCHAR* stk);//获取从指定天数开始的股票信息
void sto_daybyday2float(int sto_num, long int No_date, long int read_days, STODAYFLOAT* info);//将所得天数的信息转换为浮点数
void turn_a2_f_t(STODAYCHAR* str, STODAYFLOAT* f);//总控数据类型修改函数 
void turn_a2f(char* str, float* price);//将每日指标都转换为浮点数
void turn_a2i(char* str, int* y, int* m, int* d);//date切换成int


#endif