#ifndef _MARKETFUNC_H_
#define _MARKETFUNC_H_

#include"database.h"

//改变显示信息的日期
void change_datadate(int sto_num, long int* No_date, STODAYFLOAT info[], UPLINE* upinfo, DOWNLINE* downinfo, float* max, float* min, int read_way, int LR);

//计算标准差,days为20.0
float std_deviation(STODAYFLOAT* info, int days, int date);


//寻找画主图的边界函数
void get_border(STODAYFLOAT* info, float* max, float* min, int read_days);


//在指定位置画浮点数
void draw_float(float flt, int x, int y, int bits, int color);

//寻找画副图的边界函数
void get_viceborder(DOWNLINE* vice, float* max, float* min);



//鼠标移动来显示数据
void mouse_datashow(STODAYFLOAT* info, UPLINE upinfo, DOWNLINE downinfo, int* p_preday, int* p_day, int state, int colorstate);

#endif

