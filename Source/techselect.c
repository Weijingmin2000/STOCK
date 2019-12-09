#include"public.h"
#include"database.h"
#include"techslc.h"
#include"slctfunc.h"
#include"fundam.h"

/*****************************************
techselect.c
COPYRIGHT:	weijingmin
FUNCTION:	control and draw
ABSTRACT:
		A.draw T_select page1 and page2
		B.control and skip
		C.button lighting and recovering
DATE:2019/10/26
******************************************/


/*********************************************
FUNCTION:technical_control
DESCRIPTION：选股的总控函数
INPUT:cinfo,u,sto_list,page
RETURN:0/t(sto_list[i])
***********************************************/
int technical_control(Ctr* cinfo,USER* u, int *sto_list,int* page)
{
	int i, j;
	int t = 0;
	int return_page;//点击返回键返回的页面
	int option[11][6];//技术面选股的option数组
	//分别对应着:
	//1Kline,2Kline,3Kline,nKline,MA,KDJ,MACD,RSI,VOL/OBV,B IAS,BOLL
	memset(option, 0, 66*sizeof(int));
	memset(cinfo, 0, sizeof(Ctr ));
	cinfo->func = 5;
	while (1)
	{
		if(cinfo->func != 5 /*&& cinfo->func != 10 && cinfo->func != 9*/)
		{
			clrmous(MouseX, MouseY);
			delay(10);
			return 0;
		}
		switch (*page)
		{
		case 1:
			T_stoselect1(cinfo, u, page, option, sto_list);
			memset(option, 0, 66*sizeof(int));//到此处表明有翻页操作，需要全部置零
			return_page = 1;
			break;
		case 2:
			T_stoselect2(cinfo, u, page, option, sto_list);
			memset(option, 0, 66*sizeof(int));//到此处表明有翻页操作，需要全部置零
			return_page = 1;
			break;
		case 3:
			t = T_searchresult(cinfo, u, page, sto_list,return_page);//显示选股结果
			if(t)
				return t;
			break;
		case 4:
			fundam_control(cinfo, u, page, sto_list);//基本面选股
			return_page = 4;
			break;
		}
	}
}

/*********************************************
FUNCTION:lightbutton_Tselect
DESCRIPTION：选股按钮点亮
INPUT:num
RETURN:无
***********************************************/
void lightbutton_Tselect(int num)
{
	clrmous(MouseX, MouseY);
	delay(10);
	switch (num)
	{
	case 1:
		setcolor(BLUE);
		setfillstyle(SOLID_FILL, LIGHTBLUE);
		bar3d(560, 452, 620, 479, 5, 1);
		puthz(565, 457, "下一页", 16, 17, BLUE);
		break;
	case 2:
		setcolor(RED);
		setfillstyle(SOLID_FILL, LIGHTRED);
		bar3d(20, 452, 80, 479, 5, 1);
		puthz(25, 457, "上一页", 16, 17, RED);
		break;
	case 3:
		setcolor(CYAN);
		setfillstyle(SOLID_FILL, LIGHTCYAN);
		bar3d(250, 452, 290, 479, 5, 1);
		puthz(255, 457, "选股", 16, 17, CYAN);
		break;
	case 4:
		setcolor(MAGENTA);
		setfillstyle(SOLID_FILL, MAGENTA);
		bar3d(250, 452, 290, 479, 5, 1);
		puthz(255, 457, "选股", 16, 17, LIGHTMAGENTA);
		break;
	case 5:
		setcolor(RED);
		setfillstyle(SOLID_FILL, LIGHTRED);
		bar(100, 462, 140, 479);
		rectangle(100, 462, 140, 479);
		puthz(105, 462, "返回", 16, 17, RED);
	}
}

/*********************************************
FUNCTION:recoverbutton_Tselect
DESCRIPTION：选股按钮恢复点亮
INPUT:num
RETURN:无
***********************************************/
void recoverbutton_Tselect(int num)
{
	clrmous(MouseX, MouseY);
	delay(10);
	switch (num)
	{
	case 1:
		setcolor(WHITE);
		setfillstyle(SOLID_FILL, BLUE);
		bar3d(560, 452, 620, 479, 5, 1);
		puthz(565, 457, "下一页", 16, 17, WHITE);
		break;
	case 2:
		setcolor(WHITE);
		setfillstyle(SOLID_FILL, RED);
		bar3d(20, 452, 80, 479, 5, 1);
		puthz(25, 457, "上一页", 16, 17, WHITE);
		break;
	case 3:
		setcolor(WHITE);
		setfillstyle(SOLID_FILL, CYAN);
		bar3d(250, 452, 290, 479, 5, 1);
		puthz(255, 457, "选股", 16, 17, WHITE);
		break;
	case 4:
		setcolor(WHITE);
		setfillstyle(SOLID_FILL, LIGHTMAGENTA);
		bar3d(250, 452, 290, 479, 5, 1);
		puthz(255, 457, "选股", 16, 17, WHITE);
		break;
	case 5:
		setcolor(WHITE);
		setfillstyle(SOLID_FILL, RED);
		bar(100, 462, 140, 479);
		rectangle(100, 462, 140, 479);
		puthz(105, 462, "返回", 16, 17, WHITE);
	}
}

/*********************************************
FUNCTION:draw_T_stoselect1
DESCRIPTION：技术面选股第一页绘制
INPUT:void
RETURN:无
***********************************************/
void draw_T_stoselect1(void)
{
	setbkcolor(BLACK);
	setlinestyle(SOLID_LINE, 0, 1);
	setcolor(RED);
	line(0, 24, 24, 48);
	line(24, 48, 600, 48);
	line(24, 48, 24, 48 + 240);
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(3, HORIZ_DIR, 3);
	outtextxy(6, 48, "K");
	puthz(0, 48 + 24 + 6, "线", 24, 0, RED);
	puthz(0, 48 + 24*2 + 6*2, "形", 24, 0, RED);
	puthz(0, 48 + 24*3 + 6*3, "态", 24, 0, RED);
	puthz(0, 48 + 24*4 + 6*4, "选", 24, 0, RED);
	puthz(0, 48 + 24*5 + 6*5, "股", 24, 0, RED);


	setcolor(WHITE);
	settextstyle(3, HORIZ_DIR, 1);
	puthz(25, 49, "单根", 16, 17, WHITE);
	outtextxy(25 + 17 * 2 + 4, 42, "K");
	puthz(25 + 17 * 3, 49, "线", 16, 17, WHITE);
	outtextxy(25 + 17 * 4 + 2, 42, ":");
	line(25, 68, 225, 68);
	puthz(35, 80, "一字涨停板", 16, 17, WHITE);
	draw_element(35 + 17 * 5 + 4, 83, 1, WHITE);//画按钮

	puthz(35, 80 + 30, "长上影线", 16, 17, WHITE);
	draw_element(35 + 17 * 5 + 4, 83 + 30, 1, WHITE);//画按钮

	puthz(35, 80 + 30 * 2, "大阳线", 16, 17, WHITE);
	draw_element(35 + 17 * 5 + 4, 83 + 30 * 2, 1, WHITE);//画按钮


	setcolor(YELLOW);
	settextstyle(3, HORIZ_DIR, 1);
	puthz(312, 49, "两根", 16, 17, YELLOW);
	outtextxy(312 + 17 * 2 + 4, 42, "K");
	puthz(312 + 17 * 3, 49, "线", 16, 17, YELLOW);
	outtextxy(312 + 17 * 4 + 2, 42, ":");
	line(312, 68, 512, 68);
	puthz(322, 80, "多方压制", 16, 17, YELLOW);
	draw_element(322 + 17 * 5 + 4, 83, 1, YELLOW);//画按钮

	puthz(322, 80 + 30, "空方压制", 16, 17, YELLOW);
	draw_element(322 + 17 * 5 + 4, 83 + 30, 1, YELLOW);//画按钮

	puthz(322, 80 + 30 * 2, "走势停顿", 16, 17, YELLOW);
	draw_element(322 + 17 * 5 + 4, 83 + 30 * 2, 1, YELLOW);//画按钮


	setcolor(LIGHTRED);
	settextstyle(3, HORIZ_DIR, 1);
	puthz(25, 168, "三根", 16, 17, LIGHTRED);
	outtextxy(25 + 17 * 2 + 4, 162, "K");
	puthz(25 + 17 * 3, 168, "线", 16, 17, LIGHTRED);
	outtextxy(25 + 17 * 4 + 2, 162, ":");
	line(25, 188, 200, 188);
	puthz(35, 200, "三连阳", 16, 17, LIGHTRED);
	draw_element(35 + 17 * 5 + 4, 203, 1, LIGHTRED);//画按钮

	puthz(35, 200 + 30, "三连阴", 16, 17, LIGHTRED);
	draw_element(35 + 17 * 5 + 4, 203 + 30, 1, LIGHTRED);//画按钮

	puthz(35, 200 + 30 * 2, "岛形反转", 16, 17, LIGHTRED);
	draw_element(35 + 17 * 5 + 4, 203 + 30 * 2, 1, LIGHTRED);//画按钮


	setcolor(LIGHTBLUE);
	settextstyle(3, HORIZ_DIR, 1);
	puthz(312, 168, "多根", 16, 17, LIGHTBLUE);
	outtextxy(312 + 17 * 2 + 4, 162, "K");
	puthz(312 + 17 * 3, 168, "线", 16, 17, LIGHTBLUE);
	outtextxy(312 + 17 * 4 + 2, 162, ":");
	line(312, 188, 512, 188);
	puthz(322, 200, "出水芙蓉", 16, 17, LIGHTBLUE);
	draw_element(322 + 17 * 5 + 4, 203, 1, LIGHTBLUE);//画按钮

	puthz(322, 200 + 30, "断头铡刀", 16, 17, LIGHTBLUE);
	draw_element(322 + 17 * 5 + 4, 203 + 30, 1, LIGHTBLUE);//画按钮

	setcolor(CYAN);
	line(0, 290, 24, 314);
	line(24, 314, 600, 314);
	line(24, 314, 24, 314 + 140);
	settextstyle(3, HORIZ_DIR, 1);
	outtextxy(2, 314, "MA");
	puthz(0, 314 + 24 + 6, "形", 24, 0, CYAN);
	puthz(0, 314 + 24 * 2 + 6 * 2, "态", 24, 0, CYAN);
	puthz(0, 314 + 24 * 3 + 6 * 3, "选", 24, 0, CYAN);
	puthz(0, 314 + 24 * 4 + 6 * 4, "股", 24, 0, CYAN);

	setcolor(WHITE);
	settextstyle(3, HORIZ_DIR, 1);
	puthz(45, 325, "多头排列", 16, 17, WHITE);
	draw_element(45 + 17 * 4 + 12, 332, 2, WHITE);//画按钮
	puthz(210, 325, "空头排列", 16, 17, WHITE);
	draw_element(210 + 17 * 4 + 12, 332, 2, WHITE);//画按钮

	puthz(45, 325 + 40, "粘合", 16, 17, YELLOW);
	draw_element(45 + 17 * 4 + 12, 332 + 40, 2, YELLOW);//画按钮
	puthz(210, 325 + 40, "发散", 16, 17, YELLOW);
	draw_element(210 + 17 * 4 + 12, 332 + 40, 2, YELLOW);//画按钮

	puthz(45, 325 + 40 * 2, "拐头向上", 16, 17, LIGHTMAGENTA);
	draw_element(45 + 17 * 4 + 12, 332 + 40 * 2, 2, LIGHTMAGENTA);//画按钮
	puthz(210, 325 + 40 * 2, "拐头向下", 16, 17, LIGHTMAGENTA);
	draw_element(210 + 17 * 4 + 12, 332 + 40 * 2, 2, LIGHTMAGENTA);//画按钮

	setcolor(WHITE);
	setfillstyle(SOLID_FILL, CYAN);
	bar3d(250, 452, 290, 479, 5, 1);
	puthz(255, 457, "选股", 16, 17, WHITE);

	setcolor(WHITE);
	setfillstyle(SOLID_FILL, BLUE);
	bar3d(560, 452, 620, 479, 5, 1);
	puthz(565, 457, "下一页", 16, 17, WHITE);

}

/*********************************************
FUNCTION:draw_T_stoselect1
DESCRIPTION：技术面选股第二页绘制
INPUT:void
RETURN:无
***********************************************/
void draw_T_stoselect2(void)
{
	setbkcolor(BLACK);
	setlinestyle(SOLID_LINE, 0, 3);
	setfillstyle(SOLID_FILL, WHITE);
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(2, HORIZ_DIR, 6);

	//KDJ
	setfillstyle(SOLID_FILL, WHITE);
	bar(0, 24, 260, 44);
	setcolor(RED);
	line(0, 46, 260, 46);
	setlinestyle(SOLID_LINE, 0, 1);
	line(0, 52, 260, 52);
	line(0, 58, 260, 58);
	puthz(5, 26, "随机指标", 16, 17, BLACK);
	outtextxy(10 + 17 * 4, 24, "KDJ");
	//KDJ指数与形态
	setcolor(WHITE);
	outtextxy(5, 58+4, "KD");
	puthz(24, 58 + 6, "金叉", 16, 17, WHITE);
	puthz(140, 58 + 6, "死叉", 16, 17, WHITE);
	outtextxy(5, 58 + 32 + 4, "J");
	puthz(15, 58 + 32 + 6, "超买", 16, 17, WHITE);
	puthz(140, 58 + 32 + 6, "超卖", 16, 17, WHITE);
	draw_element(24 + 90, 58 + 10, 1, RED);
	draw_element(144 + 90, 58 + 10, 1, RED);
	draw_element(24 + 90, 58 + 32 + 10, 1, RED);
	draw_element(144 + 90, 58 + 32 + 10, 1, RED);

	//MACD
	setfillstyle(SOLID_FILL, WHITE);
	bar(320, 24, 580, 44);
	setcolor(BLUE);
	setlinestyle(SOLID_LINE, 0, 3);
	line(320, 46, 580, 46);
	setlinestyle(SOLID_LINE, 0, 1);
	line(320, 52, 580, 52);
	line(320, 58, 580, 58);
	puthz(325, 26, "异同移动平均线", 16, 17, BLACK);
	outtextxy(330 + 17 * 7, 24, "MACD");
	//MACD指数与形态
	setcolor(WHITE);
	puthz(320, 58 + 6, "顶背离", 16, 17, WHITE);
	puthz(460, 58 + 6, "底背离", 16, 17, WHITE);
	puthz(320, 58 + 32 + 6, "金叉", 16, 17, WHITE);
	puthz(460, 58 + 32 + 6, "死叉", 16, 17, WHITE);
	draw_element(344 + 90, 58 + 10, 1, BLUE);
	draw_element(464 + 90, 58 + 10, 1, BLUE);
	draw_element(344 + 90, 58 + 32 + 10, 1, BLUE);
	draw_element(464 + 90, 58 + 32 + 10, 1, BLUE);


	//RSI
	setfillstyle(SOLID_FILL, WHITE);
	bar(0, 164, 260, 184);
	setcolor(BROWN);
	setlinestyle(SOLID_LINE, 0, 3);
	line(0, 186, 260, 186);
	setlinestyle(SOLID_LINE, 0, 1);
	line(0, 192, 260, 192);
	line(0, 198, 260, 198);
	puthz(5, 166, "相对强弱指数", 16, 17, BLACK);
	outtextxy(10 + 17 * 6, 164, "RSI");
	//RSI指数与形态
	setcolor(WHITE);
	puthz(5, 198 + 6, "超买", 16, 17, WHITE);
	outtextxy(10 + 17 * 2, 198 + 4, "(>80)");
	puthz(5, 198 + 32 + 6, "超卖", 16, 17, WHITE);
	outtextxy(10 + 17 * 2, 198 + 32+ 4, "(<20)");
	draw_element(24 + 90, 198 + 10, 1, BROWN);
	draw_element(24 + 90, 198 + 32 + 10, 1, BROWN);


	//VOL/OBV
	setfillstyle(SOLID_FILL, WHITE);
	bar(320, 164, 580, 184);
	setcolor(GREEN);
	setlinestyle(SOLID_LINE, 0, 3);
	line(320, 186, 580, 186);
	setlinestyle(SOLID_LINE, 0, 1);
	line(320, 192, 580, 192);
	line(320, 198, 580, 198);
	puthz(325, 166, "成交量", 16, 17, BLACK);
	setcolor(BLACK);
	outtextxy(326 + 17 * 3, 164, "/");
	puthz(386, 166, "能量潮", 16, 17, BLACK);
	setcolor(GREEN);
	outtextxy(391 + 17 * 3, 164, "VOL/OBV");
	//VOL/OBV指数与形态
	setcolor(WHITE);
	puthz(320, 198 + 6, "上涨趋势", 16, 17, WHITE);
	puthz(320, 198 + 32 + 6, "下跌趋势", 16, 17, WHITE);
	draw_element(344 + 90, 198 + 10, 1, GREEN);
	draw_element(344 + 90, 198 + 32 + 10, 1, GREEN);


	//BIAS
	setfillstyle(SOLID_FILL, WHITE);
	bar(0, 304, 260, 324);
	setcolor(LIGHTMAGENTA);
	setlinestyle(SOLID_LINE, 0, 3);
	line(0, 326, 260, 326);
	setlinestyle(SOLID_LINE, 0, 1);
	line(0, 332, 260, 332);
	line(0, 338, 260, 338);
	puthz(5, 306, "乖离率", 16, 17, BLACK);
	outtextxy(10 + 17 * 3, 304, "BIAS");
	//BIAS指数与形态
	setcolor(WHITE);
	puthz(5, 338 + 6, "超买", 16, 17, WHITE);
	outtextxy(10 + 17 * 2, 338 + 4, "(>16%)");
	puthz(5, 338 + 32 + 6, "超卖", 16, 17, WHITE);
	outtextxy(10 + 17 * 2, 338 + 32 + 4, "(<-16%)");
	draw_element(24 + 90, 338 + 10, 1, LIGHTMAGENTA);
	draw_element(24 + 90, 338 + 32 + 10, 1, LIGHTMAGENTA);


	//BOLL
	setfillstyle(SOLID_FILL, WHITE);
	bar(320, 304, 580, 324);
	setcolor(LIGHTCYAN);
	setlinestyle(SOLID_LINE, 0, 3);
	line(320, 326, 580, 326);
	setlinestyle(SOLID_LINE, 0, 1);
	line(320, 332, 580, 332);
	line(320, 338, 580, 338);
	puthz(325, 306, "布林带", 16, 17, BLACK);
	outtextxy(330 + 17 * 3, 304, "BOLL");
	//BOLL指数与形态
	setcolor(WHITE);
	puthz(320, 338 + 32 + 6, "突破", 16, 17, WHITE);
	line(325 + 17 * 2, 338 + 6 + 8, 325 + 17 * 2 + 20, 338 + 6 + 8);
	puthz(325 + 54 + 5, 338 + 6, "上轨", 16, 17, WHITE);
	line(325 + 17 * 2, 338 + 32 + 6 + 8, 325 + 17 * 2 + 20, 338 + 32 + 6 + 8);
	puthz(325 + 54 + 5, 338 + 32 + 6, "中轨", 16, 17, WHITE);
	line(325 + 17 * 2, 338 + 64 + 6 + 8, 325 + 17 * 2 + 20, 338 + 64 + 6 + 8);
	puthz(325 + 54 + 5, 338 + 64 + 6, "下轨", 16, 17, WHITE);
	line(325 + 34, 338 + 14, 325 + 34, 338 + 64 + 14);
	draw_element(344 + 90, 338 + 10, 1, LIGHTCYAN);
	draw_element(344 + 90, 338 + 32 + 10, 1, LIGHTCYAN);
	draw_element(344 + 90, 338 + 64 + 10, 1, LIGHTCYAN);

	setcolor(WHITE);
	setfillstyle(SOLID_FILL, LIGHTMAGENTA);
	bar3d(250, 452, 290, 479, 5, 1);
	puthz(255, 457, "选股", 16, 17, WHITE);


	setcolor(WHITE);
	setfillstyle(SOLID_FILL, RED);
	bar3d(20, 452, 80, 479, 5, 1);
	puthz(25, 457, "上一页", 16, 17, WHITE);
}


/*********************************************
FUNCTION:draw_element
DESCRIPTION：技术面选股元素绘制
INPUT:x,y,state,color
RETURN:无
***********************************************/
void draw_element(int x, int y, int state,int color)
{
	switch (state)
	{
	case 1:
		setfillstyle(SOLID_FILL, color);
		bar3d(x, y, x + 10, y + 10, 5, 1);
		break;
	case 2:
		setcolor(color);
		circle(x, y, 5);
		break;
	}
}