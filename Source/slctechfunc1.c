#include"public.h"
#include"database.h"
#include"techslc.h"
#include"slctfunc.h"
#include"tsearch.h"
#include"control.h"

/*****************************************
slctechfunc1.c
COPYRIGHT:	weijingmin
FUNCTION:	T_select page 1
ABSTRACT:
		A.find button for light
		B.check option array
DATE:2019/10/29
******************************************/

/*********************************************
FUNCTION:T_stoselect1
DESCRIPTION：本页显示K线形态选股，MA均线形态选股
INPUT:cinfo,u,page,opt[][6],sto_list
RETURN:无
***********************************************/
void T_stoselect1(Ctr* cinfo, USER* u, int* page, int opt[][6], int* sto_list)
{
	int i, j;
	int num;//按钮标号
	int color = 0;//记录本次标号的按钮颜色
	int precolor = 0;//记录上次标号的按钮颜色
	clrmous(MouseX, MouseY);
	delay(100);
	cleardevice();
	draw_T_stoselect1();
	mainmenu_draw();
	control_judge(u);
	while (1)
	{
		if(!bioskey(1))
		{
			newmouse(&MouseX, &MouseY, &press);
			control_mouse(cinfo,u);
		}
		else if(bioskey(1))
			cinfo->key = bioskey(0);
		control_key(cinfo);
		if(control_menu(cinfo))
		{
			clrmous(MouseX,MouseY);
		//	setfillstyle(1,BLACK);
			//bar(0,25,639,480);
			mainmenu_draw();
			control_judge(u);
			draw_T_stoselect1();
			submenu_draw(cinfo->itemnum);
			memset(opt,0,66*sizeof(int));
		}
		
		if(cinfo->func != 5 && cinfo->func != 10)
		{
			clrmous(MouseX,MouseY);
			delay(10);
			return;
		}

		if (MouseX > 35 && MouseX < 512 && MouseY> 320 && MouseY < 440)
		{
			if (mouse_press(45 + 17 * 4 + 7, 332 - 5, 45 + 17 * 4 + 17, 332 + 5) == 2)
			{
				MouseS = 1;
				continue;
			}
			else if (mouse_press(210 + 17 * 4 + 7, 332 - 5, 210 + 17 * 4 + 17, 332 + 5) == 2)
			{
				MouseS = 1;
				continue;
			}
			else if (mouse_press(45 + 17 * 4 + 7, 332 + 35, 45 + 17 * 4 + 17, 332 + 45) == 2)
			{
				MouseS = 1;
				continue;
			}
			else if (mouse_press(210 + 17 * 4 + 7, 332 + 35, 210 + 17 * 4 + 17, 332 + 45) == 2)
			{
				MouseS = 1;
				continue;
			}
			else if (mouse_press(45 + 17 * 4 + 7, 332 + 75, 45 + 17 * 4 + 17, 332 + 85) == 2)
			{
				MouseS = 1;
				continue;
			}
			else if (mouse_press(210 + 17 * 4 + 7, 332 + 75, 210 + 17 * 4 + 17, 332 + 85) == 2)
			{
				MouseS = 1;
				continue;
			}
			else
			{
				MouseS = 0;
			}

			if (mouse_press(35, 320, 512, 440) == 1)
			{
				MA_select1_check(&precolor, &color, opt);//检查第一页哪个元素被标记，改变opt数组并进行标记反馈
				continue;
			}
		}


		if (MouseX > 35 && MouseX < 512 && MouseY> 68 && MouseY < 280)
		{
			if (mouse_press(35 + 17 * 5 + 4, 83, 35 + 17 * 5 + 14, 93) == 2
				|| mouse_press(35 + 17 * 5 + 4, 83 + 30, 35 + 17 * 5 + 14, 123) == 2
				|| mouse_press(35 + 17 * 5 + 4, 83 + 30 * 2, 35 + 17 * 5 + 14, 153) == 2)
			{
				MouseS = 1;
				continue;
			}
			else if (mouse_press(322 + 17 * 5 + 4, 83, 322 + 17 * 5 + 14, 93) == 2
				|| mouse_press(322 + 17 * 5 + 4, 83 + 30, 322 + 17 * 5 + 14, 123) == 2
				|| mouse_press(322 + 17 * 5 + 4, 83 + 30 * 2, 322 + 17 * 5 + 14, 153) == 2)
			{
				MouseS = 1;
				continue;
			}
			else if (mouse_press(35 + 17 * 5 + 4, 203, 35 + 17 * 5 + 14, 213) == 2
				|| mouse_press(35 + 17 * 5 + 4, 203 + 30, 35 + 17 * 5 + 14, 243) == 2
				|| mouse_press(35 + 17 * 5 + 4, 203 + 30 * 2, 35 + 17 * 5 + 14, 273) == 2)
			{
				MouseS = 1;
				continue;
			}
			else if (mouse_press(322 + 17 * 5 + 4, 203, 322 + 17 * 5 + 14, 213) == 2
				|| mouse_press(322 + 17 * 5 + 4, 203 + 30, 322 + 17 * 5 + 14, 243) == 2)
			{
				MouseS = 1;
				continue;
			}
			else
			{
				MouseS = 0;
			}

			if (mouse_press(35, 68, 512, 280) == 1)
			{
				T_select1_check(opt);//检查第一页哪个元素被标记，改变opt数组并进行标记反馈
				continue;
			}
		}
		
		
		if (MouseX > 560 && MouseX < 620 && MouseY > 442 && MouseY < 479)
		{
			if (mouse_press(560, 442, 620, 479) == 2)//未点击
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_Tselect(1);
					num = 1;
				}
				continue;
				//标亮函数
			}
			else if (mouse_press(560, 442, 620, 479) == 1)
			{
				MouseS = 0;
				lightbutton_Tselect(1);
				delay(100);
				*page = 2;
				return;
			}
		}


		if (MouseX > 250 && MouseX < 290 && MouseY > 452 && MouseY < 479)
		{
			if (mouse_press(250, 452, 290, 479) == 2)//未点击
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_Tselect(3);
					num = 3;
				}
				continue;
				//标亮函数
			}
			else if (mouse_press(250, 452, 290, 479) == 1)
			{
				MouseS = 0;
				lightbutton_Tselect(3);
				T_search(opt, sto_list, RED);
				*page = 3;
				return;
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
}

/*********************************************
FUNCTION:T_select1_check
DESCRIPTION：寻找标记，
	打标记并给对应的opt数组赋值
INPUT:opt[][6]
RETURN:无
***********************************************/
void T_select1_check(int opt[][6])
{
	int x, y;//要点亮的按钮的坐标
	int i;
	int color;
	int pre_x, pre_y;//上一个被点亮按钮的坐标
	int tech = 0, index = 0;//tech为类型号，index为类型选项

	//Kline点亮和标记
	find_Klinebutton(&tech, &index, &x, &y);//寻找用户点击的框
	if ((tech == 0) || ((tech != 0) && (index == 0)))//没有点击有用框
	{
		return;
	}

	switch (tech)
	{
	case 1:
		color = WHITE;
		break;
	case 2:
		color = YELLOW;
		break;
	case 3:
		color = LIGHTRED;
		break;
	case 4:
		color = LIGHTBLUE;
		break;
	}
	for (i = 0; i < 4; i++)
	{
		if (opt[tech - 1][i] == 1)//这一类型存在已被选择项
		{
			if (i == index - 1)//表明用户目前点击的按钮和上次选择这一项的两次一致
			{
				find_lastbutton(tech, i, &pre_x, &pre_y);
				clrmous(MouseX, MouseY);
				setcolor(color);
				draw_element(pre_x, pre_y, 1, color);
				opt[tech - 1][i] = 0;
				return;
			}
			else//表明用户在这一类型里点击的按钮与上次的不一致
			{
				find_lastbutton(tech, i, &pre_x, &pre_y);//寻找上一次点击的按钮的左上角坐标
				clrmous(MouseX, MouseY);
				setcolor(color);
				draw_element(pre_x, pre_y, 1, color);//恢复之前的按钮
				opt[tech - 1][i] = 0;
			}
		}
	}
	opt[tech - 1][index - 1] = 1;
	x += 17 * 5 + 4;
	y = y + 30 * (index - 1);
	clrmous(MouseX, MouseY);
	setcolor(color);
	draw_element(x, y, 1, LIGHTGRAY);//点亮标记的按钮
	
	return;
}

/*********************************************
FUNCTION:MA_select1_check
DESCRIPTION：寻找ma标记，
	打标记并给对应的opt数组赋值
INPUT:precolor,color,opt[][6]
RETURN:无
***********************************************/
void MA_select1_check(int *precolor, int *color, int opt[][6])
{
	int x, y;//要点亮的按钮的坐标
	int i, j;
	int pre_x, pre_y;//上一个被点亮按钮的坐标
	int tech = 0, index = 0;//tech为类型号，index为类型选项

	//MA点亮和标记
	find_MAbutton(&tech, &index, &x, &y);
	if ((tech != 5) || ((tech == 5) && (index == 0)))//没有点击有用框
	{
		return;
	}
	*precolor = *color;
	switch (index)
	{
	case 1:
		*color = WHITE;
		break;
	case 2:
		*color = WHITE;
		break;
	case 3:
		*color = YELLOW;
		break;
	case 4:
		*color = YELLOW;
		break;
	case 5:
		*color = LIGHTMAGENTA;
		break;
	case 6:
		*color = LIGHTMAGENTA;
		break;
	}
	for (i = 0; i < 6; i++)
	{
		if (opt[tech - 1][i] == 1)//这一类型存在已被选择项
		{
			if (i == index - 1)//表明用户目前点击的按钮和上次选择这一项的两次一致
			{
				find_lastbutton(tech, i + 1, &pre_x, &pre_y);
				clrmous(MouseX, MouseY);
				draw_element(pre_x, pre_y, 2, *precolor);
				opt[tech - 1][i] = 0;
				return;
			}
			else//表明用户在这一类型里点击的按钮与上次的不一致
			{
				find_lastbutton(tech, i + 1, &pre_x, &pre_y);//寻找上一次点击的按钮的左上角坐标
				clrmous(MouseX, MouseY);
				draw_element(pre_x, pre_y, 2, *precolor);//恢复之前的按钮
				opt[tech - 1][i] = 0;
			}
		}
	}
	opt[tech - 1][index - 1] = 1;
	clrmous(MouseX, MouseY);
	draw_element(x, y, 2, LIGHTCYAN);//点亮标记的按钮

}

/*********************************************
FUNCTION:find_MAbutton
DESCRIPTION：找到MA中的某一形态指标
INPUT:tech,index,x,y
RETURN:无
***********************************************/
void find_MAbutton(int* tech, int* index, int* x, int* y)
{
	//tech为类型号，index为类型选项,x和y为某一K线类型的左上角坐标
	if (mouse_press(35, 320, 512, 450) == 1)
	{
		delay(120);
		*tech = 5;//MA选股
		*x = 45;
		*y = 332;
	}

	if (mouse_press(*x + 17 * 4 + 7, *y - 5, *x + 17 * 4 + 17, *y + 5) == 1)
	{
		delay(120);
		*index = 1;//多头排列
		*x += 17 * 4 + 12;
		return;
	}
	else if (mouse_press(*x + 165 + 17 * 4 + 7, *y - 5, *x + 165 + 17 * 4 + 17, *y + 5) == 1)
	{
		delay(120);
		*index = 2;//空头排列
		*x += 165 + 17 * 4 + 12;
		return;
	}
	else if (mouse_press(*x + 17 * 4 + 7, *y + 35, *x + 17 * 4 + 17, *y + 45) == 1)
	{
		delay(120);
		*index = 3;//粘合
		*x += 17 * 4 + 12;
		*y += 40;
		return;
	}
	else if (mouse_press(*x + 165 + 17 * 4 + 7, *y + 35, *x + 165 + 17 * 4 + 17, *y + 45) == 1)
	{
		delay(120);
		*index = 4;//发散
		*x += 165 + 17 * 4 + 12;
		*y += 40;
		return;
	}
	else if (mouse_press(*x + 17 * 4 + 7, *y + 75, *x + 17 * 4 + 17, *y + 85) == 1)
	{
		delay(120);
		*index = 5;//拐头向上
		*x += 17 * 4 + 12;
		*y += 80;
	}
	else if (mouse_press(*x + 165 + 17 * 4 + 7, *y + 75, *x + 165 + 17 * 4 + 17, *y + 85) == 1)
	{
		delay(120);
		*index = 6;//拐头向下
		*x += 165 + 17 * 4 + 12;
		*y += 80;
		return;
	}
	
}

/*********************************************
FUNCTION:find_Klinebutton
DESCRIPTION：找到是多少根K线形态中的某一指标
INPUT:tech,index,x,y
RETURN:无
***********************************************/
void find_Klinebutton(int* tech, int* index, int* x, int* y)
{
	//tech为类型号，index为类型选项,x和y为某一K线类型的左上角坐标
	if (mouse_press(35, 68, 225, 168) == 1)
	{
		delay(120);
		*tech = 1;//单根K线
		*x = 35;
		*y = 83;
	}
	else if (mouse_press(322, 68, 512, 168) == 1)
	{
		delay(120);
		*tech = 2;//双根K线
		*x = 322;
		*y = 83;
	}
	else if (mouse_press(35, 188, 225, 280) == 1)
	{
		delay(120);
		*tech = 3;//三根K线
		*x = 35;
		*y = 203;
	}
	else if (mouse_press(322, 188, 512, 280) == 1)
	{
		delay(120);
		*tech = 4;//多根K线
		*x = 322;
		*y = 203;
	}

	if (mouse_press(*x + 17 * 5 + 4, *y, *x + 17 * 5 + 14, *y + 10) == 1)
	{
		delay(120);
		*index = 1;//n根K线中的第一项形态
	}
	else if (mouse_press(*x + 17 * 5 + 4, *y + 30, *x + 17 * 5 + 14, *y + 40) == 1)
	{
		delay(120);
		*index = 2;//n根K线中的第二项形态
	}
	else if (mouse_press(*x + 17 * 5 + 4, *y + 60, *x + 17 * 5 + 14, *y + 70) == 1)
	{
		delay(120);
		*index = 3;//n根K线中的第三项形态
	}
}

/*********************************************
FUNCTION:find_lastbutton
DESCRIPTION：找到上一个标记的坐标
INPUT:tech,last,pre_x,pre_y
RETURN:无
***********************************************/
void find_lastbutton(int tech, int last, int* pre_x, int* pre_y)
{
	if (tech == 1 || tech == 3)
	{
		*pre_x = 35 + 17 * 5 + 4;
	}
	else if (tech == 2 || tech == 4)
	{
		*pre_x = 322 + 17 * 5 + 4;
	}
	if (tech == 1 || tech == 2)
	{
		*pre_y = 83 + last * 30;
	}
	else if (tech == 3 || tech == 4)
	{
		*pre_y = 203 + last * 30;
	}

	if (tech == 5)//MA
	{
		if (last % 2 == 0)//last=2,4,6
		{
			*pre_x = 210 + 17 * 4 + 12;
			if (last == 2)
			{
				*pre_y = 332;
			}
			else if (last == 4)
			{
				*pre_y = 332 + 40;
			}
			else if (last == 6)
			{
				*pre_y = 332 + 80;
			}
		}
		else if (last % 2 == 1)//last=1,3,5
		{
			*pre_x = 45 + 17 * 4 + 12;
			if (last == 1)
			{
				*pre_y = 332;
			}
			else if (last == 3)
			{
				*pre_y = 332 + 40;
			}
			else if (last == 5)
			{
				*pre_y = 332 + 80;
			}
		}
	}
}