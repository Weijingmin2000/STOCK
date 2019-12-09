#include"public.h"
#include"about.h"

/*****************************************
about.c
COPYRIGHT:	weijingmin
FUNCTION:	detail infomation for software
ABSTRACT:
		A.show about
		B.show help
DATE:2019/11/6
******************************************/


/*********************************************
FUNCTION:About
DESCRIPTION：关于界面总控
INPUT:cinfo,u
RETURN:无
***********************************************/
void About(Ctr* cinfo, USER* u)//关于
{
	int num = 0;//按钮标号
	int cnt = 0;
	memset(cinfo, 0, sizeof(Ctr));
	cinfo->func = 7;            //用7标记关于函数
	clrmous(MouseX, MouseY);
	cleardevice();
	mainmenu_draw();
	control_judge(u);
	About_show();
	while (1)
	{
		if (!bioskey(1))
		{
			newmouse(&MouseX, &MouseY, &press);
			control_mouse(cinfo, u);
		}
		else if (bioskey(1))
		{
			cinfo->key = bioskey(0);
		}
		control_key(cinfo);
		if (control_menu(cinfo))
		{
			cleardevice();
			mainmenu_draw();
			control_judge(u);
			submenu_draw(cinfo->itemnum);
			About_show();
			continue;
		}

		if (cinfo->func != 7 && cinfo->func != 15)
		{
			clrmous(MouseX, MouseY);
			delay(10);
			return;
		}

		if (MouseX > 590 && MouseX < 630 && MouseY > 400 && MouseY < 420)//转账
		{
			if (mouse_press(590, 400, 630, 420) == 2)//未点击
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_about(590, 400, 630, 420, WHITE, LIGHTBLUE);
					num = 1;
				}
				continue;

				//标亮函数
			}

			if (mouse_press(590, 400, 630, 420) == 1)//&& cinfo->itemnum == 0)
			{
				MouseS = 0;
				lightbutton_about(590, 400, 630, 420, WHITE, LIGHTBLUE);
				cnt = (cnt + 1) % 2;
				switch (cnt)
				{
				case 0:
					About_show();//显示关于
					break;
				case 1:
					Help_show();
					break;
				}
				continue;
				//待修改
			}
		}

		if (num != 0)
		{
			MouseS = 0;
			recoverbutton_about(num);
			num = 0;
		}
		if (MouseS != 0)
		{
			MouseS = 0;
		}
	}
}

/*********************************************
FUNCTION:lightbutton_about
DESCRIPTION：About按钮点亮
INPUT:x,y,x1,y1,color1,color2
RETURN:无
***********************************************/
void lightbutton_about(int x, int y, int x1, int y1, int color1, int color2)//About按钮点亮
{
	clrmous(MouseX, MouseY);
	delay(10);
	setcolor(color1);
	setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
	setfillstyle(1, color2);
	bar(x, y, x1, y1);
	rectangle(x, y, x1, y1);	//画边框
	puthz(595, 402, "切换", 16, 17, BLUE);
}

/*********************************************
FUNCTION:recoverbutton_about
DESCRIPTION：About按钮恢复
INPUT:status
RETURN:无
***********************************************/
void recoverbutton_about(int status)//About按钮恢复
{
	clrmous(MouseX, MouseY);

	switch (status)
	{
	case 1:
		setfillstyle(SOLID_FILL, BLUE);
		bar(590, 400, 630, 420);
		puthz(595, 402, "切换", 16, 17, WHITE);
		break;
	}
}

/*********************************************
FUNCTION:About_show
DESCRIPTION：显示关于信息
INPUT:void
RETURN:无
***********************************************/
void About_show(void)
{
	int i, j;

	delay(300);
	setfillstyle(SOLID_FILL, BLACK);
	bar(1, 24, 639, 479);
	setbkcolor(BLACK);
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(2, HORIZ_DIR, 6);
	/*
	- 开发人员：华中科技大学人工智能与自动化学院
				自动化1801魏靖旻、邓述民
	- 测试人员：华中科技大学人工智能与自动化学院C语言教授、副教授
	- 更新日期：2019/11/4
	- 大小：89.6M
	- version:1.0.0

		此软件为入门级产品，对电脑配置要求较低。
		此软件不作为任何商业用途，如果发现有任何违规或者抄袭现象或
	被抄袭现象，开发者将承担或有权追究其责任。
		此软件的开发目的，是为了方便证券从业人员或者投资人员的模拟
	交易与投资，以及学习证券。此软件的数据来源于通达信2009/1/5
	----2019/9/23`(平安银行<-->高新发展)如果有任何数据流失的问题，
	可向开发者反馈，开发者将进行修改和完善。

		最后，我们想感谢C语言课程设计的所有老师、助教，以及帮助过
	我们的同学。软件依旧有很多不足或者是可更新优化的地方。希望我们
	在软件设计和开发方面能走的更远。
	*/
	for (i = 0; i < 20; i++)
	{
		for (j = 0; j < 620; j++)
		{
			putpixel(10 + 10 * j, 40 + 20 * i, LIGHTRED);
		}
	}
	puthz(10, 40, "开发人员：华中科技大学人工智能与自动化学院", 16, 17, WHITE);

	puthz(10+17*5, 60, "自动化", 16, 17, RED);
	setcolor(RED);
	outtextxy(13 + 17 * 8, 57, "1801");
	puthz(10 + 17 * 12, 60, "魏靖", 16, 17, YELLOW);
	setcolor(YELLOW);
	outtextxy(13 + 17 * 14, 57, "min | ");
	puthz(10 + 17 * 17, 60, "邓述民", 16, 17, YELLOW);

	puthz(10, 80, "测试人员：华中科技大学人工智能与自动化学院", 16, 17, WHITE);
	setcolor(RED);
	outtextxy(13 + 17 * 21, 77, "C");
	puthz(10 + 22 * 17, 80, "语言教授、副教授", 16, 17, WHITE);

	puthz(10, 100, "更新日期：", 16, 17, WHITE);
	setcolor(RED);
	outtextxy(13 + 17 * 5, 97, "2019/11/9");

	puthz(10, 120, "大小：", 16, 17, WHITE);
	setcolor(YELLOW);
	outtextxy(13 + 17 * 3, 117, "29.2M");

	setcolor(LIGHTCYAN);
	outtextxy(10, 137, "VERSION: 1.0.2");

	puthz(10 + 17 * 2, 180, "此软件为入门级产品，对电脑配置要求较低。", 16, 17, WHITE);

	puthz(10 + 17 * 2, 200, "此软件不作为任何商业用途，如果发现有任何违规或者抄袭现象或", 16, 17, WHITE);
	puthz(10, 220, "被抄袭现象，开发者将承担或有权追究其责任。", 16, 17, WHITE);

	puthz(10 + 17 * 2, 240, "此软件的开发目的，是为了方便证券从业人员或者投资人员的模拟", 16, 17, WHITE);
	puthz(10, 260, "交易与投资，以及学习证券。此软件的数据来源于通达信", 16, 17, WHITE);
	setcolor(RED);
	outtextxy(13 + 17 * 25, 257, "2009/1/5--2019/9/23");
	puthz(10, 280, "（平安银行——高新发展）如果有任何数据流失的问题，可向开发者反", 16, 17, WHITE);
	puthz(10, 300, "馈，开发者将进行修改和完善。", 16, 17, WHITE);
	
	puthz(10+17*2, 320, "最后，我们想感谢", 16, 17, WHITE);
	outtextxy(13 + 17 * 10, 317, "C");
	puthz(10 + 17 * 11, 320, "语言课程设计的所有老师、助教，以及帮助过", 16, 17, WHITE);
	puthz(10, 340, "我们的同学。软件依旧有很多不足或者是可更新优化的地方。希望我们", 16, 17, WHITE);
	puthz(10, 360, "能一直热爱",16, 17, WHITE);
	outtextxy(13 + 17 * 5, 357, "C");
	puthz(13 + 17 * 6, 360, "语言和编程设计，在软件开发方面走的更远。", 16, 17, WHITE);

	
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(3, HORIZ_DIR, 4);
	setcolor(RED);
	outtextxy(125, 390, "HUST:");
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(2, HORIZ_DIR, 8);
	setcolor(LIGHTRED);
	outtextxy(200, 400, "	  SCHOOL OF");
	setcolor(LIGHTCYAN);
	outtextxy(50, 430, "ARTIFICIAL INTELLIGENCE AND AUTOMATION");

	setfillstyle(SOLID_FILL, BLUE);
	bar(590, 400, 630, 420);
	puthz(595, 402, "切换", 16, 17, WHITE);
}


/*********************************************
FUNCTION:Help_show
DESCRIPTION：显示帮助信息
INPUT:void
RETURN:无
***********************************************/
void Help_show(void)
{
	int i, j;

	delay(300);
	setfillstyle(SOLID_FILL, BLACK);
	bar(1, 24, 639, 479);
	setbkcolor(BLACK);
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(2, HORIZ_DIR, 6);
	/*
	- 软件部分快捷键操作说明

	  主菜单：F1可打开系统菜单，F2可进入 行情，F3可进入沪深，
	F4可进入选股，F5可进入交易，F6可进入个人

	  沪深界面：用户通过UP键和DOWN键来进行页面的跳转，可查
	看两百支股票的详细信息

	  行情界面：用户通过UP键和DOWN键来进行股票队列的跳转，
	通过LEFT和RIGHT键可查看该个股十年内任何一天的信息。通过
	1,2,3可切换不同的换天数模式

	  换股->历史记录界面：用户通过UP键和DOWN键来进行历史
	记录的翻页

	  个人->持仓界面：用户通过UP键和DOWN键来进行持仓信息的翻页
	*/
	for (i = 0; i < 20; i++)
	{
		for (j = 0; j < 620; j++)
		{
			putpixel(10 + 10 * j, 40 + 20 * i, LIGHTCYAN);
		}
	}

	puthz(10 + 17 * 2, 40, "软件部分快捷键操作说明", 16, 17, WHITE);

	puthz(10 + 17 * 2, 80, "主菜单：", 16, 17, WHITE);
	setcolor(RED);
	outtextxy(13 + 17 * 6, 77, "F1");
	puthz(10 + 17 * 8, 80, "可打开系统菜单，", 16, 17, WHITE);
	outtextxy(13 + 17 * 16, 77, "F2");
	puthz(10 + 17 * 18, 80, "可进入行情，", 16, 17, WHITE);
	outtextxy(13 + 17 * 24, 77, "F3");
	puthz(10 + 17 * 26, 80, "可进入沪深，", 16, 17, WHITE);
	outtextxy(13, 97, "F4");
	puthz(10 + 17 * 2, 100, "可进入选股，", 16, 17, WHITE);
	outtextxy(13 + 17 * 8, 97, "F5");
	puthz(10 + 17 * 10, 100, "可进入交易，", 16, 17, WHITE);
	outtextxy(10 + 17 * 16, 97, "F6");
	puthz(10 + 17 * 18, 100, "可进入个人", 16, 17, WHITE);

	puthz(10 + 17 * 2, 140, "沪深界面：用户通过", 16, 17, WHITE);
	outtextxy(15 + 17 * 11, 137, "UP");
	puthz(10 + 17 * 13, 140, "键和", 16, 17, WHITE);
	outtextxy(20 + 17 * 15, 137, "DOWN");
	puthz(10 + 17 * 19, 140, "键来进行页面的跳转，可查", 16, 17, WHITE);
	puthz(10, 160, "看两百支股票的详细信息", 16, 17, WHITE);

	puthz(10 + 17 * 2, 200, "沪深界面：用户通过", 16, 17, WHITE);
	outtextxy(15 + 17 * 11, 197, "UP");
	puthz(10 + 17 * 13, 200, "键和", 16, 17, WHITE);
	outtextxy(23 + 17 * 15, 197, "DOWN");
	puthz(10 + 17 * 19, 200, "键来进行股票队列的跳转，通", 16, 17, WHITE);
	puthz(10, 220, "过", 16, 17, WHITE);
	outtextxy(20 + 17, 217, "LEFT");
	puthz(10 + 17 * 4, 220, "和", 16, 17, WHITE);
	outtextxy(23 + 17 * 5, 217, "RIGHT");
	puthz(10 + 17 * 9, 220, "键可查看该个股十年内任何一天的信息。通过", 16, 17, WHITE);
	outtextxy(10, 240, "1,2,3");
	puthz(10 + 17 * 3, 240, "可切换不同的换天数模式", 16, 17, WHITE);

	puthz(10 + 17 * 2, 280, "换股——历史记录界面：用户通过", 16, 17, WHITE);
	outtextxy(15 + 17 * 17, 277, "UP");
	puthz(10 + 17 * 19, 280, "键和", 16, 17, WHITE);
	outtextxy(23 + 17 * 21, 277, "DOWN");
	puthz(10 + 17 * 25, 280, "键来进行历史", 16, 17, WHITE);
	puthz(10, 300, "记录的翻页", 16, 17, WHITE);

	puthz(10 + 17 * 2, 340, "个人——持仓界面：用户通过", 16, 17, WHITE);
	outtextxy(15 + 17 * 15, 337, "UP");
	puthz(10 + 17 * 17, 340, "键和", 16, 17, WHITE);
	outtextxy(23 + 17 * 19, 337, "DOWN");
	puthz(10 + 17 * 23, 340, "键来进行持仓信息的翻页", 16, 17, WHITE);

	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(3, HORIZ_DIR, 4);
	setcolor(RED);
	outtextxy(125, 390, "HUST:");
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(2, HORIZ_DIR, 8);
	setcolor(LIGHTRED);
	outtextxy(200, 400, "	  SCHOOL OF");
	setcolor(LIGHTCYAN);
	outtextxy(50, 430, "ARTIFICIAL INTELLIGENCE AND AUTOMATION");

	setfillstyle(SOLID_FILL, BLUE);
	bar(590, 400, 630, 420);
	puthz(595, 402, "切换", 16, 17, WHITE);
}
