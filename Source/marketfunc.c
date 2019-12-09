#include"public.h"
#include"database.h"
#include"marketfc.h"
#include"techline.h"
#include"storead.h"

/*******************************
marketfunc.c
COPYRIGHT:	weijingmin
FUNCTION:	function in market
ABSTRACT:
		A.change_datashow
		B.draw float
		C.find drawing border
DATE:2019/10/20
********************************/

/*********************************************
FUNCTION:change_datadate
DESCRIPTION：改变显示信息的日期
INPUT:sto_num,No_date,info,
	upinfo,downinfo,max,min,read_way,LR
RETURN:无
***********************************************/
void change_datadate(int sto_num, long int* No_date, STODAYFLOAT info[], UPLINE* upinfo, DOWNLINE* downinfo, float* max, float* min, int read_way, int LR)
{
	int i, j;
	if (LR == 1)//左侧按钮往前开
	{
		switch (read_way)
		{
		case 1:
			//if (*No_date + 1 + 150 <= maxline)
			//{
			*No_date += 1;
			//}
			break;
		case 2:
			//if (*No_date + 30 + 150 <= maxline)
			//{
			*No_date += 30;
			//}
			break;
		case 3:
			//if (*No_date + 90 + 150 <= maxline)
			//{
			*No_date += 90;
			//}
		}

		sto_daybyday2float(sto_num, *No_date, 150, info);//1为股票序号，1为开始天数，150为读取天数
		get_border(info, max, min, 150);//150为读取天数
		setfillstyle(SOLID_FILL, BLACK);
		bar(81, 73, 619, 341);
		bar(81, 343, 619, 455);//遮盖之前的主图副图图像
		for (i = 0; i < 11; i++)
		{
			for (j = 0; j < 135; j++)
			{
				putpixel(80 + 4 * j, 72 + 40 * i, RED);
			}
		}
		Kline_draw(info, *max, *min);
		MA_draw(info, upinfo, *max, *min);
		VOL_draw(info, downinfo);
	}
	else if (LR == 2)//右侧按钮往后开
	{
		switch (read_way)
		{
		case 1:
			if (*No_date - 1 >= 1)
			{
				*No_date -= 1;
			}
			break;
		case 2:
			if (*No_date - 30 >= 1)
			{
				*No_date -= 30;
			}
			break;
		case 3:
			if (*No_date - 90 >= 1)
			{
				*No_date -= 90;
			}
			break;
		}
		

		sto_daybyday2float(sto_num, *No_date, 150, info);//1为股票序号，1为开始天数，150为读取天数
		get_border(info, max, min, 150);//150为读取天数
		setfillstyle(SOLID_FILL, BLACK);
		bar(81, 73, 619, 341);
		bar(81, 343, 619, 455);//遮盖之前的主图副图图像
		for (i = 0; i < 11; i++)
		{
			for (j = 0; j < 135; j++)
			{
				putpixel(80 + 4 * j, 72 + 40 * i, RED);
			}
		}
		Kline_draw(info, *max, *min);
		MA_draw(info, upinfo, *max, *min);
		VOL_draw(info, downinfo);
	}
}


/*********************************************
FUNCTION:mouse_datashow
DESCRIPTION：鼠标移动来显示数据
INPUT:info,upinfo,downinfo,p_preday,p_day,
	state,colorstate
RETURN:无
***********************************************/
void mouse_datashow(STODAYFLOAT* info, UPLINE upinfo, DOWNLINE downinfo, int* p_preday, int* p_day,int state,int colorstate)
{
	float rose = 0.0;//涨幅
	float rise_fall = 0.0;//涨跌
	float swing = 0.0;//振幅
	char year[20] = { '\0' };
	char month[3] = { '\0' };
	char day[3] = { '\0' };
	int d;
	if (MouseX < 620 && MouseX>80 && MouseY < 456 && MouseY > 24 && (*p_preday != *p_day))
	{
		d = *p_day;

		rose = (100 * (info[d].Close - info[d + 1].Close)) / info[d + 1].Close;
		//涨幅=(现价-上一个交易日收盘价）/上一个交易日收盘价*100%
		rise_fall = info[d].Close - info[d + 1].Close;
		//涨跌=今收盘-昨收盘(前面有些函数中的rise_fall是当日涨跌，用来换色。这个是技术指标的涨跌)
		swing = (100 * (info[d].High - info[d].Low)) / info[d + 1].Close;
		//股票振幅是股票开盘后的当日最高价和最低价之间的差的绝对值与昨日收盘价的百分比
		
		//遮盖各项数据
		setfillstyle(SOLID_FILL, BLACK);
		bar(80 + 17 * 2 + 4, 27, 80 + 17 * 2 + 4 + 7 * 10, 27 + 10);
		bar(190 + 38, 27, 190 + 38 + 7 * 10, 27 + 10);
		bar(296 + 38, 27, 296 + 38 + 7 * 10, 27 + 10);
		bar(404 + 38, 27, 404 + 38 + 7 * 10, 27 + 10);
		bar(512 + 38, 27, 512 + 38 + 7 * 10, 27 + 10);

		bar(80 + 38, 51, 80 + 38 + 7 * 10, 51 + 10);
		bar(190 + 38, 51, 190 + 38 + 7 * 10, 51 + 10);
		bar(296 + 38, 51, 296 + 38 + 7 * 10, 51 + 10);
		bar(404 + 55, 51, 404 + 55 + 5 * 10, 51 + 10);
		bar(512 + 55, 51, 512 + 55 + 5 * 10, 51 + 10);

		bar(80 + 20, 73, 80 + 20 + 6 * 10, 82);
		bar(170 + 28, 73, 170 + 20 + 6 * 10, 82);
		bar(260 + 28, 73, 260 + 20 + 6 * 10, 82);
		bar(350 + 28, 73, 350 + 20 + 6 * 10, 82);

		bar(80 + 28 + 20, 343, 80 + 20 + 6 * 10, 351);
		bar(170 + 28 + 20, 343, 170 + 20 + 6 * 10, 351);
		bar(260 + 28 + 20, 343, 260 + 20 + 6 * 10, 351);

		
		settextjustify(LEFT_TEXT, TOP_TEXT);
		settextstyle(2, HORIZ_DIR, 4);

		if (rise_fall >= 0)
		{
			settextjustify(LEFT_TEXT, TOP_TEXT);
			settextstyle(0, HORIZ_DIR, 1);
			setcolor(LIGHTRED);
			puthz(82, 50, "涨跌", 16, 17, LIGHTRED);
			outtextxy(80 + 17 * 2, 56, ":");
		}
		else if (rise_fall < 0)
		{
			settextjustify(LEFT_TEXT, TOP_TEXT);
			settextstyle(0, HORIZ_DIR, 1);
			setcolor(LIGHTBLUE);
			puthz(82, 50, "涨跌", 16, 17, LIGHTBLUE);
			outtextxy(80 + 17 * 2, 56, ":");
		}
		settextjustify(LEFT_TEXT, TOP_TEXT);
		settextstyle(2, HORIZ_DIR, 4);
		outtextxy(80 + 38 + 35, 53, "%");
		if (rose >= 0)
		{
			settextjustify(LEFT_TEXT, TOP_TEXT);
			settextstyle(0, HORIZ_DIR, 1);
			setcolor(RED);
			puthz(190, 50, "涨幅", 16, 17, RED);
			outtextxy(188 + 17 * 2, 56, ":");
		}
		else if (rise_fall < 0)
		{
			settextjustify(LEFT_TEXT, TOP_TEXT);
			settextstyle(0, HORIZ_DIR, 1);
			setcolor(LIGHTCYAN);
			puthz(190, 50, "涨幅", 16, 17, LIGHTCYAN);
			outtextxy(188 + 17 * 2, 56, ":");
		}

		settextjustify(LEFT_TEXT, TOP_TEXT);
		settextstyle(2, HORIZ_DIR, 4);
		outtextxy(190 + 38 + 35, 53, "%");
		setcolor(YELLOW);
		outtextxy(296 + 38 + 35, 53, "%");
		setcolor(LIGHTBLUE);
		outtextxy(404 + 55 + 35, 52, "M");
		setcolor(LIGHTCYAN);
		outtextxy(512 + 55 + 35, 52, "B");

		/*settextjustify(LEFT_TEXT, TOP_TEXT);
		settextstyle(2, HORIZ_DIR, 4);
		setcolor(WHITE);
		outtextxy(80 + 28 + 35, 342, "M");
		setcolor(YELLOW);
		outtextxy(170 + 20 + 35, 342, "M");
		setcolor(GREEN);
		outtextxy(260 + 28 + 35, 342, "M");*/

		setcolor(YELLOW);
		settextjustify(LEFT_TEXT, TOP_TEXT);
		settextstyle(2, HORIZ_DIR, 4);
		itoa(info[d].Year, year, 10);
		itoa(info[d].Month, month, 10);
		itoa(info[d].Day, day, 10);
		strcat(year, "/");
		strcat(year, month);
		strcat(year, "/");
		strcat(year, day);

		outtextxy(80 + 44, 28, year);//显示时间
		draw_float(info[d].Open, 190 + 44, 28, 2, WHITE);//今开
		draw_float(info[d].High, 296 + 44, 28, 2, RED);//最高
		draw_float(info[d].Low, 404 + 44, 28, 2, GREEN);//最低
		draw_float(info[d].Close, 512 + 44, 28, 2, WHITE);//今收

		if (rise_fall >= 0)
		{
			draw_float(rise_fall, 80 + 44, 52, 2, LIGHTRED);//涨跌
		}
		else if (rise_fall < 0)
		{
			draw_float(rise_fall, 80 + 44, 52, 2, LIGHTBLUE);//涨跌
		}
		if (rose >= 0)
		{
			draw_float(rose, 190 + 44, 52, 2, RED);//涨幅
		}
		else if (rose < 0)
		{
			draw_float(rose, 190 + 44, 52, 2, LIGHTCYAN);//涨幅
		}
		draw_float(swing, 296 + 44, 52, 2, YELLOW);//振幅
		draw_float(info[d].VOL / 1000000.0, 404 + 61, 52, 2, LIGHTBLUE);//成交量
		draw_float(info[d].VOT / 100000000.0, 512 + 61, 52, 2, LIGHTCYAN);//成交额
		
		draw_float(upinfo.first[d], 80 + 20, 72, 2, WHITE);//5日均线，BOLL上轨，ENE上轨
		draw_float(upinfo.second[d], 170 + 28, 72, 2, YELLOW);//10日均线，BOLL中轨，ENE中轨
		draw_float(upinfo.third[d], 260 + 28, 72, 2 , LIGHTMAGENTA);//20日均线，BOLL下轨，ENE下轨
		if (state == 0)//表明需要画四个数据
		{
			draw_float(upinfo.forth[d], 350 + 28, 72, 2, GREEN);//60日均线
		}
		
		if (colorstate == 1)
		{
			draw_float(downinfo.first[d] / 1000000.0, 80 + 28 + 20, 342, 2, WHITE);//副图VOL
		}
		else if (colorstate == 2 )
		{
			draw_float(downinfo.first[d], 80 + 28 + 20, 342, 2, LIGHTRED);//K
		}
		else if (colorstate == 3)
		{
			draw_float(downinfo.first[d], 80 + 28 + 20, 342, 2, WHITE);//DIF
		}
		else if (colorstate == 4)
		{
			draw_float(downinfo.first[d], 80 + 28 + 20, 342, 2, WHITE);//RSI6,BIAS24,CR
		}
		else if (colorstate == 5)
		{
			draw_float(downinfo.first[d] / 1000000.0, 80 + 28 + 20, 342, 2, WHITE);//OBV
		}
		else if (colorstate == 6)
		{
			draw_float(downinfo.first[d], 80 + 28 + 20, 342, 2, WHITE);//DMA,AR
		}
		
		if (colorstate == 1)
		{
			draw_float(downinfo.second[d] / 1000000.0, 170 + 28 + 20, 342, 2, YELLOW);//副图VOL5日均线
		}
		else if (colorstate == 2)
		{
			draw_float(downinfo.second[d], 170 + 28 + 20, 342, 2, YELLOW);//DEA
		}
		else if (colorstate == 3)
		{
			draw_float(downinfo.second[d], 170 + 28 + 20, 342, 2, YELLOW);//D
		}
		else if (colorstate == 4)
		{
			draw_float(downinfo.second[d], 170 + 28 + 20, 342, 2, YELLOW);//RSI12,BIAS24,CRMA1
		}
		else if (colorstate == 6)
		{
			draw_float(downinfo.second[d], 170 + 28 + 20, 342, 2, YELLOW);//AMA,BR
		}
		
		
		if (colorstate == 1)
		{
			draw_float(downinfo.third[d] / 1000000.0, 260 + 28 + 20, 342, 2, GREEN);//副图VOL10日均线
		}
		else if (colorstate == 2)
		{
			draw_float(downinfo.third[d], 260 + 28 + 20, 342, 2, LIGHTMAGENTA);//J
		}
		else if (colorstate == 3)
		{
			draw_float(downinfo.third[d], 260 + 28 + 20, 342, 2, RED);//MACD
		}
		else if (colorstate == 4)
		{
			draw_float(downinfo.third[d], 260 + 28 + 20, 342, 2, LIGHTMAGENTA);//RSI24,BIAS24,CRMA2
		}
		
		
	}
}

/*********************************************
FUNCTION:draw_float
DESCRIPTION：在指定位置画浮点数
INPUT:flt,x,y,bits,color
RETURN:无
***********************************************/
void draw_float(float flt, int x, int y, int bits,int color)//bits为要画的限定小数位数
{
	int i;
	int integer;//整数部分
	float decimal;//带bits的小数部分
	int part;//四舍五入后的小数部分(用整数表达)
	float temp_bit;//bits的后一位需要四舍五入
	char ch_integer[5] = { '\0' };
	char ch_decimals[3] = { '\0' };
	char str[12] = { '\0' };

	//如果为绝对值小于1的负数
	if (flt > -1.0 && flt < 0.0)
	{
		strcat(str, "-");//拷贝负号
	}
	integer = (int)flt;
	itoa(integer, ch_integer, 10);
	strcat(str, ch_integer);//拷贝整数部分
	strcat(str, ".");

	decimal = fabs(flt - (int)flt);//小数部分
	for (i = 0; i < bits; i++)
	{
		decimal *= 10;//根据需要打印的位数化成整数
	}

	//bits的后一位temp_bits四舍五入
	temp_bit = decimal - (int)decimal;
	if (temp_bit < 0.5)
	{
		part = (int)decimal;
	}
	else if (temp_bit >= 0.5)
	{
		part = (int)decimal + 1;
	}

	//如果小数第一位为0，补全第一位
	if (part < 10 && part != 0)
	{
		strcat(str, "0");
	}
	//没有小数部分，补全0
	else if (part == 0)
	{
		strcat(str, "00");
	}

	if (part != 0)
	{
		itoa(part, ch_decimals, 10);
		strcat(str, ch_decimals);//拷贝小数部分
	}
	setcolor(color);
	outtextxy(x, y, str);

}

/*********************************************
FUNCTION:get_viceborder
DESCRIPTION：寻找画副图的边界函数
INPUT:vice,max,min
RETURN:无
***********************************************/
void get_viceborder(DOWNLINE* vice, float* max, float* min)
{
	int i;
	*max = vice->first[0];
	*min = vice->first[0];
	for (i = 0; i < 90; i++)
	{
		if (vice->first[i] > * max)
		{
			*max = vice->first[i];
		}
		if (vice->first[i] < *min)
		{
			*min = vice->first[i];
		}
		if (vice->second[i] > * max)
		{
			*max = vice->second[i];
		}
		if (vice->second[i] < *min)
		{
			*min = vice->second[i];
		}
		if (vice->third[i] > * max)
		{
			*max = vice->third[i];
		}
		if (vice->third[i] < *min)
		{
			*min = vice->third[i];
		}
	}
}

/*********************************************
FUNCTION:get_border
DESCRIPTION：寻找画主图的边界函数
INPUT:info,max,min,read_days
RETURN:无
***********************************************/
void get_border(STODAYFLOAT* info, float* max, float* min, int read_days)
{
	int i, j;
	float MA5 = 0.0;//MA5平均
	float MA10 = 0.0;//MA10平均
	float MA20 = 0.0;//MA20平均(布林线中轨)
	float MA60 = 0.0;//MA60平均
	float std_dev = 0.0;//标准差
	float BOLL_toprail = 0.0;//布林线上轨
	float BOLL_bottomrail = 0.0;//布林线下轨
	float ENE_top = 0.0;//ENE上轨
	float ENE_bottom = 0.0;//ENE下轨
	//float ENE = 0.0;//ENE中轨

	*max = info[0].High;
	*min = info[0].Low;
	for (i = 0; i < read_days - 60; i++)//i+j不可越界,j最多到六十 
	{
		MA5 = 0.0;
		MA10 = 0.0;
		MA20 = 0.0;
		MA60 = 0.0;
		if (info[i].High > * max)
		{
			*max = info[i].High;//寻找最大的最高价作为上边界
		}
		if (info[i].Low < *min)
		{
			*min = info[i].Low;//寻找最小的最低价作为下边界
		}

		for (j = 0; j < 5; j++)
		{
			MA5 += info[i + j].Close;
		}
		MA5 /= 5.0;//计算五日均线
		if (MA5 > * max)
		{
			*max = MA5;//五日均线大于最高价，为上边界
		}
		if (MA5 < *min)
		{
			*min = MA5;//五日均线小于最低价，为下边界
		}

		for (j = 0; j < 10; j++)
		{
			MA10 += info[i + j].Close;
		}
		MA10 /= 10.0;//计算十日均线
		if (MA10 > * max)
		{
			*max = MA10;//十日均线最大，为上边界
		}
		if (MA10 < *min)
		{
			*min = MA10;//十日均线最小，为下边界
		}

		for (j = 0; j < 20; j++)
		{
			MA20 += info[i + j].Close;
		}
		MA20 /= 20.0;//计算二十日均线
		if (MA20 > * max)
		{
			*max = MA20;//二十日均线最大，为上边界
		}
		if (MA20 < *min)
		{
			*min = MA20;//二十日均线最小，为下边界
		}

		for (j = 0; j < 60; j++)
		{
			MA60 += info[i + j].Close;
		}
		MA60 /= 60.0;//计算六十日均线
		if (MA60 > * max)
		{
			*max = MA60;//六十日均线最大，为上边界
		}
		if (MA60 < *min)
		{
			*min = MA60;//六十日均线最小，为下边界
		}

		std_dev = std_deviation(info, 20, i);//计算从i日开始20天内的方差
		//中轨线=N日的移动平均线，布林线日指标N一般为20

		BOLL_toprail = MA20 + 2 * std_dev;//上轨线（up线）=中轨线+两倍的标准差（2SD）
		BOLL_bottomrail = MA20 - 2 * std_dev;//下轨线（down线）=中轨线-两倍的标准差（2SD）

		if (BOLL_toprail > * max)
		{
			*max = BOLL_toprail;//布林线上轨最大，为上边界
		}
		if (BOLL_bottomrail < *min)
		{
			*min = BOLL_bottomrail;//布林线下轨最大，为下边界
		}
		/*M1代表中轨基准线的上浮百分比，一般设为11（即11%）；
		M2代表中轨基准线的下跌百分比，一般设为9（即9%）。
		N日平均一般取十日均线*/
		ENE_top = (1 + 11 / 100.0) * MA10;		//ENE上轨=(1+M1/100)*收盘价的N日简单移动平均
		ENE_bottom = (1 - 9 / 100.0) * MA10;	//ENE下轨=(1-M2/100)*收盘价的N日简单移动平均
		//ENE = (ENE_top + ENE_bottom) / 2.0;	//ENE中轨=(ENE上轨+ENE下轨)/2
		if (ENE_top > * max)
		{
			*max = ENE_top;//轨道线上轨最大，为上边界
		}
		if (ENE_bottom < *min)
		{
			*min = ENE_bottom;//轨道线下轨最大，为下边界
		}
	}
}

/*********************************************
FUNCTION:std_deviation
DESCRIPTION：计算标准差,days为20.0
INPUT:info,days,date
RETURN:无
***********************************************/
float std_deviation(STODAYFLOAT* info, int days, int date)
{
	int i;
	float aver = 0.0;//20日平均值
	float variance = 0.0;//方差
	float std_dev = 0.0;//标准差
	for (i = 0; i < days; i++)
	{
		aver += info[date + i].Close;
	}
	aver /= days * 1.0;//计算均值
	for (i = 0; i < days; i++)
	{
		variance += (info[date + i].Close - aver) * (info[date + i].Close - aver);
	}
	variance /= days * 1.0;//计算方差
	std_dev = sqrt(variance);//计算标准差

	return std_dev;
}