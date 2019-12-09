#include"public.h"
#include"marketfc.h"
#include"techline.h"

/*****************************************
techline.c
COPYRIGHT:	weijingmin
FUNCTION:	draw techline
ABSTRACT:
		A.control the tech index
		B.draw techline
DATE:2019/10/14
******************************************/


/*********************************************
FUNCTION:majorinfo_show
DESCRIPTION：显示主图信息的总控函数
INPUT:kind,info,major,max,min
RETURN:无
***********************************************/
void majorinfo_show (int kind, STODAYFLOAT* info, UPLINE* major,float max,float min)
{
	switch (kind)
	{
	case 1:
		Kline_draw(info, max, min);
		MA_draw(info, major, max, min);
		break;
	case 2:
		Kline_draw(info, max, min);
		BOLL_draw(info, major, max, min);
		break;
	case 3:
		Kline_draw(info,max,min);
		ENE_draw(info, major, max, min);
		break;
	}
}


/*********************************************
FUNCTION:viceinfo_show
INPUT：显示副图信息的总控函数
DATA FLOW:kind,info,vice
RETURN:无
***********************************************/
void viceinfo_show(int kind, STODAYFLOAT* info, DOWNLINE* vice)
{
	switch (kind)
	{
	case 1:
		BIAS_draw(info, vice);
		break;
	case 2:
		BRAR_draw(info, vice);
		break;
	case 3:
		CR_draw(info, vice);
		break;
	case 4:
		DMA_draw(info, vice);
		break;
	case 5:
		OBV_draw(info, vice);
		break;
	case 6:
		KDJ_draw(info, vice);
		break;
	case 7:
		MACD_draw(info, vice);
		break;
	case 8:
		RSI_draw(info, vice);
		break;
	case 9:
		VOL_draw(info, vice);
	}
}

/*********************************************
FUNCTION:Kline_draw
DESCRIPTION：画K线,max,min为get_border计算的边界
INPUT:info,max,min
RETURN:无
***********************************************/
void Kline_draw(STODAYFLOAT* info, float max, float min)
{
	int i;
	float rise_fall = 0.0;//判断涨跌
	int top = 0;//上影线最高处
	int bottom = 0;//下影线最低处
	int high = 0;//K线长方形的上边线坐标
	int low = 0;//K线长方形的下边线坐标
	for (i = 0; i < 90; i++)
	{
		rise_fall = info[i].Close - info[i].Open;//涨跌计算
		if (rise_fall >= 1e-6)//股票涨时
		{
			setcolor(RED);
			setlinestyle(SOLID_LINE, 0, 1);
			high = 342 - (int)((info[i].Close - min) * 270 / (max - min));//按比例放大缩小
			low = 342 - (int)((info[i].Open - min) * 270 / (max - min));
			top = 342 - (int)((info[i].High - min) * 270 / (max - min));
			bottom = 342 - (int)((info[i].Low - min) * 270 / (max - min));
			rectangle(619 - 6 * i, high, 615 - 6 * i, low);//画K线实体部分
			line(617 - 6 * i, high, 617 - 6 * i, top);//画上影线
			line(617 - 6 * i, low, 617 - 6 * i, bottom);//画下影线
		}
		else if (rise_fall < 1e-6)//股票跌时
		{
			setcolor(LIGHTCYAN);
			setlinestyle(SOLID_LINE, 0, 1);
			setfillstyle(SOLID_FILL, LIGHTCYAN);
			high = 342 - (int)((info[i].Open - min) * 270 / (max - min));
			low = 342 - (int)((info[i].Close - min) * 270 / (max - min));
			top = 342 - (int)((info[i].High - min) * 270 / (max - min));
			bottom = 342 - (int)((info[i].Low - min) * 270 / (max - min));
			bar(619 - 6 * i, high, 615 - 6 * i, low);//画K线实体部分
			line(617 - 6 * i, bottom, 617 - 6 * i, top);//画上下影线
		}
	}
}

/*********************************************
FUNCTION:MA_draw
DESCRIPTION：绘制MA线，major为存放主图均线信息的结构体指针
INPUT:info,major,max,min
RETURN:无
***********************************************/
void MA_draw(STODAYFLOAT* info, UPLINE* major, float max, float min)
{
	int i, j;
	for (i = 0; i < 90; i++)
	{
		major->first[i] = 0.0;
		major->second[i] = 0.0;
		major->third[i] = 0.0;
		major->forth[i] = 0.0;
	}
	for (i = 0; i < 90; i++)
	{
		for (j = 0; j < 5; j++)
		{
			major->first[i] += info[i + j].Close;
		}
		major->first[i] /= 5.0;//计算五日均值
		for (j = 0; j < 10; j++)
		{
			major->second[i] += info[i + j].Close;
		}
		major->second[i] /= 10.0;//计算十日均值
		for (j = 0; j < 20; j++)
		{
			major->third[i] += info[i + j].Close;
		}
		major->third[i] /= 20.0;//计算二十日均值
		for (j = 0; j < 60; j++)
		{
			major->forth[i] += info[i + j].Close;
		}
		major->forth[i] /= 60.0;//计算六十日均值
	}

	setlinestyle(SOLID_LINE, 0, 1);
	for (i = 1; i < 90; i++)
	{
		//(MA-min)/(342-y)=(max-min)/270
		setcolor(WHITE);
		line(625 - 6 * i, 341 - (int)((major->first[i - 1] - min) * 260 / (max - min)), 619 - 6 * i, 341 - (int)((major->first[i] - min) * 260 / (max - min)));//绘制5日均线
		setcolor(YELLOW);
		line(625 - 6 * i, 341 - (int)((major->second[i - 1] - min) * 260 / (max - min)), 619 - 6 * i, 341 - (int)((major->second[i] - min) * 260 / (max - min)));//绘制10日均线
		setcolor(LIGHTMAGENTA);
		line(625 - 6 * i, 341 - (int)((major->third[i - 1] - min) * 260 / (max - min)), 619 - 6 * i, 341 - (int)((major->third[i] - min) * 260 / (max - min)));//绘制20日均线
		setcolor(GREEN);
		line(625 - 6 * i, 341 - (int)((major->forth[i - 1] - min) * 260 / (max - min)), 619 - 6 * i, 341 - (int)((major->forth[i] - min) * 260 / (max - min)));//绘制60日均线
	}
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(2, HORIZ_DIR, 4);
	setcolor(WHITE);
	outtextxy(82, 72, "M5:");
	setcolor(YELLOW);
	outtextxy(172, 72, "M10:");
	setcolor(LIGHTMAGENTA);
	outtextxy(262, 72, "M20:");
	setcolor(GREEN);
	outtextxy(352, 72, "M60:");

}

/*********************************************
FUNCTION:BOLL_draw
DESCRIPTION：绘制BOLL布林线指标
INPUT:info,major,max,min
RETURN:无
***********************************************/
void BOLL_draw(STODAYFLOAT* info, UPLINE* major, float max, float min)
{
	int i,j;
	float std_dev = 0.0;
	for (i = 0; i < 90; i++)
	{
		major->first[i] = 0.0;
		major->second[i] = 0.0;
		major->third[i] = 0.0;
	}
	for (i = 0; i < 90; i++)
	{
		for (j = 0; j < 20; j++)
		{
			major->second[i] += info[i + j].Close;
		}
		major->second[i] /= 20.0;//中轨线=N日的移动平均线，布林线日指标N一般为20
		std_dev = std_deviation(info, 20, i);//计算从i日开始20天内的方差
		major->first[i] = major->second[i] + 2 * std_dev;//上轨线（up线）=中轨线+两倍的标准差（2SD）
		major->third[i] = major->second[i] - 2 * std_dev;//下轨线（down线）=中轨线-两倍的标准差（2SD）
	}

	setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
	for (i = 1; i < 90; i++)
	{
		setcolor(WHITE);
		line(625 - 6 * i, 341 - (int)((major->first[i - 1] - min) * 260 / (max - min)), 619 - 6 * i, 341 - (int)((major->first[i] - min) * 260 / (max - min)));//绘制布林线上轨
		setcolor(YELLOW);
		line(625 - 6 * i, 341 - (int)((major->second[i - 1] - min) * 260 / (max - min)), 619 - 6 * i, 341 - (int)((major->second[i] - min) * 260 / (max - min)));//绘制布林线中轨
		setcolor(LIGHTMAGENTA);
		line(625 - 6 * i, 341 - (int)((major->third[i - 1] - min) * 260 / (max - min)), 619 - 6 * i, 341 - (int)((major->third[i] - min) * 260 / (max - min)));//绘制布林线下轨
	}
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(2, HORIZ_DIR, 4);
	setcolor(WHITE);
	outtextxy(82, 72, "UP:");
	setcolor(YELLOW);
	outtextxy(172, 72, "MB:");
	setcolor(LIGHTMAGENTA);
	outtextxy(262, 72, "DN:");
}

/*********************************************
FUNCTION:ENE_draw
DESCRIPTION：绘制ENE轨道线指标
INPUT:info,major,max,min
RETURN:无
***********************************************/
void ENE_draw(STODAYFLOAT* info, UPLINE* major, float max, float min)
{
	int i, j;
	float MA10 = 0.0;
	for (i = 0; i < 90; i++)
	{
		major->first[i] = 0.0;
		major->second[i] = 0.0;
		major->third[i] = 0.0;
	}
	for (i = 0; i < 90; i++)
	{
		MA10 = 0.0;
		for (j = 0; j < 10; j++)
		{
			MA10 += info[i + j].Close;
		}
		MA10 /= 10.0;
		major->first[i] = (1 + 11.0 / 100.0) * MA10;	//ENE上轨=(1+M1/100)*收盘价的N日简单移动平均
		major->third[i] = (1 - 9.0 / 100.0) * MA10;	//ENE下轨=(1-M2/100)*收盘价的N日简单移动平均
		major->second[i] = (major->first[i] + major->third[i]) / 2.0;	//ENE中轨=(ENE上轨+ENE下轨)/2
	}

	setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
	for (i = 1; i < 90; i++)
	{
		setcolor(WHITE);
		line(625 - 6 * i, 341 - (int)((major->first[i - 1] - min) * 260 / (max - min)), 619 - 6 * i, 341 - (int)((major->first[i] - min) * 260 / (max - min)));//绘制ENE上轨
		setcolor(YELLOW);
		line(625 - 6 * i, 341 - (int)((major->second[i - 1] - min) * 260 / (max - min)), 619 - 6 * i, 341 - (int)((major->second[i] - min) * 260 / (max - min)));//绘制ENE中轨
		setcolor(LIGHTMAGENTA);
		line(625 - 6 * i, 341 - (int)((major->third[i - 1] - min) * 260 / (max - min)), 619 - 6 * i, 341 - (int)((major->third[i] - min) * 260 / (max - min)));//绘制ENE下轨
	}
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(2, HORIZ_DIR, 4);
	setcolor(WHITE);
	outtextxy(82, 72, "UP:");
	setcolor(YELLOW);
	outtextxy(172, 72, "ENE:");
	setcolor(LIGHTMAGENTA);
	outtextxy(262, 72, "DN:");
}

/*********************************************
FUNCTION:VOL_draw
DESCRIPTION：绘制成交量柱状图,vice为存放副图成交量信息的结构体指针
INPUT:info,vice
RETURN:无
***********************************************/
void VOL_draw(STODAYFLOAT* info, DOWNLINE* vice)
{
	int i, j;
	float rise_fall = 0.0;//涨跌
	float max = info[0].VOL;

	for (i = 0; i < 90; i++)
	{
		vice->first[i] = 0.0;
		vice->second[i] = 0.0;
		vice->third[i] = 0.0;

		if (info[i].VOL > max)
		{
			max = info[i].VOL;//计算最大值，确定副图绘制的最高点
		}
	}

	for (i = 0; i < 90; i++)
	{
		rise_fall = info[i].Close - info[i].Open;
		if (rise_fall >= 1e-6)//涨
		{
			setcolor(RED);
			setlinestyle(SOLID_LINE, 0, 1);
			//(456-y)/VOL=114/max
			rectangle(619 - 6 * i, 455, 615 - 6 * i, 455 - (int)(info[i].VOL * 110 / max));
		}
		else if (rise_fall < 1e-6)//跌
		{
			setfillstyle(SOLID_FILL, LIGHTCYAN);
			bar(619 - 6 * i, 455, 615 - 6 * i, 455 - (int)(info[i].VOL * 110 / max));
		}
		vice->first[i] = info[i].VOL;//副图左上角显示成交量
		for (j = 0; j < 5; j++)
		{
			vice->second[i] += info[i + j].VOL;
		}
		vice->second[i] /= 5.0;//副图中间显示5日均值
		for (j = 0; j < 10; j++)
		{
			vice->third[i] += info[i + j].VOL;
		}
		vice->third[i] /= 10.0;//右边显示10日均值
	}
	
	setlinestyle(SOLID_LINE, 0, 1);
	for (i = 1; i < 90; i++)
	{
		setcolor(YELLOW);//绘制5日成交量均线
		line(625 - 6 * i, 455 - (int)(vice->second[i - 1] * 100 / max), 619 - 6 * i, 455 - (int)(vice->second[i] * 100 / max));
		setcolor(GREEN);
		line(625 - 6 * i, 455 - (int)(vice->third[i - 1] * 100 / max), 619 - 6 * i, 455 - (int)(vice->third[i] * 100 / max));
	}
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(2, HORIZ_DIR, 4);
	setcolor(WHITE);
	outtextxy(82, 342, "VOL:");
	setcolor(YELLOW);
	outtextxy(172, 342, "M5:");
	setcolor(GREEN);
	outtextxy(262, 342, "M10:");
}

/*********************************************
FUNCTION:BIAS_draw
DESCRIPTION：绘制乖离率BIAS
INPUT:info,vice
RETURN:无
***********************************************/
void BIAS_draw(STODAYFLOAT* info, DOWNLINE* vice)//绘制乖离率BIAS
{
	int i, j;
	float MA6 = 0.0;
	float MA12 = 0.0;
	float MA24 = 0.0;
	float max = 0.0, min = 0.0;
	for (i = 0; i < 90; i++)
	{
		vice->first[i] = 0.0;
		vice->second[i] = 0.0;
		vice->third[i] = 0.0;
	}
	//乖离率=[(当日收盘价-N日平均价)/N日平均价]*100%
	//其中N，一般5、6、10、12、24、30和72。在实际运用中，短线使用6日乖离率较为有效，中线则放大为10日或12日。
	for (i = 0; i < 90; i++)
	{
		MA6 = 0.0;
		MA12 = 0.0;
		MA24 = 0.0;
		for (j = 0; j < 6; j++)
		{
			MA6 += info[i + j].Close;
		}
		MA6 /= 6.0;
		vice->first[i] = 100 * (info[i].Close - MA6) / MA6;//BIAS6

		for (j = 0; j < 12; j++)
		{
			MA12 += info[i + j].Close;
		}
		MA12 /= 12.0;
		vice->second[i] = 100 * (info[i].Close - MA12) / MA12;//BIAS12

		for (j = 0; j < 24; j++)
		{
			MA24 += info[i + j].Close;
		}
		MA24 /= 24.0;
		vice->third[i] = 100 * (info[i].Close - MA24) / MA24;//BIAS24
	}
	get_viceborder(vice, &max, &min);
	setlinestyle(SOLID_LINE, 0, 1);
	for (i = 1; i < 90; i++)
	{
		setcolor(WHITE);
		line(625 - 6 * i, 455 - (int)((vice->first[i - 1] - min) * 100 / (max - min)), 619 - 6 * i, 455 - (int)((vice->first[i] - min) * 100 / (max - min)));//RSI6
		setcolor(YELLOW);
		line(625 - 6 * i, 455 - (int)((vice->second[i - 1] - min) * 100 / (max - min)), 619 - 6 * i, 455 - (int)((vice->second[i] - min) * 100 / (max - min)));//RSI12
		setcolor(LIGHTMAGENTA);
		line(625 - 6 * i, 455 - (int)((vice->third[i - 1] - min) * 100 / (max - min)), 619 - 6 * i, 455 - (int)((vice->third[i] - min) * 100 / (max - min)));//RSI24
	}
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(2, HORIZ_DIR, 4);
	setcolor(WHITE);
	outtextxy(82, 342, "BIAS6:");
	setcolor(YELLOW);
	outtextxy(172, 342, "BIAS12:");
	setcolor(LIGHTMAGENTA);
	outtextxy(262, 342, "BIAS24:");
}

/*********************************************
FUNCTION:BRAR_draw
DESCRIPTION：绘制人气意愿指标BRAR
INPUT:info,vice
RETURN:无
***********************************************/
void BRAR_draw(STODAYFLOAT* info, DOWNLINE* vice)//绘制人气意愿指标BRAR
{
	int i, j;
	float H_O = 0.0, O_L = 0.0;
	float H_CY = 0.0, CY_L = 0.0;
	float max = 0.0, min = 0.0;
	for (i = 0; i < 90; i++)
	{
		vice->first[i] = 0.0;
		vice->second[i] = 0.0;
		vice->third[i] = 0.0;
	}
	for (i = 0; i < 90; i++)
	{
		H_O = 0.0;
		O_L = 0.0;
		for (j = 0; j < 26; j++)
		{
			H_O += info[i + j].High - info[i + j].Low;
			O_L += info[i + j].Open - info[i + j].Low;
		}
		vice->first[i] = 100 * (H_O / O_L);
		/*N日AR=(N日内（H－O）之和除以N日内（O－L）之和)*100。其中，H为当日最高价，L为当日最低价，O为当日开盘价，N为设定的时间参数，一般原始参数日设定为26日*/
	}
	for (i = 0; i < 90; i++)
	{
		H_CY = 0.0;
		CY_L = 0.0;
		for (j = 0; j < 26; j++)
		{
			H_CY += info[i + j].High - info[i + j + 1].Close;
			CY_L += info[i + j + 1].Close - info[i + j].Low;
		}
		vice->second[i] = 100 * (H_CY / CY_L);
		/*N日BR=N日内（H－CY）之和除以N日内（CY－L）之和*100。其中，H为当日最高价，L为当日最低价，CY为前一交易日的收盘价，N为设定的时间参数，一般原始参数日设定为26日。*/
	}
	get_viceborder(vice, &max, &min);
	setlinestyle(SOLID_LINE, 0, 1);
	for (i = 1; i < 90; i++)
	{
		setcolor(WHITE);
		line(625 - 6 * i, 455 - (int)((vice->first[i - 1] - min) * 100 / (max - min)), 619 - 6 * i, 455 - (int)((vice->first[i] - min) * 100 / (max - min)));//AR
		setcolor(YELLOW);
		line(625 - 6 * i, 455 - (int)((vice->second[i - 1] - min) * 100 / (max - min)), 619 - 6 * i, 455 - (int)((vice->second[i] - min) * 100 / (max - min)));//BR
	}
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(2, HORIZ_DIR, 4);
	setcolor(WHITE);
	outtextxy(82, 342, "AR:");
	setcolor(YELLOW);
	outtextxy(172, 342, "BR:");
}

/*********************************************
FUNCTION:CR_draw
DESCRIPTION：绘制能量指标CR
INPUT:info,vice
RETURN:无
***********************************************/
void CR_draw(STODAYFLOAT* info, DOWNLINE* vice)//绘制能量指标CR
{
	int i, j;
	float P1 = 0.0, P2 = 0.0;
	float YM = 0.0;
	float max=0.0, min=0.0;
	for (i = 0; i < 90; i++)
	{
		vice->first[i] = 0.0;
		vice->second[i] = 0.0;
		vice->third[i] = 0.0;
	}
	for (i = 0; i < 110; i++)
	{
		P1 = 0.0;
		P2 = 0.0;
		for (j = 0; j < 26; j++)
		{
			YM = (info[i + j + 1].Close + info[i + j + 1].Open + info[i + j + 1].Low + info[i + j + 1].High) / 4.0;
			/*中间价YM通过对昨日（YM）交易的最高价、最低价、开盘价和收盘价进行加权平均而得到的,加权方式有四种：
			1、M = （2C + H + L）÷4
			2、M = （C + H + L + O）÷4
			3、M = （C + H + L）÷3
			4、M = （H + L）÷2这里采取第二种加权方式*/
			P1 += info[i + j].High - YM;
			P2 += YM - info[i + j].Low;
		}
		vice->first[i] = 100 * P1 / P2;//CR（N日）=P1÷P2×100。式中，P1=Σ（H－YM），表示N日以来多方力量的总和；P2=Σ（YM－L），表示N日以来空方力量的总和。
	}
	for (i = 0; i < 90; i++)
	{
		for (j = 0; j < 10; j++)
		{
			vice->second[i] += vice->first[i + j];
		}
		vice->second[i] /= 10.0;//CR十日均线
	}
	for (i = 0; i < 90; i++)
	{	
		for (j = 0; j < 20; j++)
		{
			vice->third[i] += vice->first[i + j];
		}
		vice->third[i] /= 20.0;//CR二十日均线
	}
	get_viceborder(vice, &max, &min);
	setlinestyle(SOLID_LINE, 0, 1);
	for (i = 1; i < 90; i++)
	{
		setcolor(WHITE);
		line(625 - 6 * i, 455 - (int)((vice->first[i - 1] - min) * 100 / (max - min)), 619 - 6 * i, 455 - (int)((vice->first[i] - min) * 100 / (max - min)));//RSI6
		setcolor(YELLOW);
		line(625 - 6 * i, 455 - (int)((vice->second[i - 1] - min) * 100 / (max - min)), 619 - 6 * i, 455 - (int)((vice->second[i] - min) * 100 / (max - min)));//RSI12
		setcolor(LIGHTMAGENTA);
		line(625 - 6 * i, 455 - (int)((vice->third[i - 1] - min) * 100 / (max - min)), 619 - 6 * i, 455 - (int)((vice->third[i] - min) * 100 / (max - min)));//RSI24
	}
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(2, HORIZ_DIR, 4);
	setcolor(WHITE);
	outtextxy(82, 342, "CR:");
	setcolor(YELLOW);
	outtextxy(172, 342, "MA10:");
	setcolor(LIGHTMAGENTA);
	outtextxy(262, 342, "MA20:");
}

/*********************************************
FUNCTION:DMA_draw
DESCRIPTION：绘制平行线差DMA
INPUT:info,vice
RETURN:无
***********************************************/
void DMA_draw(STODAYFLOAT* info, DOWNLINE* vice)//绘制平行线差DMA
{
	int i, j;
	float max = 0.0, min = 0.0;
	float short_arv = 0.0, long_arv = 0.0;
	for (i = 0; i < 90; i++)
	{
		vice->first[i] = 0.0;
		vice->second[i] = 0.0;
		vice->third[i] = 0.0;
	}
	for (i = 0; i < 100; i++)
	{
		short_arv = 0.0;
		long_arv = 0.0;
		for (j = 0; j < 10; j++)
		{
			short_arv += info[i + j].Close;
		}
		short_arv /= 10.0;
		for (j = 0; j < 50; j++)
		{
			long_arv += info[i + j].Close;
		}
		long_arv /= 50.0;
		vice->first[i] = short_arv - long_arv;//DMA（10）=10日股价平均值—50日股价平均值
	}
	for (i = 0; i < 90; i++)
	{
		for (j = 0; j < 10; j++)
		{
			vice->second[i] += vice->first[i + j];//AMA（10）=10日DMA平均值
		}
		vice->second[i] /= 10.0;
	}
	get_viceborder(vice, &max, &min);
	setlinestyle(SOLID_LINE, 0, 1);
	for (i = 1; i < 90; i++)
	{
		setcolor(WHITE);
		line(625 - 6 * i, 455 - (int)((vice->first[i - 1] - min) * 100 / (max - min)), 619 - 6 * i, 455 - (int)((vice->first[i] - min) * 100 / (max - min)));//RSI6
		setcolor(YELLOW);
		line(625 - 6 * i, 455 - (int)((vice->second[i - 1] - min) * 100 / (max - min)), 619 - 6 * i, 455 - (int)((vice->second[i] - min) * 100 / (max - min)));//RSI12
	}
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(2, HORIZ_DIR, 4);
	setcolor(WHITE);
	outtextxy(82, 342, "DMA:");
	setcolor(YELLOW);
	outtextxy(172, 342, "AMA:");
}


/*********************************************
FUNCTION:OBV_draw
DESCRIPTION：绘制能量潮OBV
INPUT:info,vice
RETURN:无
***********************************************/
void OBV_draw(STODAYFLOAT* info, DOWNLINE* vice)//绘制能量潮OBV
{
	int i;
	float OBV = 0.0;
	float max = 0.0, min = 0.0;
	for (i = 0; i < 90; i++)
	{
		vice->first[i] = 0.0;
		vice->second[i] = 0.0;
		vice->third[i] = 0.0;
	}
	/*以某日为基期，逐日累计每日上市股票总成交量，若隔日指数或股票上涨，则基期OBV加上本日成交量为本日OBV。隔日指数或股票下跌，则基期OBV减去本日成交量为本日OBV。一般来说，只是观察OBV的升降并无多大意义，必须配合K线图的走势才有实际的效用。*/
	for (i = 0; i < 90; i++)
	{
		if (info[i + 1].Close - info[i].Close >= 0)//下跌
		{
			OBV += info[i].VOL;
		}
		else if (info[i + 1].Close - info[i].Close < 0)
		{
			OBV -= info[i].VOL;
		}
		vice->first[i] = OBV;
	}
	get_viceborder(vice, &max, &min);
	setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
	for (i = 1; i < 90; i++)
	{
		//(OBV-min)/(max-min)=(456-y)/110
		setcolor(WHITE);
		line(625 - 6 * i, 455 - (int)((vice->first[i - 1] - min) * 100 / (max - min)), 619 - 6 * i, 455 - (int)((vice->first[i] - min) * 100 / (max - min)));//绘制k线
	}
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(2, HORIZ_DIR, 4);
	setcolor(WHITE);
	outtextxy(82, 342, "OBV:");
}

/*********************************************
FUNCTION:KDJ_draw
DESCRIPTION：绘制随机指标KDJ
INPUT:info,vice
RETURN:无
***********************************************/
void KDJ_draw(STODAYFLOAT* info, DOWNLINE* vice)
{
	int i, j;
	float RSV = 0.0, closemax, closemin;//max,min为9日内
	float max, min;//边界值
	float K[91];
	float D[91];
	float J[91];
	K[90] = 50.0;
	D[90] = 50.0;//若无前一日K值与D值，则可分别用50来代替
	for (i = 0; i < 90; i++)
	{
		vice->first[i] = 0.0;
		vice->second[i] = 0.0;
		vice->third[i] = 0.0;
		K[i] = 0.0;
		D[i] = 0.0;
		J[i] = 0.0;
	}
	for (i = 89; i >= 0; i--)
	{
		//n日RSV=（Cn－Ln）/（Hn－Ln）×100;
		//Cn为第n日收盘价；Ln为n日内的最低价；Hn为n日内的最高价;
		closemin = info[i].Close;
		closemax = info[i].Close;
		for (j = 0; j < 9; j++)//以9日为周期的KD线
		{
			if (info[i + j].Close > closemax)
			{
				closemax = info[i + j].Close;
			}
			if (info[i + j].Close < closemin)
			{
				closemin = info[i + j].Close;
			}
		}
		RSV = 100 * (info[i].Close - closemin) / (closemax - closemin);
		K[i] = K[i + 1] * 0.67 + RSV * 0.33;//当日K值=2/3×前一日K值+1/3×当日RSV
		D[i] = D[i + 1] * 0.67 + K[i] * 0.33;//当日D值=2/3×前一日D值+1/3×当日K值
		J[i] = 3 * K[i + 1] - 2 * D[i + 1];//当日J值=3*当日K值-2*当日D值
	}

	for (i = 0; i < 90; i++)
	{
		vice->first[i] = K[i];
		vice->second[i] = D[i];
		vice->third[i] = J[i];
	}

	get_viceborder(vice, &max, &min);
	setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
	for (i = 1; i < 90; i++)
	{
		//(KDJ-min)/(max-min)=(456-y)/110
		setcolor(LIGHTRED);
		line(625 - 6 * i, 455 - (int)((K[i - 1] - min) * 100 / (max - min)), 619 - 6 * i, 455 - (int)((K[i] - min) * 100 / (max - min)));//绘制k线
		setcolor(YELLOW);
		line(625 - 6 * i, 455 - (int)((D[i - 1] - min) * 100 / (max - min)), 619 - 6 * i, 455 - (int)((D[i] - min) * 100 / (max - min)));//绘制d线
		setcolor(LIGHTMAGENTA);
		line(625 - 6 * i, 455 - (int)((J[i - 1] - min) * 100 / (max - min)), 619 - 6 * i, 455 - (int)((J[i] - min) * 100 / (max - min)));//绘制j线
	}

	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(2, HORIZ_DIR, 4);
	setcolor(LIGHTRED);
	outtextxy(82, 342, "K:");
	setcolor(YELLOW);
	outtextxy(172, 342, "D:");
	setcolor(LIGHTMAGENTA);
	outtextxy(262, 342, "J:");
}

/*********************************************
FUNCTION:MACD_draw
DESCRIPTION：绘制异同移动平均线MACD
INPUT:info,vice
RETURN:无
***********************************************/
void MACD_draw(STODAYFLOAT* info, DOWNLINE* vice)
{
	int i;
	float EMA12[91] = { 0 };
	float EMA26[91] = { 0 };//12、16日移动平均
	float DIF[91] = { 0 };//离差值
	float DEA[91] = { 0 };//离差平均值
	float max = 0.0;
	EMA12[90] = info[90].Close;
	EMA26[90] = info[90].Close;
	for (i = 0; i < 90; i++)
	{
		vice->first[i] = 0.0;
		vice->second[i] = 0.0;
		vice->third[i] = 0.0;
	}
	for (i = 89; i >= 0; i--)
	{
		EMA12[i] = EMA12[i + 1] * 0.846 + info[i].Close * 0.154;
		//EMA（12）=前一日EMA（12）×11/13+今日收盘价×2/13	
		EMA26[i] = EMA26[i + 1] * 0.926 + info[i].Close * 0.074;
		//EMA（26）=前一日EMA（26）×25/27+今日收盘价×2/27
		DIF[i] = EMA12[i] - EMA26[i];
		//DIF=今日EMA（12）－今日EMA（26）
		DEA[i] = DEA[i + 1] * 0.8 + DIF[i] * 0.2;
		//今日DEA（MACD）=前一日DEA×8/10+今日DIF×2/10
	}
	for (i = 89; i > 0; i--)
	{
		vice->first[i] = EMA12[i] - EMA26[i];//DIF
		vice->second[i] = DIF[i] * 0.2 + DEA[i + 1] * 0.8;//DEA
		vice->third[i] = 2 * (DIF[i] - DEA[i]);//用（DIF-DEA）×2即为MACD柱状图
		if (fabs(vice->first[i]) > max)
		{
			max = fabs(vice->first[i]);
		}
		if (fabs(vice->second[i]) > max)
		{
			max = fabs(vice->second[i]);
		}
		if (fabs(vice->third[i]) > max)
		{
			max = fabs(vice->third[i]);
		}
	}

	setlinestyle(SOLID_LINE, 0, 1);
	for (i = 0; i < 90; i++)
	{
		if ((vice->third[i]) >= 0)
		{
			setcolor(RED);
		}
		else if ((vice->third[i]) < 0)
		{
			setcolor(GREEN);
		}
		line(620 - 6 * i, 400, 620 - 6 * i, 400 - (int)(50 * vice->third[i] / max));//绘制MACD柱线
		
	}
	for (i = 1; i < 90; i++)
	{
		setcolor(YELLOW);
		line(625 - 6 * i, 400 - (int)(50 * vice->second[i - 1] / max), 619 - 6 * i, 400 - (int)(50 * vice->second[i] / max));//绘制DEA
		setcolor(WHITE);
		line(625 - 6 * i, 400 - (int)(50 * vice->first[i - 1] / max), 619 - 6 * i, 400 - (int)(50 * vice->first[i] / max));//绘制DIF
	}
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(2, HORIZ_DIR, 4);
	setcolor(WHITE);
	outtextxy(82, 342, "DIF:");
	setcolor(YELLOW);
	outtextxy(172, 342, "DEA:");
	setcolor(RED);
	outtextxy(262, 342, "MACD:");
}

/*********************************************
FUNCTION:RSI_draw
DESCRIPTION：绘制相对强弱指标RSI
INPUT:info,vice
RETURN:无
***********************************************/
void RSI_draw(STODAYFLOAT* info, DOWNLINE* vice)//相对强弱指标RSI
{
	int i;
	float max = 0.0, min = 0.0;
	for (i = 0; i < 90; i++)
	{
		vice->first[i] = 0.0;
		vice->second[i] = 0.0;
		vice->third[i] = 0.0;
	}
	for (i = 0; i < 90; i++)
	{
		vice->first[i] = RSI_calculate(info, i, 6);
		vice->second[i] = RSI_calculate(info, i, 12);
		vice->third[i] = RSI_calculate(info, i, 24);
	}
	get_viceborder(vice, &max, &min);
	setlinestyle(SOLID_LINE, 0, 1);
	for (i = 1; i < 90; i++)
	{
		setcolor(WHITE);
		line(625 - 6 * i, 455 - (int)((vice->first[i - 1] - min) * 100 / (max - min)), 619 - 6 * i, 455 - (int)((vice->first[i] - min) * 100 / (max - min)));//RSI6
		setcolor(YELLOW);
		line(625 - 6 * i, 455 - (int)((vice->second[i - 1] - min) * 100 / (max - min)), 619 - 6 * i, 455 - (int)((vice->second[i] - min) * 100 / (max - min)));//RSI12
		setcolor(LIGHTMAGENTA);
		line(625 - 6 * i, 455 - (int)((vice->third[i - 1] - min) * 100 / (max - min)), 619 - 6 * i, 455 - (int)((vice->third[i] - min) * 100 / (max - min)));//RSI24
	}
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(2, HORIZ_DIR, 4);
	setcolor(WHITE);
	outtextxy(82, 342, "RSI6:");
	setcolor(YELLOW);
	outtextxy(172, 342, "RSI12:");
	setcolor(LIGHTMAGENTA);
	outtextxy(262, 342, "RSI24:");
}

/*********************************************
FUNCTION:RSI_calculate
DESCRIPTION：计算RSIRSI
INPUT:info,date,days
RETURN:无
***********************************************/
float RSI_calculate(STODAYFLOAT* info, int date, int days)//RSI有6,12,24的区分
{
	int i;
	float RSI=0.0;//相对强弱指标RSI
	float sum_rise = 0.0;//涨数和
	float sum_fall = 0.0;//跌数和
	float difference;//两天收盘价的差值
	for (i = 0; i < days; i++)
	{
		difference = info[date + i + 1].Close - info[date + i].Close;//前一天-后一天(数据库内数据相反)

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