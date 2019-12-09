#include"public.h"
#include"slctfunc.h"
#include"storead.h"
#include"market.h"
#include"techslc.h"
#include"control.h"
#define MAXPAGELIST 10

/*****************************************
searchlist.c
COPYRIGHT:	weijingmin
FUNCTION:	show search result and skip
ABSTRACT:
		A.show T_search result
		B.skip to the market
DATE:2019/11/1
******************************************/


/*********************************************
FUNCTION:T_searchresult
DESCRIPTION：显示搜索结果
INPUT:cinfo,u,p_page,sto_list,return_page
RETURN:0/sto_list[i]
***********************************************/
int T_searchresult(Ctr * cinfo, USER * u, int* p_page, int* sto_list, int return_page)
{
	int i, j;
	int key = 0;//键盘
	int num = 0;
	int page = 1;
	int sum_page = 0;
	int cnt = 0;
	char sto_num[5] = { 0 };
	STODAYCHAR info[200][1];
	//STODAYCHAR** info = NULL;


	clrmous(MouseX, MouseY);
	delay(100);
	cleardevice();
	draw_T_result();
	mainmenu_draw();
	control_judge(u);
	memset(info, '\0', 200 * sizeof(STODAYCHAR));
	for (i = 0; i < 200; i++)
	{
		if (sto_list[i] == 0)//算出有几个满足条件
		{
			break;//压缩
		}
	}
	cnt = i;//压缩矩阵并开辟空间
	if (cnt != i)
	{
		cnt = 0;
	}
	if (cnt == 0)
	{
		//弹窗?
		show_failpop();
		*p_page = return_page;
		/*if(info!=NULL)
		{
			free(info);
			info=NULL;
		}*/
		return 0;
	}

	/*if ((info = (STODAYCHAR * *)malloc(cnt * sizeof(STODAYCHAR*))) == NULL)
	{
		printf("memoryallocation runs wrong for info");
		delay(3000);
		exit(1);
	}*/
	if (cnt % MAXPAGELIST == 0)
	{
		sum_page = cnt / MAXPAGELIST;
	}
	else
	{
		sum_page = cnt / MAXPAGELIST + 1;//补页
	}
	setfillstyle(SOLID_FILL, RED);
	bar(622, 80 + 360 * (page - 1) / sum_page, 628, 80 + 360 * page / sum_page);
	//进度条初始化*/
	for (i = MAXPAGELIST * (page - 1); i < MAXPAGELIST * page; i++)//初始化第一页的显示
	{
		/*if ((info[i] = (STODAYCHAR*)malloc(sizeof(STODAYCHAR))) == NULL)
		{
			printf("memoryallocation runs wrong");
			delay(3000);
			exit(1);
		}*/
		memset(info[i], '\0', sizeof(STODAYCHAR));
		if (sto_list[i] != 0)
		{
			sto_daybyday(sto_list[i], 1, 1, info[i]);
			itoa(sto_list[i], sto_num, 10);
			show_searchsto(info[i], i - MAXPAGELIST * (page - 1), sto_num);
			/*if (info[i] != NULL)
			{
				free(info[i]);
			}*/
		}
		else
		{
			continue;
		}
	}

	while (1)
	{
		if (!bioskey(1))
		{
			newmouse(&MouseX, &MouseY, &press);
			control_mouse(cinfo, u);
		}
		else if (bioskey(1))
		{
			key = bioskey(0);
			cinfo->key = key;
			if (key == DOWN)
			{
				setfillstyle(SOLID_FILL, BLACK);
				bar(10, 80, 600, 440);
				setcolor(RED);
				for (i = 0; i < 6; i++)
				{
					line(45 + 80 * i, 60, 45 + 80 * i, 459);
				}
				delay(100);

				page = (page + 1) % (sum_page + 1);
				if (page == 0)
				{
					page = 1;//保证只有一页的时候不出bug
				}
				for (i = MAXPAGELIST * (page - 1); i < MAXPAGELIST * page; i++)
				{
					/*if ((info[i] = (STODAYCHAR*)malloc(sizeof(STODAYCHAR))) == NULL)
					{
						printf("memoryallocation runs wrong");
						delay(3000);
						exit(1);
					}*/
					memset(info[i], '\0', sizeof(STODAYCHAR));
					if (sto_list[i] != 0)
					{
						sto_daybyday(sto_list[i], 1, 1, info[i]);
						itoa(sto_list[i], sto_num, 10);//股票代码
						show_searchsto(info[i], i - MAXPAGELIST * (page - 1), sto_num);//显示信息
						/*if(info[i]!=NULL)
						{
							free(info[i]);
						}*/
					}
					else
					{
						/*if(info[i]!=NULL)
						{
							free(info[i]);
						}*/
						continue;
					}
				}

				setfillstyle(SOLID_FILL, LIGHTGRAY);
				bar(622, 66, 628, 452);
				setfillstyle(SOLID_FILL, RED);
				setfillstyle(SOLID_FILL, RED);
				bar(622, 80 + 360 * (page - 1) / sum_page, 628, 80 + 360 * page / sum_page);

				cinfo->key = 0;
				continue;
			}
			if (key == UP)
			{
				setfillstyle(SOLID_FILL, BLACK);
				bar(10, 80, 600, 440);
				setcolor(RED);
				for (i = 0; i < 6; i++)
				{
					line(45 + 80 * i, 60, 45 + 80 * i, 459);
				}
				delay(100);
				page--;
				if (page == 0)
				{
					page = sum_page;
				}
				if (page == 0)
				{
					page = 1;//保证只有一页的时候不出bug
				}
				for (i = MAXPAGELIST * (page - 1); i < MAXPAGELIST * page; i++)
				{
					/*if ((info[i] = (STODAYCHAR*)malloc(sizeof(STODAYCHAR))) == NULL)
					{
						printf("memoryallocation runs wrong");
						delay(3000);
						exit(1);
					}*/
					memset(info[i], '\0', sizeof(STODAYCHAR));
					if (sto_list[i] != 0)
					{
						sto_daybyday(sto_list[i], 1, 1, info[i]);
						itoa(sto_list[i], sto_num, 10);//股票代码
						show_searchsto(info[i], i - MAXPAGELIST * (page - 1), sto_num);//显示信息
						/*if(info[i]!=NULL)
						{
							free(info[i]);
						}*/
					}
					else
					{
						/*if(info[i]!=NULL)
						{
							free(info[i]);
						}*/
						continue;
					}
				}
				setfillstyle(SOLID_FILL, LIGHTGRAY);
				bar(622, 66, 628, 452);
				setfillstyle(SOLID_FILL, RED);
				bar(622, 80 + 360 * (page - 1) / sum_page, 628, 80 + 360 * page / sum_page);
				cinfo->key = 0;
				continue;
			}
		}
		control_key(cinfo);  //键盘控制
		if (control_menu(cinfo))
		{
			clrmous(MouseX, MouseY);
			//setfillstyle(1,BLACK);
			//bar(0,25,639,480);
			mainmenu_draw();
			control_judge(u);
			draw_T_stoselect1();
			for (i = MAXPAGELIST * (page - 1); i < MAXPAGELIST * page; i++)//初始化第一页的显示
			{
				/*if ((info[i] = (STODAYCHAR*)malloc(sizeof(STODAYCHAR))) == NULL)
				{
					printf("memoryallocation runs wrong");
					delay(3000);
					exit(1);
				}*/
				memset(info[i], '\0', sizeof(STODAYCHAR));
				if (sto_list[i] != 0)
				{
					sto_daybyday(sto_list[i], 1, 1, info[i]);
					itoa(sto_list[i], sto_num, 10);

					show_searchsto(info[i], i - MAXPAGELIST * (page - 1), sto_num);
					/*if(info[i]!=NULL)
					{
						free(info[i]);
					}*/
				}
				else
				{
					/*if(info[i]!=NULL)
					{
						free(info[i]);
					}*/
					continue;
				}
			}
		}

		if (cinfo->func != 5 && cinfo->func != 10)
		{
			clrmous(MouseX, MouseY);
			delay(10);
			/*if(info!=NULL)
			{
				free(info);
				info=NULL;
			}*/
			return 0;
		}

		if (mouse_press(620, 450, 630, 460) == 1)//下翻
		{
			setfillstyle(SOLID_FILL, BLACK);
			bar(10, 80, 600, 440);
			setcolor(RED);
			for (i = 0; i < 6; i++)
			{
				line(45 + 80 * i, 60, 45 + 80 * i, 459);
			}
			delay(100);
			page = (page + 1) % (sum_page + 1);
			if (page == 0)
			{
				page = 1;
			}

			for (i = MAXPAGELIST * (page - 1); i < MAXPAGELIST * page; i++)
			{
				/*if ((info[i] = (STODAYCHAR*)malloc(sizeof(STODAYCHAR))) == NULL)
				{
					printf("memoryallocation runs wrong");
					delay(3000);
					exit(1);
				}*/
				memset(info[i], '\0', sizeof(STODAYCHAR));
				if (sto_list[i] != 0)
				{
					sto_daybyday(sto_list[i], 1, 1, info[i]);
					itoa(sto_list[i], sto_num, 10);//股票代码
					show_searchsto(info[i], i - MAXPAGELIST * (page - 1), sto_num);//显示信息
					/*if(info[i]!=NULL)
					{
						free(info[i]);
					}*/
				}
				else
				{
					/*if(info[i]!=NULL)
					{
						free(info[i]);
					}*/
					continue;
				}
			}

			setfillstyle(SOLID_FILL, LIGHTGRAY);
			bar(622, 66, 628, 452);
			setfillstyle(SOLID_FILL, RED);
			bar(622, 80 + 360 * (page - 1) / sum_page, 628, 80 + 360 * page / sum_page);
		}
		if (mouse_press(620, 58, 630, 68) == 1)//上翻
		{
			setfillstyle(SOLID_FILL, BLACK);
			bar(10, 80, 600, 440);
			setcolor(RED);
			for (i = 0; i < 6; i++)
			{
				line(45 + 80 * i, 60, 45 + 80 * i, 459);
			}
			delay(100);
			page--;
			if (page == 0)
			{
				page = sum_page;
			}
			if (page == 0)
			{
				page = 1;//保证只有一页的时候不出bug
			}
			for (i = MAXPAGELIST * (page - 1); i < MAXPAGELIST * page; i++)
			{
				/*if ((info[i] = (STODAYCHAR*)malloc(sizeof(STODAYCHAR))) == NULL)
				{
					printf("memoryallocation runs wrong");
					delay(3000);
					exit(1);
				}*/
				memset(info[i], '\0', sizeof(STODAYCHAR));
				if (sto_list[i] != 0)
				{
					sto_daybyday(sto_list[i], 1, 1, info[i]);
					itoa(sto_list[i], sto_num, 10);//股票代码
					show_searchsto(info[i], i - MAXPAGELIST * (page - 1), sto_num);//显示信息
					/*if(info[i]!=NULL)
					{
						free(info[i]);
					}*/
				}
				else
				{
					/*if(info[i]!=NULL)
					{
						free(info[i]);
					}*/
					continue;
				}
			}

			setfillstyle(SOLID_FILL, LIGHTGRAY);
			bar(622, 66, 628, 452);
			setfillstyle(SOLID_FILL, RED);
			bar(622, 80 + 360 * (page - 1) / sum_page, 628, 80 + 360 * page / sum_page);
		}
		if (mouse_press(10, 80, 460, 450) == 1)//搜索
		{
			delay(300);
			j = (MouseY - 80) / 35;
			i = (MouseY - 80) / 35 + (page - 1) * 10;
			setlinestyle(SOLID_LINE, 0, 1);
			setcolor(RED);
			if (sto_list[i] != 0)//此处股票存在
			{
				line(1, 107 + j * 35, 619, 107 + j * 35);//打标记
				delay(2000);
				/*if(info != NULL)
				{
					free(info);
					info = NULL;
				}*/
				//sto_market(sto_list[i]);//cinfo待改！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
				return sto_list[i];
			}
		}

		if (MouseX > 100 && MouseX < 140 && MouseY > 461 && MouseY < 479)
		{
			if (mouse_press(100, 461, 140, 479) == 2)//未点击
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_Tselect(5);
					num = 5;
				}
				continue;
				//标亮函数
			}
			else if (mouse_press(100, 461, 140, 479) == 1)
			{
				MouseS = 0;
				lightbutton_Tselect(5);
				*p_page = return_page;//返回

				break;
			}
		}

		if (num != 0)
		{
			MouseS = 0;
			recoverbutton_Tselect(num);
			num = 0;
		}
		if (MouseS != 0)
		{
			MouseS = 0;
		}
	}
	/*if (info != NULL)
	{
		free(info);
		info = NULL;
	}*/
	return 0;
}

/*********************************************
FUNCTION:show_failpop
DESCRIPTION：没找到股票的返回弹窗
INPUT:void
RETURN:无
***********************************************/
void show_failpop(void)
{
	setfillstyle(SOLID_FILL, LIGHTGRAY);
	bar(180, 150, 460, 330);
	setfillstyle(SOLID_FILL, RED);
	bar(200, 170, 440, 310);
	puthz(250, 200, "未找到相应股票", 16, 20, WHITE);
	puthz(250, 260, "将在", 16, 20, WHITE);
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(2, HORIZ_DIR, 6);
	setcolor(WHITE);
	outtextxy(295, 260, "4");
	puthz(310, 260, "秒后返回", 16, 20, WHITE);
	delay(1000);
	setfillstyle(SOLID_FILL, RED);
	bar(295, 260, 308, 273);
	outtextxy(295, 260, "3");
	delay(1000);
	bar(295, 260, 308, 273);
	outtextxy(295, 260, "2");
	delay(1000);
	bar(295, 260, 308, 273);
	outtextxy(295, 260, "1");
	delay(1000);//倒计时
}

/*********************************************
FUNCTION:show_searchsto
DESCRIPTION：打印搜索的股票信息
INPUT:info,coordY,sto
RETURN:无
***********************************************/
void show_searchsto(STODAYCHAR* info, int coordY, char* sto)
{

	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(2, HORIZ_DIR, 6);
	setcolor(LIGHTMAGENTA);
	outtextxy(10, 80 + coordY * 35, sto);
	setcolor(WHITE);
	outtextxy(50, 80 + coordY * 35, info->open);
	setcolor(RED);
	outtextxy(130, 80 + coordY * 35, info->high);
	setcolor(GREEN);
	outtextxy(210, 80 + coordY * 35, info->low);
	setcolor(WHITE);
	outtextxy(290, 80 + coordY * 35, info->close);
	setcolor(BLUE);
	outtextxy(370, 80 + coordY * 35, info->VOL);
	setcolor(LIGHTBLUE);
	outtextxy(460, 80 + coordY * 35, info->VOT);
}

/*********************************************
FUNCTION:draw_T_result
DESCRIPTION：画搜索结果界面
INPUT:void
RETURN:无
***********************************************/
void draw_T_result(void)
{
	int i;
	setbkcolor(BLACK);
	setlinestyle(SOLID_LINE, 0, 2);
	setcolor(RED);
	rectangle(1, 25, 639, 459);
	setfillstyle(SOLID_FILL, RED);
	bar(2, 26, 638, 58);
	for (i = 0; i < 6; i++)
	{
		line(45 + 80 * i, 60, 45 + 80 * i, 459);
	}
	setfillstyle(SOLID_FILL, LIGHTGRAY);
	bar(620, 58, 630, 459);
	setcolor(RED);
	rectangle(620, 58, 630, 459);
	setcolor(BLACK);
	line(622, 453, 628, 453);
	line(622, 453, 625, 458);
	line(625, 458, 628, 453);
	floodfill(625, 457, BLACK);
	line(622, 65, 628, 65);
	line(622, 65, 625, 60);
	line(625, 60, 628, 65);
	floodfill(625, 62, BLACK);

	setcolor(WHITE);
	setfillstyle(SOLID_FILL, RED);
	bar(100, 462, 140, 479);
	rectangle(100, 462, 140, 479);
	puthz(105, 462, "返回", 16, 17, WHITE);

	puthz(10, 60, "序号", 16, 17, LIGHTMAGENTA);
	puthz(50, 60, "今开", 16, 17, WHITE);
	puthz(130, 60, "最高", 16, 17, RED);
	puthz(210, 60, "最低", 16, 17, GREEN);
	puthz(290, 60, "今收", 16, 17, YELLOW);
	puthz(370, 60, "成交量", 16, 17, BLUE);
	puthz(460, 60, "成交额", 16, 17, LIGHTBLUE);
}