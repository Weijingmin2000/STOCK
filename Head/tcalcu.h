#ifndef _TCALCU_H_
#define _TCALCU_H_
#include"database.h"

int K_onelimitup(PRICEINFO* pinfo);//单根K线——一字涨停板
int K_longhatch(PRICEINFO* pinfo);//单根K线——长上影线
int K_greatredline(PRICEINFO* pinfo);//单根K线——大阳线

int K2_manysuppress(PRICEINFO* pinfo);//双根K线——多方压制
int K2_emptysuppress(PRICEINFO* pinfo);//双根K线——空方压制
int K2_trendpause(PRICEINFO* pinfo);//双根K线——走势停顿

int K3_tribleredline(PRICEINFO* pinfo);//三根K线——三连阳
int K3_triblegreenline(PRICEINFO* pinfo);//三根K线——三连阴
int K3_islandreverse(PRICEINFO* pinfo);//三根K线——岛形反转

int KN_bathbeauty(PRICEINFO* pinfo);//多根K线——出水芙蓉
int KN_fodderchoppor(PRICEINFO* pinfo);//多根K线——断头铡刀

int MA_manyarray(float* Close);//均线——多头排列
int MA_emptyarray(float* Close);//均线——空头排列
int MA_bond(float* Close);// 均线——粘合
int MA_divergence(float* close);//均线——发散
int MA_turnup(float* close);//均线——拐头向上
int MA_turndown(float* close);//均线——拐头向下
void get_7MA(float MA[][4], float* close);//获得7天里的4种均线

int KDJ_goldencross(PRICEINFO* pinfo);//随机指标——金叉
int KDJ_deadcross(PRICEINFO* pinfo);//随机指标——死叉
int KDJ_overbuy(PRICEINFO* pinfo);//随机指标——超买
int KDJ_oversell(PRICEINFO* pinfo);//随机指标——超卖
void get_KDJ(PRICEINFO* pinfo, float* k, float* d, float* j);//获得7天KDJ的值

int MACD_updevide(PRICEINFO* pinfo);//异同移动平均线——顶背离
int MACD_goldencross(PRICEINFO* pinfo);//异同移动平均线——金叉
int MACD_downdevide(PRICEINFO* pinfo);//异同移动平均线——底背离
int MACD_deadcross(PRICEINFO* pinfo);//异同移动平均线——死叉
void get_MACD(PRICEINFO* pinfo, float* ema12, float* ema26, float* diff, float* dea);//获得7天里MACD的值

int RSI_overbuy(PRICEINFO* pinfo);//相对强弱指标——超买
int RSI_oversell(PRICEINFO* pinfo);//相对强弱指标——超卖
float get_RSI(PRICEINFO* pinfo, int date, int days);//获得7天里的RSI，RSI有6,12,24的区分

int VOLOBV_trendup(PRICEINFO* pinfo);//成交量/能量潮——上涨趋势
int VOLOBV_trenddown(PRICEINFO* pinfo);//成交量/能量潮——下降趋势
void get_OBV(PRICEINFO* pinfo, float* obv);//获得7天里的能量潮

int BIAS_overbuy(PRICEINFO* pinfo);// 乖离率——超买
int BIAS_oversell(PRICEINFO* pinfo);//乖离率——超卖
void get_BIAS(PRICEINFO* pinfo, float* bias6, float* bias12, float* bias24);//获得7天的乖离率

int BOLL_breaktoprail(PRICEINFO* pinfo);//布林带——突破上轨
int BOLL_breakmediumrail(PRICEINFO* pinfo);//布林带——突破中轨
int BOLL_breakbottomrail(PRICEINFO* pinfo);//布林带——突破下轨
void get_BOLL(PRICEINFO* pinfo, float* uprail, float* midrail, float* downrail);//获得7天的布林带
float get_std_deviation(PRICEINFO* pinfo, int days, int date);//计算标准差,days为20.0

#endif