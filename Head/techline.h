#ifndef _TECHLINE_H_
#define _THCHLINE_H_

#include"database.h"

void majorinfo_show(int kind, STODAYFLOAT* info, UPLINE* major, float max, float min);//显示主图信息的总控函数
void viceinfo_show(int kind, STODAYFLOAT* info, DOWNLINE* vice);////显示副图信息的总控函数

void Kline_draw(STODAYFLOAT* info, float max, float min);//画K线,max,min为get_border计算的边界
void MA_draw(STODAYFLOAT* info, UPLINE* major, float max, float min);//绘制MA线，major为存放主图均线信息的结构体指针
void BOLL_draw(STODAYFLOAT* info, UPLINE* major, float max, float min);//绘制BOLL布林线指标
void ENE_draw(STODAYFLOAT* info, UPLINE* major, float max, float min);//绘制ENE轨道线指标


void VOL_draw(STODAYFLOAT* info, DOWNLINE* vice);//绘制VOL成交量柱状图,vice为存放副图信息的结构体指针
void BRAR_draw(STODAYFLOAT* info, DOWNLINE* vice);//绘制人气意愿指标BRAR
void BIAS_draw(STODAYFLOAT* info, DOWNLINE* vice);//绘制乖离率BIAS
void CR_draw(STODAYFLOAT* info, DOWNLINE* vice);//绘制能量指标CR
void DMA_draw(STODAYFLOAT* info, DOWNLINE* vice);//绘制平行线差DMA
void OBV_draw(STODAYFLOAT* info, DOWNLINE* vice);//绘制能量潮OBV
void KDJ_draw(STODAYFLOAT* info, DOWNLINE* vice);//绘制随机指标KDJ
void MACD_draw(STODAYFLOAT* info, DOWNLINE* vice);//绘制异同移动平均线MACD
void RSI_draw(STODAYFLOAT* info, DOWNLINE* vice);//绘制相对强弱指标RSI
float RSI_calculate(STODAYFLOAT* info, int date, int days);//计算RSI

#endif