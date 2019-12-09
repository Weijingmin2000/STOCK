#include"public.h"
#include"database.h"
#include"tsearch.h"
#include"storead.h"
#include"tcalcu.h"

/*****************************************
tsearch.c
COPYRIGHT:	weijingmin
FUNCTION:	control function for search
ABSTRACT:
		A.abstract information to pinfo
		B.allocation
		C.judge and search
DATE:2019/10/26
******************************************/

/*********************************************
FUNCTION:T_search
DESCRIPTION：技术面选股搜索函数
INPUT:opt[][6],sto_list,progress_color
RETURN:无
***********************************************/
void T_search(int opt[][6], int* sto_list, int progress_color)
{
	PRICEINFO* pinfo;//90天价格信息的指针，作为一个中间变量，每次搜索后全部重置
	int i;
	int cnt = 0;//找到了符合条件的多少支股票的整形变量
	/*for (i = 0; i < 200; i++)
	{
		sto_list[i] = 0;
	}*/
	memset(sto_list, 0, 200*sizeof(int));
	if ((pinfo = (PRICEINFO*)malloc(sizeof(PRICEINFO))) == 0)
	{
		printf("memoryallocation runs wrong in tsearch.c");
		delay(3000);
		exit(1);
	}
	memset(pinfo, 0, sizeof(PRICEINFO));//初始化结构体信息
	setlinestyle(SOLID_LINE, 0, 1);
	setcolor(WHITE);
	rectangle(320, 462, 524, 479);
	setfillstyle(SOLID_FILL, LIGHTGRAY);
	bar(322, 464, 522, 477);
	for (i = 0; i < 200; i++)
	{
		sto_90days2float(pinfo, i + 1);//循环读取200支股票的90天数据，并写入pinfo
		if (techsearch(opt, pinfo) == 1)
		{
			//printf("find");//调试
			sto_list[cnt] = i + 1;
			cnt++;
		}
		else
		{
			//printf("not find");//调试
		}
		memset(pinfo, 0, sizeof(PRICEINFO));
		refresh_progress(i, progress_color);
	}
	if (pinfo != NULL)
	{
		free(pinfo);
		pinfo = NULL;
	}
}

/*********************************************
FUNCTION:refresh_progress
DESCRIPTION：更新进度条
INPUT:progress,color
RETURN:无
***********************************************/
void refresh_progress(int progress,int color)
{
	setfillstyle(SOLID_FILL, color);
	bar(322, 464, 322 + progress, 477);
}

/*********************************************
FUNCTION:sto_90days2float
DESCRIPTION：读取一只股票
	90天的Open,Close,High,Low,VOL信息
INPUT:pinfo,sto_num
RETURN:无
***********************************************/
void sto_90days2float(PRICEINFO* pinfo, int sto_num)
{
	int i;
	STODAYFLOAT info[90];//与market的操作类似
	memset(info, 0, 90 * sizeof(STODAYFLOAT));
	sto_daybyday2float(sto_num, 1, 90, info);//读取90天信息并写入info
	for (i = 0; i < 90; i++)
	{
		pinfo->Open[i] = info[i].Open;
		pinfo->Close[i] = info[i].Close;
		pinfo->High[i] = info[i].High;
		pinfo->Low[i] = info[i].Low;
		pinfo->VOL[i] = info[i].VOL;//给价格结构赋值
	}
}


/*********************************************
FUNCTION:techsearch
DESCRIPTION：技术面搜索
INPUT:opt[][6],pinfo
RETURN:0/1
***********************************************/
int techsearch(int opt[][6], PRICEINFO* pinfo)
{
	//单根K线
	if (opt[0][0] == 1)//一字涨停板
	{
		if (K_onelimitup(pinfo) == 0)//不满足
		{
			return 0;
		}
	}
	else if (opt[0][1] == 1)//长上影线
	{
		if (K_longhatch(pinfo) == 0)
		{
			return 0;
		}
	}
	else if (opt[0][2] == 1)//大阳线
	{
		if (K_greatredline(pinfo) == 0)
		{
			return 0;
		}
	}


	//双根K线
	if (opt[1][0] == 1)//多方压制
	{
		if (K2_manysuppress(pinfo) == 0)
		{
			return 0;
		}
	}
	else if (opt[1][1] == 1)//空方压制
	{
		if (K2_emptysuppress(pinfo) == 0)
		{
			return 0;
		}
	}
	else if (opt[1][2] == 1)//走势停顿
	{
		if (K2_trendpause(pinfo) == 0)
		{
			return 0;
		}
	}


	//三根K线
	if (opt[2][0] == 1)//三连阳
	{
		if (K3_tribleredline(pinfo) == 0)
		{
			return 0;
		}
	}
	else if (opt[2][1] == 1)//三连阴
	{
		if (K3_triblegreenline(pinfo) == 0)
		{
			return 0;
		}
	}
	else if (opt[2][2] == 1)//岛形反转
	{
		if (K3_islandreverse(pinfo) == 0)
		{
			return 0;
		}
	}


	//多根K线
	if (opt[3][0] == 1)//出水芙蓉
	{
		if (KN_bathbeauty(pinfo) == 0)
		{
			return 0;
		}
	}
	else if (opt[3][1] == 1)//断头铡刀
	{
		if (KN_fodderchoppor(pinfo) == 0)
		{
			return 0;
		}
	}


	//MA形态选股
	if (opt[4][0] == 1)//多头排列
	{
		if (MA_manyarray(pinfo->Close) == 0)
		{
			return 0;
		}
	}
	else if (opt[4][1] == 1)//空头排列
	{
		if (MA_emptyarray(pinfo->Close) == 0)
		{
			return 0;
		}
	}
	else if (opt[4][2] == 1)//粘合
	{
		if (MA_bond(pinfo->Close) == 0)
		{
			return 0;
		}
	}
	else if (opt[4][3] == 1)//发散
	{
		if (MA_divergence(pinfo->Close) == 0)
		{
			return 0;
		}
	}
	else if (opt[4][4] == 1)//拐头向上
	{
		if (MA_turnup(pinfo->Close) == 0)
		{
			return 0;
		}
	}
	else if (opt[4][5] == 1)//拐头向下
	{
		if (MA_turndown(pinfo->Close) == 0)
		{
			return 0;
		}
	}



	//随机指标KDJ
	if (opt[5][0] == 1)//KD金叉
	{
		if (KDJ_goldencross(pinfo) == 0)
		{
			return 0;
		}
	}
	else if (opt[5][1] == 1)//J超买
	{
		if (KDJ_overbuy(pinfo) == 0)
		{
			return 0;
		}
	}
	else if (opt[5][2] == 1)//KD死叉
	{
		if (KDJ_deadcross(pinfo) == 0)
		{
			return 0;
		}
	}
	else if (opt[5][3] == 1)//J超卖
	{
		if (KDJ_oversell(pinfo) == 0)
		{
			return 0;
		}
	}


	//异同移动平均线MACD
	if (opt[6][0] == 1)//顶背离
	{
		if (MACD_updevide(pinfo) == 0)
		{
			return 0;
		}
	}
	else if (opt[6][1] == 1)//金叉
	{
		if (MACD_goldencross(pinfo) == 0)
		{
			return 0;
		}
	}
	else if (opt[6][2] == 1)//底背离
	{
		if (MACD_downdevide(pinfo) == 0)
		{
			return 0;
		}
	}
	else if (opt[6][3] == 1)//死叉
	{
		if (MACD_deadcross(pinfo) == 0)
		{
			return 0;
		}
	}


	//相对强弱指数RSI
	if (opt[7][0] == 1)//超买(>80)
	{
		if (RSI_overbuy(pinfo) == 0)
		{
			return 0;
		}
	}
	else if (opt[7][1] == 1)//超卖(<80)
	{
		if (RSI_oversell(pinfo) == 0)
		{
			return 0;
		}
	}


	//成交量/能量潮 VOL/OBV
	if (opt[8][0] == 1)//上涨趋势
	{
		if (VOLOBV_trendup(pinfo) == 0)
		{
			return 0;
		}
	}
	else if (opt[8][1] == 1)//下跌趋势
	{
		if (VOLOBV_trenddown(pinfo) == 0)
		{
			return 0;
		}
	}


	//乖离率BIAS
	if (opt[9][0] == 1)//超买(>16%)
	{
		if (BIAS_overbuy(pinfo) == 0)
		{
			return 0;
		}
	}
	else if (opt[9][1] == 1)//超卖(<-16%)
	{
		if (BIAS_oversell(pinfo) == 0)
		{
			return 0;
		}
	}


	//布林带BOLL
	if (opt[10][0] == 1)//突破上轨
	{
		if (BOLL_breaktoprail(pinfo) == 0)
		{
			return 0;
		}
	}
	else if (opt[10][1] == 1)//突破下轨
	{
		if (BOLL_breakmediumrail(pinfo) == 0)
		{
			return 0;
		}
	}
	else if (opt[10][2] == 1)//突破中轨
	{
		if (BOLL_breakbottomrail(pinfo) == 0)
		{
			return 0;
		}
	}

	return 1;//走到这一步表示这次选股所选择的一些技术指标都满足，则该股票为被选股
}