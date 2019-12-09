#include"public.h"
#include"forget.h"
#include"lgfunc.h"

/*****************************************
forgetpass.c
COPYRIGHT:	weijingmin
FUNCTION:	find password
ABSTRACT:
		A.change password 
		B.button lightning and recovering
DATE:2019/9/8
******************************************/


/*********************************************
FUNCTION:user_forgetpass
DESCRIPTION：忘记密码界面
INPUT:func
RETURN:无
***********************************************/
void user_forgetpass(int *func)	
{
	int num;
	char U[15] = { '\0' };//6-12位，用户名
	char I[20] = { '\0' };//18位，身份证
	char NP[20] = { '\0' };//8-16位，新密码
	char CP[20] = { '\0' };//8-16位，确认密码
	int state1 = 0;
	int state2 = 0;
	int state3 = 0;
	int state4 = 0;
	clrmous(MouseX, MouseY);
	delay(100);
	cleardevice();
	draw_forgetpass();

	while (1)
	{
		newmouse(&MouseX, &MouseY, &press);
		if (MouseX > 95 && MouseX < 95 + 450 && MouseY>130 && MouseY < 130 + 30)
		{
			if (mouse_press(95, 130, 95 + 450, 130 + 30) == 2)//用户名框未点击
			{
				MouseS = 2;
				if (num == 0 && state1 == 0)//防止被重复标亮
				{
					lightbutton_forget(95, 130, 95 + 450, 130 + 30, LIGHTBLUE, DARKGRAY, 1);//color1为边框色，color2为填充色
					num = 1;
				}
				continue;

				//标亮函数
			}
			else if (mouse_press(95, 130, 95 + 450, 130 + 30) == 1)//用户名框点击
			{
				MouseS = 0;
				lightbutton_forget(95, 130, 95 + 450, 130 + 30, LIGHTBLUE, DARKGRAY, 1);
				U[0] = '\0';
				inputadmin(U, 95, 130, 12, DARKGRAY);
				if(strlen(U) != 0)
					state1 = 1;	
				else 
					state1 = 0;
				continue;
				//输入函数
			}
		}
		if (MouseX > 95 && MouseX < 95 + 450 && MouseY>180 + 30 && MouseY < 180 + 60)
		{
			if (mouse_press(95, 180 + 30, 95 + 450, 180 + 60) == 2)//身份证未点击
			{
				MouseS = 2;
				if (num == 0 && state2 == 0)
				{
					lightbutton_forget(95, 180 + 30, 95 + 450, 180 + 60, LIGHTBLUE, DARKGRAY, 1);//标亮函数
					num = 2;
				}
				continue;
			}
			else if (mouse_press(95, 180 + 30, 95 + 450, 180 + 60) == 1)//身份证点击
			{
				MouseS = 0;
				lightbutton_forget(95, 180 + 30, 95 + 450, 180 + 60, LIGHTBLUE, DARKGRAY, 1);
				I[0] = '\0';
				inputadmin(I, 95, 180 + 30, 18, DARKGRAY);
				if(strlen(I) != 0)
					state2 = 1;
				else 
					state2 = 0;
				continue;
				//输入函数
			}
		}
		if (MouseX > 95 && MouseX < 95 + 450 && MouseY>260 + 30 && MouseY < 260 + 60)
		{
			if (mouse_press(95, 260 + 30, 95 + 450, 260 + 60) == 2)//新密码框未点击
			{
				MouseS = 2;
				if (num == 0 && state3 == 0)
				{
					lightbutton_forget(95, 260 + 30, 95 + 450, 260 + 60, LIGHTBLUE, DARKGRAY, 1);//标亮函数
					num = 3;
				}
				continue;
			}
			else if (mouse_press(95, 260 + 30, 95 + 450, 260 + 60) == 1)//新密码框点击
			{
				MouseS = 0;
				lightbutton_forget(95, 260 + 30, 95 + 450, 260 + 60, LIGHTBLUE, DARKGRAY, 1);
				NP[0] = '\0';
				inputpassword(NP, 95, 260 + 30, 16, DARKGRAY);
				if(strlen(NP) != 0)
					state3 = 1;
				else 
					state3 = 0;
				continue;
				//输入函数
			}
		}
		if (MouseX > 95 && MouseX < 95 + 450 && MouseY>340 + 30 && MouseY < 340 + 60)
		{
			if (mouse_press(95, 340 + 30, 95 + 450, 340 + 60) == 2)//确认密码框未点击
			{
				MouseS = 2;
				if (num == 0 && state4 == 0)
				{
					lightbutton_forget(95, 340 + 30, 95 + 450, 340 + 60, LIGHTBLUE, DARKGRAY, 1);//标亮函数
					num = 4;
				}
				continue;
			}
			else if (mouse_press(95, 340 + 30, 95 + 450, 340 + 60) == 1)//确认密码框点击
			{
				MouseS = 0;
				lightbutton_forget(95, 340 + 30, 95 + 450, 340 + 60, LIGHTBLUE, DARKGRAY, 1);
				CP[0] = '\0';
				inputpassword(CP, 95, 340 + 30, 16, DARKGRAY);
				if(strlen(CP) != 0)
					state4 = 1;
				else 
					state4 = 0;
				continue;
				//输入函数

			}
		}
		if (MouseX > 150 && MouseX < 150 + 80 && MouseY>480 - 10 - 40 && MouseY < 480 - 10)
		{
			if (mouse_press(150, 480 - 10 - 40, 150 + 80, 480 - 10) == 2)//完成框未点击
			{
				if (num == 0)
				{
					MouseS = 1;
					lightbutton_forget(150, 480 - 10 - 40, 150 + 80, 480 - 10, RED, LIGHTRED, 2);//标亮函数
					num = 5;
				}
				continue;
			}
			else if (mouse_press(150, 480 - 10 - 40, 150 + 80, 480 - 10) == 1)//完成框点击
			{
				MouseS = 0;
				if (forgetpass_complete(U, I, NP, CP))
				{
					*func = 2;
					return;
				}
				else
				{
					continue;
				}
				//检查是否可以修改密码并录入数据库
				//切换至登录界面
			}
		}
		if (MouseX > 640 - 100 - 130 && MouseX < 640 - 150 && MouseY>480 - 50 && MouseY < 480 - 10)
		{
			if (mouse_press(640 - 100 - 130, 480 - 50, 640 - 150, 480 - 10) == 2)//返回框未点击
			{
				if (num == 0)
				{
					MouseS = 1;
					lightbutton_forget(640 - 100 - 130, 480 - 50, 640 - 150, 480 - 10, BLUE, LIGHTBLUE, 3);
					num = 6;
				}
				continue;
				//标亮函数
			}
			else if (mouse_press(640 - 100 - 130, 480 - 50, 640 - 150, 480 - 10) == 1)//返回框点击
			{
				MouseS = 0;
				*func = 2;
				//切换登录界面
				return;
			}
		}
		if (MouseX > 320 - 40 && MouseX < 320 + 40 && MouseY>480 - 50 && MouseY < 480 - 10)
		{
			if (mouse_press(320 - 40, 480 - 50, 320 + 40, 480 - 10) == 2)//重置框未点击
			{
				if (num == 0)
				{
					MouseS = 1;
					lightbutton_forget(320 - 40, 480 - 50, 320 + 40, 480 - 10, CYAN, LIGHTCYAN, 4);
					num = 7;
				}
				continue;
				//标亮函数
			}
			else if (mouse_press(320 - 40, 480 - 50, 320 + 40, 480 - 10) == 1)//返回框点击
			{
				MouseS = 0;
				cleardevice();
				draw_forgetpass();
				num = 0;
				state1 = 0;
				state2 = 0;
				state3 = 0;
				state4 = 0;
				U[0] = '\0';
				I[0] = '\0';
				NP[0] = '\0';
				CP[0] = '\0';
				//重置
				continue;
			}
		}
		if (num != 0)
		{
			MouseS = 0;
			if (state1 == 0)
			{
				recoverbutton_forget(1);
			}
			if (state2 == 0)
			{
				recoverbutton_forget(2);
			}
			if (state3 == 0)
			{
				recoverbutton_forget(3);
			}
			if (state4 == 0)
			{
				recoverbutton_forget(4);
			}
			if (num == 5 || num == 6 || num == 7)
			{
				recoverbutton_forget(num);//恢复按钮颜色
			}
			num = 0;
		}
		if (MouseS != 0)
		{
			MouseS = 0;
		}

	}
}

/*********************************************
FUNCTION:lightbutton_forget
DESCRIPTION：忘记密码界面按钮点亮
INPUT:x,y,x1,y1,color1,color2,flag
RETURN:无
***********************************************/
void lightbutton_forget(int x, int y, int x1, int y1, int color1, int color2, int flag)
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
		puthz(160, 440, "完成", 24, 34, RED);
		break;
	case 3:
		puthz(420, 440, "返回", 24, 34, BLUE);
		break;
	case 4:
		puthz(290, 440, "重置", 24, 34, CYAN);
		break;
	default:
		closegraph();
		printf("something runs wrong in lightbutton");
		exit(1);
	}
}

/*********************************************
FUNCTION:recoverbutton_forget
DESCRIPTION：忘记密码界面按钮恢复
INPUT:status
RETURN:无
***********************************************/
void recoverbutton_forget(int status)
{
	clrmous(MouseX, MouseY);
	setcolor(WHITE);
	if (status == 1 || status == 2|| status==3|| status==4)
	{
		setfillstyle(SOLID_FILL, LIGHTGRAY);
		setcolor(WHITE);
		setlinestyle(SOLID_LINE, 0, 1);
	}
	else if (status == 5)
	{
		setfillstyle(SOLID_FILL, RED);
	}
	else if (status == 6)
	{
		setfillstyle(SOLID_FILL, BLUE);
	}
	else if (status == 7)
	{
		setfillstyle(SOLID_FILL, CYAN);
	}
	switch (status)
	{
	case 1:
		bar(95, 130, 95 + 450, 130 + 30);
		rectangle(95, 130, 95 + 450, 130 + 30);
		break;
	case 2:
		bar(95, 180 + 30, 95 + 450, 180 + 60);
		rectangle(95, 180 + 30, 95 + 450, 180 + 60);
		break;
	case 3:
		bar(95, 260 + 30, 95 + 450, 260 + 60);
		rectangle(95, 260 + 30, 95 + 450, 260 + 60);
		break;
	case 4:
		bar(95, 340 + 30, 95 + 450, 340 + 60);
		rectangle(95, 340 + 30, 95 + 450, 340 + 60);
		break;
	case 5:
		bar(150, 480 - 10 - 40, 150 + 80, 480 - 10);
		rectangle(150, 480 - 10 - 40, 150 + 80, 480 - 10);
		puthz(160, 440, "完成", 24, 34, WHITE);
		break;
	case 6:
		bar(640 - 100 - 130, 480 - 50, 640 - 150, 480 - 10);
		rectangle(640 - 100 - 130, 480 - 50, 640 - 150, 480 - 10);
		puthz(420, 440, "返回", 24, 34, WHITE);
		break;
	case 7:
		bar(320 - 40, 480 - 50, 320 + 40, 480 - 10);
		rectangle(320 - 40, 480 - 50, 320 + 40, 480 - 10);
		puthz(320 - 30, 440, "重置", 24, 34, WHITE);
		break;
	}
}

/*********************************************
FUNCTION:draw_forgetpass
DESCRIPTION：画忘记密码界面
INPUT:void
RETURN:无
***********************************************/
void draw_forgetpass(void)
{
	setbkcolor(WHITE);
	setfillstyle(SOLID_FILL, BLUE);
	fillellipse(65, 45, 15, 15);
	fillellipse(235, 45, 15, 15);
	fillellipse(405, 45, 15, 15);
	fillellipse(575, 45, 15, 15);
	setcolor(LIGHTGRAY);
	setlinestyle(SOLID_LINE, 0, 3);
	line(80, 45, 220, 45);
	line(250, 45, 390, 45);
	line(420, 45, 560, 45);

	settextstyle(TRIPLEX_FONT, HORIZ_DIR, 1);
	settextjustify(LEFT_TEXT, CENTER_TEXT);
	outtextxy(62, 42, "1");
	outtextxy(232, 42, "2");
	outtextxy(402, 42, "3");
	outtextxy(572, 42, "4");

	puthz(20, 65, "输入用户名", 16, 20, BLUE);
	puthz(190, 65, "输入身份证", 16, 20, BLUE);
	puthz(360, 65, "输入新密码", 16, 20, BLUE);
	puthz(540, 65, "确认密码", 16, 20, BLUE);

	setfillstyle(SOLID_FILL, LIGHTGRAY);

	outtextxy(95, 115, "user");
	bar(95, 130, 95 + 450, 130 + 30);

	outtextxy(95, 180+15, "card number");
	bar(95, 180 + 30, 95 + 450, 180 + 60);

	outtextxy(95, 260+15, "new password");
	bar(95, 260 + 30, 95 + 450, 260 + 60);

	outtextxy(95, 340+15, "confirmed password");
	bar(95, 340 + 30, 95 + 450, 340 + 60);

	setfillstyle(SOLID_FILL, RED);
	bar(150, 480 - 10 - 40, 150 + 80, 480 - 10);
	puthz(160, 440, "完成", 24, 34, WHITE);

	setfillstyle(SOLID_FILL, BLUE);
	bar(640 - 100 - 130, 480 - 50, 640 - 150, 480 - 10);
	puthz(420, 440, "返回", 24, 34, WHITE);

	setfillstyle(SOLID_FILL, CYAN);
	bar(320 - 40, 480 - 50, 320 + 40, 480 - 10);
	puthz(320-30, 440, "重置", 24, 34, WHITE);
}