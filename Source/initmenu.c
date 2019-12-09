#include"public.h"
#include"initmenu.h"
#include"clock.h"


/*****************************************
lgfunc.c
COPYRIGHT:	dengshumin and weijingmin
FUNCTION:	start with this first screen
ABSTRACT:
		A.change user, admin and passerby
DATE:2019/8/30
*******************************************/


void initmenu_set(int *func)	//初始化选择界面
{
	unsigned char h, m;
	int num=0;	//button的序号
	struct time t[1];
	clrmous(MouseX, MouseY);
	delay(100);
	cleardevice();
	gettime(t);
	h = t[0].ti_hour;
	m = t[0].ti_min;
	drawmenu();
	clock_draw();//画钟表
	clock_show();//初始化显示
	while (1)
	{
		newmouse(&MouseX, &MouseY, &press);
		clock_get(&h, &m);//实时显示时钟
		if(mouse_press(80, 350, 80 + 160, 350 + 60) == 2)//用户框中未点击
		//（设置它为最先的if条件是因为如果点击在未点击之前，则点击为无效操作，函数会直接往下运行）
		{
			if (num == 0)
			{
				MouseS = 1;
				lightbutton_initmenu(80, 350, 80 + 160, 350 + 60, WHITE, LIGHTRED, 1);//color1为边框色，color2为填充色
				num = 1;
			}
			continue;//标亮函数
		}
		else if (mouse_press(80, 350, 80 + 160, 350 + 60) == 1)//用户框中点击
		{
			MouseS = 0;
			*func = 2;
			return;
		}

		else if (mouse_press(400, 350, 560, 350 + 60) == 2)//游客框中未点击
		{
			if (num == 0)
			{
				MouseS = 1;
				lightbutton_initmenu(400, 350, 560, 350 + 60, WHITE, LIGHTRED, 3);
				num = 3;
			}
			continue;//标亮函数
		}
		else if (mouse_press(400, 350, 560, 350 + 60) == 1)//游客框中点击
		{
			MouseS = 0;
			*func = 6;
			break;
		}

		if(mouse_press(640 - 30, 0, 640, 20) == 2)//退出按钮
		{
			if (num == 0)
			{
				lightbutton_initmenu(640 - 30, 20, 640, 0, RED, RED, 4);
				num = 4;
			}
			continue;//标亮函数
		}
		else if(mouse_press(640 - 30, 0, 640, 20) == 1)
		{
			*func = 1;
			return;
		}
		else
		{
			if (num != 0)
			{
				MouseS = 0;
				recoverbutton_initmenu(num);
				num = 0;
			}
			continue;
		}
		
	}
}

void lightbutton_initmenu(int x, int y, int x1, int y1, int color1, int color2, int flag)
{
	clrmous(MouseX, MouseY);
	delay(10);
	setcolor(color1);
	setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
	setfillstyle(1, color2);
	bar(x, y, x1, y1);
	rectangle(x, y, x1, y1);	//画边框
	switch (flag)
	{
	case 1:
		puthz(160 - 17 - 16, 380 - 16, "用户", 32, 34, LIGHTGRAY);
		break;
	case 3:
		puthz(480 - 17 - 16, 380 - 16, "游客", 32, 34, LIGHTGRAY);
		break;
	case 4:
		setcolor(LIGHTRED);
		line(640 - 25, 2, 640 - 5, 2 + 16);
		line(640 - 25, 2 + 16, 640 - 5, 2);
		break;
	default:
		closegraph();
		printf("something runs wrong in lightbutton");
		exit(1);
	}
}

void recoverbutton_initmenu(int num)
{
	setfillstyle(1, RED);
	clrmous(MouseX, MouseY);
	setcolor(RED);
	setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
	switch (num)
	{
	case 1:
		rectangle(80, 350, 80 + 160, 350 + 60);
		bar(80, 350, 80 + 160, 350 + 60);
		puthz(160 - 17 - 16, 380 - 16, "用户", 32, 34, WHITE);
	case 3:
		rectangle(400, 350, 560, 350 + 60);
		bar(400, 350, 560, 350 + 60);
		puthz(480 - 17 - 16, 380 - 16, "游客", 32, 34, WHITE);
	case 4:
		setfillstyle(1, WHITE);	//退出按钮
		bar(640 - 30, 20, 640, 0);
		setlinestyle(SOLID_LINE, 0, 1);
		setcolor(RED);
		line(640 - 25, 2, 640 - 5, 2 + 16);
		line(640 - 25, 2 + 16, 640 - 5, 2);
		break;
	}
}

void drawmenu(void)
{
	setbkcolor(BLACK);
	setlinestyle(SOLID_LINE,0,3);
	setcolor(DARKGRAY);
	line(0,320,640,320);	//分界线

	setfillstyle(1,RED);
	bar(80,350,80+160,350+60);	//用户
	puthz(160-17-16,380-16, "用户", 32, 34, WHITE);

	/*bar(240,350,240+160,350+60);	//管理员
	puthz(320-34-16,380-16, "管理员", 32, 34, WHITE);*/

	bar(400,350,560,350+60);	//游客
	puthz(480-17-16,380-16,"游客", 32, 34, WHITE);

	setfillstyle(1,WHITE);	//退出按钮
	bar(640-30,20,640,0);
	setlinestyle(SOLID_LINE, 0, 1);
	setcolor(RED);
	line(640-25,2,640-5,2+16);
	line(640 - 25, 2 + 16, 640 - 5, 2);

	setfillstyle(1, LIGHTGRAY);//版权与制作者
	bar(0,458,640,480);
	setcolor(WHITE);
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(1, HORIZ_DIR, 2);
	outtextxy(2, 480 - 24, "Copyright by wjm and dsm");

}