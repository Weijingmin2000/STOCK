#include"public.h"
#include"clock.h"
#define pi 3.1415926 /*定义pi=3.14159*/


/*****************************************
clock.c
COPYRIGHT:	weijingmin
FUNCTION:	clock
ABSTRACT:
		A.show clock
DATE:2019/11/5
******************************************/

/*********************************************
FUNCTION:clock_show
DESCRIPTION：显示时钟
INPUT:void
RETURN:无
***********************************************/
void clock_show(void)
{
	unsigned char h, m;
	struct time t[1];//bc内置结构体
	setwritemode(1);
	gettime(t);
	h = t[0].ti_hour;
	m = t[0].ti_min;
	//s = t[0].ti_sec;
	setcolor(YELLOW);
	clock_change(80, h, 30);
	setcolor(LIGHTMAGENTA);
	clock_change(105, m, 6);
	delay(100);
}

/*********************************************
FUNCTION:clock_get
DESCRIPTION：获得时间并更新
INPUT:h,m
RETURN:无
***********************************************/
void clock_get(unsigned char *h, unsigned char *m)
{
	struct time t[1];
	setwritemode(1);
	gettime(t);
	if (t[0].ti_min != *m)
	{
		setcolor(LIGHTMAGENTA);
		clock_change(105, *m, 6);
		*m = t[0].ti_min;
		clock_change(105, *m, 6);
	}
	if (t[0].ti_hour != *h)
	{
		setcolor(YELLOW);
		clock_change(80, *h, 30);
		*h = t[0].ti_hour;
		clock_change(80, *h, 30);
	}
}

/*********************************************
FUNCTION:clock_change
DESCRIPTION：改变时钟
INPUT:a,b,c
RETURN:无
***********************************************/
void clock_change(int a, int b, int c)//a为半径，b为时分秒，c为换算机制
{
	int x, y;
	x = a * cos(b * c * pi / 180 - pi / 2) + 320;
	y = a * sin(b * c * pi / 180 - pi / 2) + 160;
	line(320, 160, x, y);
}

/*********************************************
FUNCTION:clock_change
DESCRIPTION：画时钟
INPUT:void
RETURN:无
***********************************************/
void clock_draw(void)
{
	int i, l, x1, x2, y1, y2; 
	setbkcolor(BLACK); 
	setlinestyle(SOLID_LINE, 0, 1);
	setcolor(WHITE);
	circle(320, 160, 120); 
	setcolor(RED);
	circle(320, 160, 105);
	setcolor(WHITE);
	circle(320, 160, 5);

	for (i = 0; i < 60; i++)
	{
		if (i % 5 == 0) l = 15;
		else l = 5;
		x1 = 120 * cos(i * 6 * pi / 180) + 320;
		y1 = 120 * sin(i * 6 * pi / 180) + 160;
		x2 = (120 - l) * cos(i * 6 * pi / 180) + 320;
		y2 = (120 - l) * sin(i * 6 * pi / 180) + 160;
		line(x1, y1, x2, y2);
	}
}