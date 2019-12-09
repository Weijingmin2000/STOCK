#include"public.h"
#include"lgfunc.h"
#include"transfer.h"
#include"database.h"

/*****************************************
lgfunc.c
COPYRIGHT:	weijingmin
FUNCTION:	money transfer
ABSTRACT:
		A.transfer control
		B.button lightning and recovering
DATE:2019/11/6
*******************************************/

/*********************************************
FUNCTION:transfer_money
DESCRIPTION：画忘记密码界面
INPUT:u
RETURN:无
***********************************************/
void transfer_money(USER *u)
{
	int state = 0;
	int state1 = 0;
	int state2 = 0;
	char M[8] = { '\0' };//转账金额
	char P[8] = { '\0' };//密码
	clrmous(MouseX, MouseY);
	delay(10);
	cleardevice();
	draw_transfer();

	while (1)
	{
		newmouse(&MouseX, &MouseY, &press);
		if (MouseX > 230 && MouseX < 230 + 380 && MouseY > 120 + 30 && 120 + 60)
		{
			if (mouse_press(230, 120 + 30, 230 + 380, 120 + 60) == 2)//金额框未点击
			{
				MouseS = 2;
				if (state == 0 && state1 == 0)//防止被重复标亮
				{
					lightbutton_transfer(230, 120 + 30, 230 + 380, 120 + 60, WHITE, DARKGRAY, 1);//color1为边框色，color2为填充色
					state = 1;
				}
				continue;

				//标亮函数
			}
			else if (mouse_press(230, 120 + 30, 230 + 380, 120 + 60) == 1)//金额框点击
			{
				MouseS = 0;
				lightbutton_transfer(230, 120 + 30, 230 + 380, 120 + 60, WHITE, DARKGRAY, 1);
				M[0] = '\0';
				inputadmin(M, 230, 120 + 30, 7, DARKGRAY);
				//输入函数
				if (strlen(M) != 0)
					state1 = 1;
				else
					state1 = 0;
				continue;
			}
		}
		if (MouseX > 230 && MouseX < 230 + 380 && MouseY>240 + 30 && 240 + 60)
		{
			if (mouse_press(230, 240 + 30, 230 + 380, 240 + 60) == 2)//密码框未点击
			{
				MouseS = 2;
				if (state == 0 && state2 == 0)
				{
					lightbutton_transfer(230, 240 + 30, 230 + 380, 240 + 60, WHITE, DARKGRAY, 2);//标亮函数
					state = 2;
				}
				continue;
			}
			else if (mouse_press(230, 240 + 30, 230 + 380, 240 + 60) == 1)//密码框点击
			{
				MouseS = 0;
				lightbutton_transfer(230, 240 + 30, 230 + 380, 240 + 60, WHITE, DARKGRAY, 2);
				P[0] = '\0';
				inputpassword(P, 230, 240 + 30, 6, DARKGRAY);
				if (strlen(P) != 0)
					state2 = 1;
				else
					state2 = 0;
				continue;
				//输入函数
			}
		}
		if (MouseX > 200 + 220 - 120 && MouseX < 200 + 220 - 50 && MouseY>360 + 30 && 360 + 30 + 35)
		{
			if (mouse_press(200 + 220 - 120, 360 + 30, 200 + 220 - 50, 360 + 30 + 35) == 2)//转账框未点击
			{
				MouseS = 1;
				if (state == 0)
				{
					lightbutton_transfer(200 + 220 - 120, 360 + 30, 200 + 220 - 50, 360 + 30 + 35, WHITE, RED, 3);//标亮函数
					state = 3;
				}
				continue;
			}
			else if (mouse_press(200 + 220 - 120, 360 + 30, 200 + 220 - 50, 360 + 30 + 35) == 1)//转账框点击
			{
				delay(300);
				MouseS = 0;
				transfer_judge(M, P, u);//检查用户是否能转账，若能则转账
				continue;
			}
		}
		if (MouseX > 200 + 220 + 50 && MouseX < 200 + 220 + 120 && MouseY>360 + 30 && 360 + 30 + 35)
		{
			if (mouse_press(200 + 220 + 50, 360 + 30, 200 + 220 + 120, 360 + 30 + 35) == 2)//返回框未点击
			{
				MouseS = 1;
				if (state == 0)
				{
					lightbutton_transfer(200 + 220 + 50, 360 + 30, 200 + 220 + 120, 360 + 30 + 35, WHITE, BLUE, 4);//标亮函数
					state = 4;
				}
				continue;
			}
			else if (mouse_press(200 + 220 + 50, 360 + 30, 200 + 220 + 120, 360 + 30 + 35) == 1)//返回框点击
			{
				MouseS = 0;
				break;
				//切换至个人界面
			}
		}
		if (state != 0)
		{
			MouseS = 0;
			if (state1 == 0)
			{
				recoverbutton_transfer(1);//恢复按钮颜色
			}
			if (state2 == 0)
			{
				recoverbutton_transfer(2);
			}
			if (state == 3 || state == 4 || state == 5)
			{
				recoverbutton_transfer(state);
			}
			state = 0;
		}
		if (MouseS != 0)
		{
			MouseS = 0;
		}
	}
	return;
}

/*********************************************
FUNCTION:lightbutton_transfer
DESCRIPTION：转账界面按钮点亮
INPUT:x,y,x1,y1,color1,color2,flag
RETURN:无
***********************************************/
void lightbutton_transfer(int x, int y, int x1, int y1, int color1, int color2, int flag)
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
		break;
	case 2:
		break;
	case 3:
		puthz(420 - 110, 360 + 35, "转账", 24, 28, LIGHTRED);
		break;
	case 4:
		puthz(420 + 60, 360 + 35, "返回", 24, 28, LIGHTBLUE);
		break;
	default:
		closegraph();
		printf("something runs wrong in lightbutton");
		exit(1);
	}
}

/*********************************************
FUNCTION:recoverbutton_transfer
DESCRIPTION：转账界面按钮点亮恢复
INPUT:status
RETURN:无
***********************************************/
void recoverbutton_transfer(int status)
{
	clrmous(MouseX, MouseY);
	if (status == 1 || status == 2)
	{
		setfillstyle(SOLID_FILL, LIGHTGRAY);
		setlinestyle(SOLID_LINE, 0, 1);
	}
	else if (status == 3)
	{
		setfillstyle(SOLID_FILL, RED);
	}
	else if (status == 4)
	{
		setfillstyle(SOLID_FILL, BLUE);
	}
	switch (status)
	{
	case 1:
		bar(230, 120 + 30, 230 + 380, 120 + 60);
		puthz(230 + 15, 120 + 38, "请输入转账金额", 16, 20, WHITE);
		break;
	case 2:
		bar(230, 240 + 30, 230 + 380, 240 + 60);
		puthz(230 + 15, 240 + 38, "请输入银行卡密码", 16, 20, WHITE);
		break;
	case 3:
		bar(200 + 220 - 120, 360 + 30, 200 + 220 - 50, 360 + 30 + 35);
		puthz(420 - 110, 360 + 35, "转账", 24, 28, WHITE);
		break;
	case 4:
		bar(200 + 220 + 50, 360 + 30, 200 + 220 + 120, 360 + 30 + 35);
		puthz(420 + 60, 360 + 35, "返回", 24, 28, WHITE);
		break;
	}
}

/*********************************************
FUNCTION:draw_transfer
DESCRIPTION：画转账界面
INPUT:void
RETURN:无
***********************************************/
void draw_transfer(void)
{
	int i;
	setbkcolor(WHITE);
	setfillstyle(SOLID_FILL, LIGHTGRAY);
	setcolor(LIGHTGRAY);
	settextstyle(TRIPLEX_FONT, HORIZ_DIR, 1);
	settextjustify(LEFT_TEXT, CENTER_TEXT);

	bar(230, 120 + 30, 230 + 380, 120 + 60);
	outtextxy(230, 120 + 15, "amout");
	puthz(230 + 15, 120 + 38, "请输入转账金额", 16, 20, WHITE);

	bar(230, 240 + 30, 230 + 380, 240 + 60);
	outtextxy(230, 240 + 15, "password");
	puthz(230 + 15, 240 + 38, "请输入银行卡密码", 16, 20, WHITE);

	setfillstyle(SOLID_FILL, RED);
	bar(200 + 220 - 120, 360 + 30, 200 + 220 - 50, 360 + 30 + 35);
	puthz(420 - 110, 360 + 35, "转账", 24, 28, WHITE);

	setfillstyle(SOLID_FILL, BLUE);
	bar(200 + 220 + 50, 360 + 30, 200 + 220 + 120, 360 + 30 + 35);
	puthz(420 + 60, 360 + 35, "返回", 24, 28, WHITE);

	setfillstyle(SOLID_FILL, RED);
	setlinestyle(SOLID_LINE, 0, 1);
	setcolor(RED);
	/*line(200, 0, 200, 480);

	line(100, 50, 30, 280);
	line(100, 50, 200 - 30, 280);
	line(100, 100, 100 - 21, 100 + 70);
	line(100, 100, 100 + 21, 100 + 70);
	line(100 - 21, 100 + 70, 100 + 21, 100 + 70);
	line(100 - 66 + 37, 200, 100 + 66 - 37, 200);
	line(100 - 66 + 37, 200, 55, 260);
	line(100 + 66 - 37, 200, 145, 260);
	line(55, 260, 70, 260);
	line(145, 260, 130, 260);
	line(70, 260, 30, 280);
	line(130, 260, 170, 280);
	floodfill(100, 70, RED);
	puthz(100 - 24, 260, "股", 48, 0, RED);*/

	for (i = 0; i < 7; i++)
	{
		line(25, 100 + i, 200, 100 + i);
		line(25, 170 + i, 200, 170 + i);
	}
	for (i = 0; i < 7; i++)
	{
		line(25 + i, 100 + i, 65 + i, 70 + i);
		line(200 + i, 177 + i, 160 + i, 207 + i);
	}
	
	puthz(90, 110, "￥", 48, 0, RED);

	setlinestyle(SOLID_LINE, 0, 1);
	setcolor(RED);
	setfillstyle(1, RED);
	fillellipse(420, 65, 25, 25);
	setfillstyle(1, WHITE);
	pieslice(420, 65, 230, 310, 25);
	fillellipse(420, 65 - 5, 12, 12);
	setcolor(WHITE);
	circle(420, 65 - 5, 12);		//画登录小人
}