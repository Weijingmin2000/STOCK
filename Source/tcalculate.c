#include"public.h"
#include"database.h"
#include"tcalcu.h"


/*****************************************
tcalculate.c
COPYRIGHT:	weijingmin
FUNCTION:	search technical index
ABSTRACT:
		A.1K(3),2K(3),3K(3),NK(2)
		B.MA(6)
		C.KDJ(3)
		D.MACD(4)
		E.RSI(2)
		F.VOL/OBV(2)
		G.BIAS(2)
		H.BOLL(3)
		I.caculate technical indicator
DATE:2019/10/30
******************************************/


/********************
单K线指标：3个
*********************/

/****************************************************************
FUNCTION:K_onelimitup,K_longhatch,K_greatredline
DESCRIPTION：一字涨停板
			长上影线
			大阳线
INPUT:pinfo
RETURN:无
****************************************************************/
int K_onelimitup(PRICEINFO* pinfo)//单根K线——一字涨停板
{
	int i;
	//寻找近九十天来的一字涨停板
	for (i = 0; i < 90; i++)
	{
		if (pinfo->High[i] == pinfo->Low[i])
		{
			if ((pinfo->Open[i] == pinfo->High[i]) && (pinfo->Close[i] == pinfo->Low[i]))
			{
				return 1;//四价相等
			}
		}
	}
	return 0;
}

int K_longhatch(PRICEINFO* pinfo)//单根K线——长上影线
{
	int i;
	float max = 0;
	//寻找7天内是否存在长上影线
	for (i = 0; i < 7; i++)
	{
		max = pinfo->Open[i];
		if (max < pinfo->Close[i])
		{
			max = pinfo->Close[i];
		}
		if (pinfo->High[i] > max * 1.03)
		{
			return 1;//找到长上影线
		}
	}

	return 0;
}

int K_greatredline(PRICEINFO* pinfo)//单根K线——大阳线
{
	int i;
	//寻找近7天来的大阳线
	for (i = 0; i < 7; i++)
	{
		if (pinfo->Open[i] > pinfo->Close[i])//以开盘价作为最低价，收盘价作为最高价，一路上涨，买势未尽
		{
			if ((pinfo->Open[i] == pinfo->High[i]) && (pinfo->Close[i] == pinfo->Low[i]))
			{
				return 1;//找到
			}
		}
	}
	return 0;
}


/********************
双K线指标：3个
*********************/

/****************************************************************
FUNCTION:K2_manysuppress,K2_emptysuppress,K2_trendpause
DESCRIPTION：多方压制
			空方压制
			走势停顿
INPUT:pinfo
RETURN:无
****************************************************************/
int K2_manysuppress(PRICEINFO* pinfo)//双根K线——多方压制
{
	int i;
	for (i = 8; i > 0; i--)
	{
		//旭日东升
		if (pinfo->Open[i] > pinfo->Close[i] && pinfo->Open[i - 1] < pinfo->Close[i - 1])
		{
			//第一天跌，第二天涨
			if (pinfo->Close[i - 1] > pinfo->Open[i] && pinfo->Open[i - 1] > pinfo->Close[i])//第二天收盘价大于第一天开盘价,开盘价大于第一天收盘价
			{
				return 1;
			}
		}
		//穿头破脚
		if (pinfo->Open[i] > pinfo->Close[i] && pinfo->Open[i - 1] < pinfo->Close[i - 1])
		{
			//第一天跌，第二天涨
			if (pinfo->Open[i - 1]<pinfo->Low[i] && pinfo->Close[i - 1]>pinfo->High[i])
			{
				//第二天的收盘价大于第一天最高价，开盘价大于第一天最低价
				return 1;
			}
		}
		//曙光初现
		if (pinfo->Open[i] > pinfo->Close[i] && pinfo->Open[i - 1] < pinfo->Close[i - 1])
		{
			//第一天跌，第二天涨
			if (pinfo->Close[i - 1] > (pinfo->Open[i] + pinfo->Close[i]) / 2.0 && pinfo->Open[i - 1] > pinfo->Close[i])
			{
				//第二天跳空低开，收盘价在第一天1/2之上
				return 1;
			}
		}
		//好友反攻
		if (pinfo->Open[i] > pinfo->Close[i] && pinfo->Open[i - 1] < pinfo->Close[i - 1])
		{
			//第一天跌，第二天涨
			if (pinfo->Close[i - 1] < pinfo->Close[i])
			{
				//第二天收盘价在第一天收盘价下
				return 1;
			}
		}
	}
	return 0;
}


int K2_emptysuppress(PRICEINFO* pinfo)//双根K线——空方压制
{
	int i;
	for (i = 8; i > 0; i--)
	{
		//倾盆大雨
		if (pinfo->Open[i] < pinfo->Close[i] && pinfo->Open[i - 1] > pinfo->Close[i - 1])
		{
			//两天内先阳后阴
			if (pinfo->Open[i] > pinfo->Open[i - 1])
			{
				//第二天大幅低开，并股价下拉，重大利空
				return 1;
			}
		}
		//穿头破脚
		if (pinfo->Open[i] < pinfo->Close[i] && pinfo->Open[i - 1] > pinfo->Close[i - 1])
		{
			//两天内先阳后阴
			if (pinfo->Open[i - 1]<pinfo->High[i] && pinfo->Close[i - 1]>pinfo->Low[i])
			{
				//第二天阴线反向覆盖第一天阳线
				return 1;
			}
		}
		//乌云盖顶
		if (pinfo->Open[i] < pinfo->Close[i] && pinfo->Open[i - 1] > pinfo->Close[i - 1])
		{
			//两天内先阳后阴
			if (pinfo->Close[i - 1] < (pinfo->Open[i] + pinfo->Close[i]) / 2.0 && pinfo->Open[i - 1] > pinfo->Close[i])
			{
				//第二天跳空高开，收盘价在第一天1/2之下
				return 1;
			}
		}
		//淡友反攻
		if (pinfo->Open[i] < pinfo->Close[i] && pinfo->Open[i - 1] > pinfo->Close[i - 1])
		{
			//两天内先阳后阴
			if (pinfo->Close[i - 1] > pinfo->Close[i])
			{
				//第二天收盘价在第一天收盘价上
				return 1;
			}
		}
	}
	return 0;
}

int K2_trendpause(PRICEINFO* pinfo)//双根K线——走势停顿
{
	int i;
	for (i = 8; i > 0; i--)
	{
		//孕线
		if (pinfo->High[i - 1] > pinfo->High[i] && pinfo->Low[i - 1] > pinfo->Low[i])
		{
			return 1;
		}
		//肩线
		if (pinfo->Low[i - 1] > pinfo->High[i] || pinfo->High[i - 1] < pinfo->Low[i])
		{
			return 1;
		}
	}
	return 0;
}


/********************
三K线指标：3个
*********************/

/**************************************************************
FUNCTION:K3_tribleredline,K3_triblegreenline,K3_islandreverse
DESCRIPTION：三连阳
			三连阴
			岛形反转
INPUT:pinfo
RETURN:无
****************************************************************/
int K3_tribleredline(PRICEINFO* pinfo)//三根K线——三连阳
{
	float medium1, medium2, medium3;
	medium1 = (pinfo->Open[3] + pinfo->Close[3]) / 2.0;//第三天
	medium2 = (pinfo->Open[2] + pinfo->Close[2]) / 2.0;//第二天
	medium3 = (pinfo->Open[1] + pinfo->Close[1]) / 2.0;//第一天
	if (medium1 < medium2 && medium2 < medium3)
	{
		return 1;
	}
	return 0;
}

int K3_triblegreenline(PRICEINFO* pinfo)//三根K线——三连阴
{
	float medium1, medium2, medium3;
	medium1 = (pinfo->Open[3] + pinfo->Close[3]) / 2.0;//第三天
	medium2 = (pinfo->Open[2] + pinfo->Close[2]) / 2.0;//第二天
	medium3 = (pinfo->Open[1] + pinfo->Close[1]) / 2.0;//第一天
	if (medium1 > medium2 && medium2 > medium3)
	{
		return 1;
	}
	return 0;
}

int K3_islandreverse(PRICEINFO* pinfo)//三根K线——岛形反转
{
	if (pinfo->Low[3] > pinfo->High[2] && pinfo->High[2] < pinfo->Low[1])
	{
		//表示走势的转折，之后可能会有连续上涨形态
		return 1;
	}
	return 0;
}

/********************
多K线指标：2个
*********************/

/**************************************************************
FUNCTION:KN_bathbeauty,KN_fodderchoppor
DESCRIPTION：出水芙蓉
			断头铡刀
INPUT:pinfo
RETURN:无
****************************************************************/
int KN_bathbeauty(PRICEINFO* pinfo)//多根K线——出水芙蓉
{
	int i;
	for (i = 20; i > 0; i--)
	{
		if ((pinfo->Open[i - 1] + pinfo->Close[i - 1]) / 2.0 > (pinfo->Open[i] + pinfo->Close[i]) / 2.0)
		{
			//如同水中盛开莲花一般拔地而起
			continue;
		}
		else
		{
			return 0;
		}
	}
	return 1;//股票整体大涨
}

int KN_fodderchoppor(PRICEINFO* pinfo)//多根K线——断头铡刀
{
	int i;
	for (i = 20; i > 0; i--)
	{
		if ((pinfo->Open[i - 1] + pinfo->Close[i - 1]) / 2.0 < (pinfo->Open[i] + pinfo->Close[i]) / 2.0)
		{
			//如同铡刀一样斩断多方的美梦
			continue;
		}
		else
		{
			return 0;
		}
	}
	return 1;//股票整体大跌
}



/********************
均线指标：6个
*********************/

/**************************************************************
FUNCTION:MA_manyarray,MA_emptyarray,MA_bond,MA_divergence,
		MA_turnup,MA_turndown
DESCRIPTION：多头排列
			空头排列
			粘合
			发散
			拐头向上
			拐头向下
INPUT:pinfo
RETURN:无
****************************************************************/
int MA_manyarray(float* Close)//均线——多头排列
{
	int i;
	float MA[7][4] = { 0 };//7天，四个均线值
	get_7MA(MA, Close);//获得7天里的均线值,(MA5,MA10,MA20,MA60)
	//MA5 > MA10 > MA20 > MA60，维持时间3天作为多头排列的定义
	for (i = 0; i < 3; i++)
	{
		if (MA[i][0] > MA[i][1] && MA[i][1] > MA[i][2] && MA[i][2] > MA[i][3]);
		else
		{
			return 0;
		}

		//保持上升趋势
		if (i != 2)
		{
			if (MA[i][0] > MA[i + 1][0] && MA[i][1] > MA[i + 1][1]
				&& MA[i][2] > MA[i + 1][2] && MA[i][3] > MA[i + 1][3])
			{
				continue;
			}
			else
			{
				return 0;
			}
		}
	}
	return 1;
}

int MA_emptyarray(float* Close)//均线——空头排列
{
	int i;
	float MA[7][4] = { 0 };		//7天，四个均线值
	get_7MA(MA, Close);			//获得7天里的均线值,(MA5,MA10,MA20,MA60)
	//MA5 < MA10 < MA20 < MA60，维持时间3天作为空头排列的定义
	for (i = 0; i < 3; i++)
	{
		if (MA[i][0] < MA[i][1] && MA[i][1] < MA[i][2] && MA[i][2] < MA[i][3]);
		else
		{
			return 0;
		}

		//保持下降趋势
		if (i != 2)
		{
			if (MA[i][0] < MA[i + 1][0] && MA[i][1] < MA[i + 1][1]
				&& MA[i][2] < MA[i + 1][2] && MA[i][3] < MA[i + 1][3]);
			else
			{
				return 0;
			}
		}
	}
	return 1;
}

int MA_bond(float* Close)//均线——粘合
{
	int i, j;
	float max, min;
	float MA[7][4] = { 0 };//7天，四个均线值
	get_7MA(MA, Close);//获得7天里的均线值,(MA5,MA10,MA20,MA60)
	//t1为最大MA,t2为最小MA,t1/t2<1.01
	for (i = 0; i < 7; i++)
	{
		min = MA[i][0];
		max = MA[i][0];
		for (j = 1; j < 4; j++)
		{
			if (MA[i][j] > max)
			{
				max = MA[i][j];
			}

			if (MA[i][j] < min)
			{
				min = MA[i][j];
			}
		}
	}
	if (((max / min - 1) * 100) < 1)
	{
		return 1;//粘合
	}

	return 0;
}

int MA_divergence(float* Close)//均线——发散
{
	int i, j;
	float MA[7][4] = { 0 };//7天，四个均线值
	get_7MA(MA, Close);//获得7天里的均线值,(MA5,MA10,MA20,MA60)
	//MA5>MA10>MA20>MA60
	for (i = 0; i < 3; i++)
	{
		if (MA[i][0] > MA[i][1] && MA[i][1] > MA[i][2] && MA[i][2] > MA[i][3])
		{
			continue;
		}
		else
		{
			return 0;
		}
	}
	return 1;//发散
}

int MA_turnup(float* Close)//均线——拐头向上
{
	int i, j;
	float MA[7][4] = { 0 };//7天，四个均线值
	get_7MA(MA, Close);//获得7天里的均线值,(MA5,MA10,MA20,MA60)
	for (i = 0; i < 3; i++)
	{
		if (i != 2)
		{
			if (MA[i][0] > MA[i + 1][0] && MA[i][1] > MA[i + 1][1]
				&& MA[i][2] > MA[i + 1][2] && MA[i][3] > MA[i + 1][3])//四种MA上涨
			{
				continue;
			}
			else
			{
				return 0;
			}
		}
	}
	return 1;
}

int MA_turndown(float* Close)//均线——拐头向下
{
	int i, j;
	float MA[7][4] = { 0 };//7天，四个均线值
	get_7MA(MA, Close);//获得7天里的均线值,(MA5,MA10,MA20,MA60)
	for (i = 0; i < 3; i++)
	{
		if (i != 2)
		{
			if (MA[i][0] < MA[i + 1][0] && MA[i][1] < MA[i + 1][1]
				&& MA[i][2] < MA[i + 1][2] && MA[i][3] < MA[i + 1][3])//四种MA下落
			{
				continue;
			}
			else
			{
				return 0;
			}
		}
	}
	return 1;
}


/**************************************************************
FUNCTION:get_7MA
DESCRIPTION：获得7天里的4种均线
INPUT:MA,close
RETURN:无
****************************************************************/
void get_7MA(float MA[][4], float* close)
{
	int i, j;
	for (i = 0; i < 7; i++)
	{
		for (j = 0; j < 5; j++)
		{
			MA[i][0] += close[i + j];
		}
		MA[i][0] /= 5.0;//计算五日均值
		for (j = 0; j < 10; j++)
		{
			MA[i][1] += close[i + j];
		}
		MA[i][1] /= 10.0;//计算十日均值
		for (j = 0; j < 20; j++)
		{
			MA[i][2] += close[i + j];
		}
		MA[i][2] /= 20.0;//计算二十日均值
		for (j = 0; j < 60; j++)
		{
			MA[i][3] += close[i + j];
		}
		MA[i][3] /= 60.0;//计算六十日均值
	}
}



/********************
KDJ指标：4个
*********************/

/**************************************************************
FUNCTION:KDJ_goldencross,KDJ_overbuy,KDJ_deadcross,KDJ_oversell
DESCRIPTION：金叉
			超买
			死叉
			超卖
INPUT:pinfo
RETURN:无
****************************************************************/
int KDJ_goldencross(PRICEINFO* pinfo)//随机指标——死叉
{
	int i;
	float K[7] = { 0 };
	float D[7] = { 0 };
	float J[7] = { 0 };
	get_KDJ(pinfo, K, D, J);
	//下跌趋势中，K大于D，K线向下跌破D线时，为卖出信号
	for (i = 7; i > 0; i--)
	{
		if (K[i] < K[i - 1])//下跌了
		{
			return 0;
		}
	}
	if (K[0] > D[0] && K[6] < D[6])
	{
		return 1;//交叉形成死叉
	}
	return 0;
}


int KDJ_overbuy(PRICEINFO* pinfo)//随机指标——超买
{
	int i;
	float K[7] = { 0 };
	float D[7] = { 0 };
	float J[7] = { 0 };
	get_KDJ(pinfo, K, D, J);
	for (i = 0; i < 7; i++)
	{
		if (J[i] > 100)
		{
			return 1;//J大于100为超买
		}
	}
	return 0;
}

int KDJ_deadcross(PRICEINFO* pinfo)//随机指标——死叉
{
	int i;
	float K[7] = { 0 };
	float D[7] = { 0 };
	float J[7] = { 0 };
	get_KDJ(pinfo, K, D, J);
	//下跌趋势中，K大于D，K线向下跌破D线时，为卖出信号
	for (i = 7; i > 0; i--)
	{
		if (K[i] < K[i - 1])//上涨了
		{
			return 0;
		}
	}
	if (K[0] < D[0] && K[6] > D[6])
	{
		return 1;//交叉形成死叉
	}
	return 0;
}

int KDJ_oversell(PRICEINFO* pinfo)//随机指标——超卖
{
	int i;
	float K[7] = { 0 };
	float D[7] = { 0 };
	float J[7] = { 0 };
	get_KDJ(pinfo, K, D, J);
	for (i = 0; i < 7; i++)
	{
		if (J[i] < 0)
		{
			return 1;//J小于0为超卖
		}
	}
	return 0;
}

/**************************************************************
FUNCTION:get_KDJ
DESCRIPTION：获得7天KDJ的值
INPUT:pinfo,k,d,j
RETURN:无
****************************************************************/
void get_KDJ(PRICEINFO* pinfo, float* k, float* d, float* j)
{
	int i, m;
	float RSV = 0.0, closemax, closemin;//max,min为9日内
	float K[20] = { 0.0 };
	float D[20] = { 0.0 };
	float J[20] = { 0.0 };
	K[20] = 50.0;
	D[20] = 50.0;//若无前一日K值与D值，则可分别用50来代替
	for (i = 19; i >= 0; i--)
	{
		//n日RSV=（Cn－Ln）/（Hn－Ln）×100;
		//Cn为第n日收盘价；Ln为n日内的最低价；Hn为n日内的最高价;
		closemin = pinfo->Close[i];
		closemax = pinfo->Close[i];
		for (m = 0; m < 9; m++)//以9日为周期的KD线
		{
			if (pinfo->Close[i + m] > closemax)
			{
				closemax = pinfo->Close[i + m];
			}
			if (pinfo->Close[i + m] < closemin)
			{
				closemin = pinfo->Close[i + m];
			}
		}
		RSV = 100 * (pinfo->Close[i] - closemin) / (closemax - closemin);
		K[i] = K[i + 1] * 0.67 + RSV * 0.33;//当日K值=2/3×前一日K值+1/3×当日RSV
		D[i] = D[i + 1] * 0.67 + K[i] * 0.33;//当日D值=2/3×前一日D值+1/3×当日K值
		J[i] = 3 * K[i + 1] - 2 * D[i + 1];//当日J值=3*当日K值-2*当日D值
	}
	for (i = 0; i < 7; i++)
	{
		k[i] = K[i];
		d[i] = D[i];
		j[i] = J[i];//七天内指标复制
	}
}


/********************
MACD指标：4个
*********************/
int MACD_updevide(PRICEINFO* pinfo)//异同移动平均线——顶背离
{
	float EMA12[8] = { 0.0 };
	float EMA26[8] = { 0.0 };
	float DIFF[8] = { 0.0 };
	float DEA[8] = { 0.0 };
	get_MACD(pinfo, EMA12, EMA26, DIFF, DEA);
	/*右边的股价比左边峰值的股价更高，
	而此时DIF或MACD不如左边峰值时间的值更高的时候，
	我们说此时就是MACD的顶背离。*/
	if (pinfo->Close[0] < pinfo->Close[1] && pinfo->Close[1] < pinfo->Close[2]);
	else
	{
		return 0;
	}
	if (DIFF[0] > DIFF[1] && DIFF[1] > DIFF[2] && DIFF[2] > DIFF[3]
		&& DEA[0] > DEA[1] && DEA[1] > DEA[2] && DEA[2] > DEA[3])
	{
		return 1;
	}
	return 0;

}

int MACD_goldencross(PRICEINFO* pinfo)//异同移动平均线——金叉
{
	int i;
	float EMA12[8] = { 0 };
	float EMA26[8] = { 0 };
	float DIFF[8] = { 0 };
	float DEA[8] = { 0 };
	get_MACD(pinfo, EMA12, EMA26, DIFF, DEA);
	//在上涨趋势中，DIF线上穿DEA线(白线上穿黄线)后,形成MACD金叉形态
	for (i = 7; i > 0; i--)
	{
		if (DIFF[i] > DIFF[i - 1])//下跌了
		{
			return 0;
		}
	}
	if (DIFF[0] < DEA[0] && DIFF[6] > DEA[6])
	{
		return 1;//交叉形成死叉
	}
	return 0;
}

int MACD_downdevide(PRICEINFO* pinfo)//异同移动平均线——底背离
{
	float EMA12[8] = { 0 };
	float EMA26[8] = { 0 };
	float DIFF[8] = { 0 };
	float DEA[8] = { 0 };
	get_MACD(pinfo, EMA12, EMA26, DIFF, DEA);
	/*当右侧的股价比左侧的低谷还低时，DIF或者MACD不如当初的值低，这就是MACD底背离*/
	if (pinfo->Close[0] < pinfo->Close[1] && pinfo->Close[1] < pinfo->Close[2]);
	else
	{
		return 0;
	}
	if (DIFF[0] > DIFF[1] && DIFF[1] > DIFF[2] && DIFF[2] > DIFF[3]
		&& DEA[0] > DEA[1] && DEA[1] > DEA[2] && DEA[2] > DEA[3])
	{
		return 1;
	}
	return 0;
}

int MACD_deadcross(PRICEINFO* pinfo)//异同移动平均线——死叉
{
	int i;
	float EMA12[8] = { 0 };
	float EMA26[8] = { 0 };
	float DIFF[8] = { 0 };
	float DEA[8] = { 0 };
	get_MACD(pinfo, EMA12, EMA26, DIFF, DEA);
	//在下跌趋势中DIFF 由上向下突破 DEA
	for (i = 7; i > 0; i--)
	{
		if (DIFF[i] < DIFF[i - 1])//上涨了
		{
			return 0;
		}
	}
	if (DIFF[0] > DEA[0] && DIFF[6] < DEA[6])
	{
		return 1;//交叉形成死叉
	}
	return 0;
}

/**************************************************************
FUNCTION:get_MACD
DESCRIPTION：获得7天里MACD的值
INPUT:pinfo,ema12,ema26,diff,dea
RETURN:无
****************************************************************/
void get_MACD(PRICEINFO* pinfo, float* EMA12, float* EMA26, float* DIFF, float* DEA)
{
	int i;
	EMA12[8] = pinfo->Close[8];
	EMA26[8] = pinfo->Close[8];
	for (i = 7; i >= 0; i--)
	{
		EMA12[i] = EMA12[i + 1] * 0.846 + pinfo->Close[i] * 0.154;
		//EMA（12）=前一日EMA（12）×11/13+今日收盘价×2/13	
		EMA26[i] = EMA26[i + 1] * 0.926 + pinfo->Close[i] * 0.074;
		//EMA（26）=前一日EMA（26）×25/27+今日收盘价×2/27
		DIFF[i] = EMA12[i] - EMA26[i];
		//DIF=今日EMA（12）－今日EMA（26）
		DEA[i] = DEA[i + 1] * 0.8 + DIFF[i] * 0.2;
		//今日DEA（MACD）=前一日DEA×8/10+今日DIF×2/10
	}
}


/********************
RSI指标：2个
*********************/
int RSI_overbuy(PRICEINFO* pinfo)//相对强弱指标——超买
{
	int i;
	float RSI[7] = { 0 };
	for (i = 0; i < 7; i++)
	{
		RSI[i] = get_RSI(pinfo, i, 6);
	}
	//RSI上穿80
	if (RSI[0] < 80 && RSI[6]>80)
	{
		return 1;
	}
	return 0;
}


int RSI_oversell(PRICEINFO* pinfo)//相对强弱指标——超卖
{
	int i;
	float RSI[7] = { 0 };
	for (i = 0; i < 7; i++)
	{
		RSI[i] = get_RSI(pinfo, i, 6);
	}
	//RSI下穿20
	if (RSI[0] > 20 && RSI[6] < 20)
	{
		return 1;
	}
	return 0;
}

/**************************************************************
FUNCTION:get_RSI
DESCRIPTION：获得7天里RSI的值
INPUT:pinfo,date,days
RETURN:无
****************************************************************/
float get_RSI(PRICEINFO* pinfo, int date, int days)//获得7天里的RSI，RSI有6,12,24的区分
{
	int i;
	float RSI = 0.0;//相对强弱指标RSI
	float sum_rise = 0.0;//涨数和
	float sum_fall = 0.0;//跌数和
	float difference;//两天收盘价的差值
	for (i = 0; i < days; i++)
	{
		difference = pinfo[date + i + 1].Close - pinfo[date + i].Close;//前一天-后一天(数据库内数据相反)

		if (difference >= 0)
		{
			sum_fall += difference;
		}
		else if (difference < 0)
		{
			sum_rise += (-difference);
		}
	}
	RSI = 100 * sum_rise / (sum_rise + sum_fall);
	//RS(相对强度)=N日内收盘价涨数和之均值÷N日内收盘价跌数和之均值
	//RSI(相对强弱指标)=100－100÷(1+RS)
	return RSI;
}


/********************
VOL/OBV指标：2个
*********************/
int VOLOBV_trendup(PRICEINFO* pinfo)//成交量/能量潮——上涨趋势
{
	int i;
	float OBV[7] = { 0 };
	get_OBV(pinfo, OBV);
	for (i = 7; i > 0; i--)
	{
		if (OBV[i] < OBV[i - 1]);//上涨
		else
		{
			return 0;
		}
	}
	return 1;
}

int VOLOBV_trenddown(PRICEINFO* pinfo)//成交量/能量潮——下降趋势
{
	int i;
	float OBV[7] = { 0 };
	get_OBV(pinfo, OBV);
	for (i = 7; i > 0; i--)
	{
		if (OBV[i] > OBV[i - 1]);//下跌
		else
		{
			return 0;
		}
	}
	return 1;
}

/**************************************************************
FUNCTION:get_OBV
DESCRIPTION：获得7天里OBV
INPUT:pinfo,obv
RETURN:无
****************************************************************/
void get_OBV(PRICEINFO* pinfo, float* obv)//获得7天里的能量潮
{
	int i;
	float OBV = 0.0;
	/*以某日为基期，逐日累计每日上市股票总成交量，
	若隔日指数或股票上涨，则基期OBV加上本日成交量为本日OBV。
	隔日指数或股票下跌，则基期OBV减去本日成交量为本日OBV。
	一般来说，只是观察OBV的升降并无多大意义，必须配合K线图的走势才有实际的效用。*/
	for (i = 0; i < 7; i++)
	{
		if (pinfo->Close[i + 1] - pinfo->Close[i] >= 0)//下跌
		{
			OBV += pinfo->VOL[i];
		}
		else if (pinfo->Close[i + 1] - pinfo->Close[i] < 0)
		{
			OBV -= pinfo->VOL[i];
		}
		obv[i] = OBV;
	}
}


/********************
BIAS指标：2个
*********************/
int BIAS_overbuy(PRICEINFO* pinfo)//乖离率——超买
{
	int i;
	float BIAS6[7] = { 0 };
	float BIAS12[7] = { 0 };
	float BIAS24[7] = { 0 };
	get_BIAS(pinfo, BIAS6, BIAS12, BIAS24);
	//BIAS值超过15
	for (i = 0; i < 7; i++)
	{
		if (BIAS6[i] > 15)
		{
			return 1;
		}
		if (BIAS12[i] > 15)
		{
			return 1;
		}
		if (BIAS24[i] > 15)
		{
			return 1;
		}
	}
	return 0;
}

int BIAS_oversell(PRICEINFO* pinfo)//乖离率——超卖
{
	int i;
	float BIAS6[7] = { 0 };
	float BIAS12[7] = { 0 };
	float BIAS24[7] = { 0 };
	get_BIAS(pinfo, BIAS6, BIAS12, BIAS24);
	//BIAS值超过15
	for (i = 0; i < 7; i++)
	{
		if (BIAS6[i] < -15)
		{
			return 1;
		}
		if (BIAS12[i] < -15)
		{
			return 1;
		}
		if (BIAS24[i] < -15)
		{
			return 1;
		}
	}
	return 0;
}

/**************************************************************
FUNCTION:get_BIAS
DESCRIPTION：获得7天里的乖离率
INPUT:pinfo,bias6,bias12,bias24
RETURN:无
****************************************************************/
void get_BIAS(PRICEINFO* pinfo, float* bias6, float* bias12, float* bias24)//获得7天的乖离率
{
	int i, j;
	float MA6 = 0.0;
	float MA12 = 0.0;
	float MA24 = 0.0;
	float BIAS6[20] = { 0 };
	float BIAS12[20] = { 0 };
	float BIAS24[20] = { 0 };
	//乖离率=[(当日收盘价-N日平均价)/N日平均价]*100%
	//其中N，一般5、6、10、12、24、30和72。在实际运用中，短线使用6日乖离率较为有效，中线则放大为10日或12日。
	for (i = 0; i < 20; i++)
	{
		MA6 = 0.0;
		MA12 = 0.0;
		MA24 = 0.0;
		for (j = 0; j < 6; j++)
		{
			MA6 += pinfo->Close[i + j];
		}
		MA6 /= 6.0;
		BIAS6[i] = 100 * (pinfo->Close[i] - MA6) / MA6;//BIAS6

		for (j = 0; j < 12; j++)
		{
			MA12 += pinfo->Close[i + j];
		}
		MA12 /= 12.0;
		BIAS12[i] = 100 * (pinfo->Close[i] - MA12) / MA12;//BIAS12

		for (j = 0; j < 24; j++)
		{
			MA24 += pinfo->Close[i + j];
		}
		MA24 /= 24.0;
		BIAS24[i] = 100 * (pinfo->Close[i] - MA24) / MA24;//BIAS24
	}
	for (i = 0; i < 7; i++)
	{
		bias6[i] = BIAS6[i];
		bias12[i] = BIAS12[i];
		bias24[i] = BIAS24[i];
	}
}

/********************
BOLL指标：3个
*********************/
int BOLL_breaktoprail(PRICEINFO* pinfo)//布林带——突破上轨
{
	float uprail[7] = { 0 };
	float midrail[7] = { 0 };
	float downrail[7] = { 0 };
	get_BOLL(pinfo, uprail, midrail, downrail);
	if (uprail[6] < midrail[0]
		&& uprail[6] < pinfo->Close[6]
		&& uprail[0] > pinfo->Close[0])
	{
		return 1;
	}

	return 0;
}

int BOLL_breakmediumrail(PRICEINFO* pinfo)//布林带——突破中轨
{
	float uprail[7] = { 0 };
	float midrail[7] = { 0 };
	float downrail[7] = { 0 };
	get_BOLL(pinfo, uprail, midrail, downrail);
	if (midrail[6] < midrail[0]
		&& midrail[6] < pinfo->Close[6]
		&& midrail[0] > pinfo->Close[0])
	{
		return 1;
	}

	return 0;
}

int BOLL_breakbottomrail(PRICEINFO* pinfo)//布林带——突破下轨
{
	float uprail[7] = { 0 };
	float midrail[7] = { 0 };
	float downrail[7] = { 0 };
	get_BOLL(pinfo, uprail, midrail, downrail);
	if (downrail[6] < midrail[0]
		&& downrail[6] < pinfo->Close[6]
		&& downrail[0] > pinfo->Close[0])
	{
		return 1;
	}

	return 0;
}

/**************************************************************
FUNCTION:get_BOLL
DESCRIPTION：获得7天里的布林带
INPUT:pinfo,uprail,midrail,downrail
RETURN:无
****************************************************************/
void get_BOLL(PRICEINFO* pinfo, float* uprail, float* midrail, float* downrail)//获得7天的布林带
{
	int i, j;
	float std_dev = 0.0;
	for (i = 0; i < 7; i++)
	{
		for (j = 0; j < 20; j++)
		{
			midrail[i] += pinfo->Close[i + j];
		}
		midrail[i] /= 20.0;//中轨线=N日的移动平均线，布林线日指标N一般为20
		std_dev = get_std_deviation(pinfo, 20, i);//计算从i日开始20天内的方差
		uprail[i] = midrail[i] + 2 * std_dev;//上轨线（up线）=中轨线+两倍的标准差（2SD）
		downrail[i] = midrail[i] - 2 * std_dev;//下轨线（down线）=中轨线-两倍的标准差（2SD）
	}
}

/**************************************************************
FUNCTION:get_std_deviation
DESCRIPTION：计算标准差
INPUT:pinfo,days,date
RETURN:无
****************************************************************/
float get_std_deviation(PRICEINFO* pinfo, int days, int date)//计算标准差,days为20.0
{
	int i;
	float aver = 0.0;//20日平均值
	float variance = 0.0;//方差
	float std_dev = 0.0;//标准差
	for (i = 0; i < days; i++)
	{
		aver += pinfo->Close[date + i];
	}
	aver /= days * 1.0;//计算均值
	for (i = 0; i < days; i++)
	{
		variance += (pinfo->Close[date + i] - aver) * (pinfo->Close[date + i] - aver);
	}
	variance /= days * 1.0;//计算方差
	std_dev = sqrt(variance);//计算标准差

	return std_dev;
}