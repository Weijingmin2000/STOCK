#include"public.h"
#include"control.h"
#include"fundam.h"
#include"fsearch.h"



/********************************************
fundamental.c
COPYRIGHT:  dengshumin
FUNCTION:   fundamentally select stock
			by dragging the article
ABSTRACT:
		A. move_circle
		B. button lighting and recovering
DATE: 2019/10/6
RETURN: 无
********************************************/


/************************************
FUNCTION: fundam_control
DESCRIPTION: 基本面控制
INPUT: cinfo,u,page,sto_list
RETURN: 无
*************************************/
void fundam_control(Ctr* cinfo,USER* u,int* page, int* sto_list)
{
	int i;
	short int n = 0;
	int min[5] = { 231,231,231,231,231 };
	int max[5] = { 531,531,531,531,531 };
	int count[5] = { 0 };         //用来判断是否启用
	char s[4] = { '\0' };
	int option[5][2] = { 0.0 };	  //五种类型，每种的范围为option[i][1]--[i][2]
	
	clrmous(MouseX,MouseY);
	cleardevice();
	delay(100);
	clrmous(MouseX,MouseY);
	mainmenu_draw();
	control_judge(u);
	fundamental_draw();
	while(1)
	{
		if(!bioskey(1))
		{
			newmouse(&MouseX, &MouseY, &press);
			control_mouse(cinfo,u);
		}
		else if(bioskey(1))
		{
			cinfo->key = bioskey(0);
		}
		control_key(cinfo);          
		if(control_menu(cinfo))                 //菜单发生了变化
		{
			clrmous(MouseX,MouseY);
			mainmenu_draw();
			control_judge(u);
			fundamental_draw();
			submenu_draw(cinfo->itemnum);
			memset(option,0,10*sizeof(int));
		}
		
		if(cinfo->func != 5 && cinfo->func != 9)  
		{
			clrmous(MouseX,MouseY);
			delay(10);
			return;
		}
		
		if(mouse_press(6,85,22,101) == 1)
		{
			clrmous(MouseX,MouseY);
			fundam_judge(count,0);
			continue;
		}
		if(mouse_press(6,85+60,22,101+60) == 1)
		{
			clrmous(MouseX,MouseY);
			fundam_judge(count,1);
			continue;
		}
		if(mouse_press(6,85+60*2,22,101+60*2) == 1)
		{
			clrmous(MouseX,MouseY);
			fundam_judge(count,2);
			continue;
		}
		if(mouse_press(6,85+60*3,22,101+60*3) == 1)
		{
			clrmous(MouseX,MouseY);
			fundam_judge(count,3);
			continue;
		}
		if(mouse_press(6,85+60*4,22,101+60*4) == 1)
		{
			clrmous(MouseX,MouseY);
			fundam_judge(count,4);
			continue;
		}                              //启用判断
		if(mouse_press(231,70+15,381,86+15) == 1)    //拖动条
		{
			memset(s,'\0',sizeof(s));
			move_circle(&MouseX,&MouseY,&press,min,max,0);
			setfillstyle(1,BLACK);
			bar(180,85,210,105);
			setcolor(GREEN);
			sprintf(s,"%d",(min[0]-231)/3);
			outtextxy(178+2,66+15,s);
			continue;
		}
		else if(mouse_press(381,70+15,531,86+15) == 1)
		{
			memset(s,'\0',sizeof(s));
			move_circle2(&MouseX,&MouseY,&press,min,max,0);
			setfillstyle(1,BLACK);
			bar(552,85,600,105);
			setcolor(RED);
			sprintf(s,"%d",(max[0]-231)/3);
			outtextxy(552,66+15,s);
			continue;
		}
		if(mouse_press(231,70+15+60,381,86+15+60) == 1)
		{
			memset(s,'\0',sizeof(s));
			move_circle(&MouseX,&MouseY,&press,min,max,1);
			setfillstyle(1,BLACK);
			bar(180,85+60,210,105+60);
			setcolor(GREEN);
			sprintf(s,"%d",(min[1]-231)/3);
			outtextxy(178+2,66+15+60,s);
			continue;
		}
		else if(mouse_press(381,70+15+60,531,86+15+60) == 1)
		{
			memset(s,'\0',sizeof(s));
			move_circle2(&MouseX,&MouseY,&press,min,max,1);
			setfillstyle(1,BLACK);
			bar(552,85+60,600,105+60);
			setcolor(RED);
			sprintf(s,"%d",(max[1]-231)/3);
			outtextxy(552,66+15+60,s);
			continue;
		}
		if(mouse_press(231,70+15+60*2,381,86+15+60*2) == 1)
		{
			memset(s,'\0',sizeof(s));
			move_circle(&MouseX,&MouseY,&press,min,max,2);
			setfillstyle(1,BLACK);
			bar(180,85+60*2,210,105+60*2);
			setcolor(GREEN);
			sprintf(s,"%d",(min[2]-231)/3);
			outtextxy(178+2,66+15+60*2,s);
			continue;
		}
		else if(mouse_press(381,70+15+60*2,531,86+15+60*2) == 1)
		{
			memset(s,'\0',sizeof(s));
			move_circle2(&MouseX,&MouseY,&press, min, max,2);
			setfillstyle(1,BLACK);
			bar(552,85+60*2,600,105+60*2);
			setcolor(RED);
			sprintf(s,"%d",(max[2]-231)/3);
			outtextxy(552,66+15+60*2,s);
			continue;
		}
		if(mouse_press(231,70+15+60*3,381,86+15+60*3) == 1)
		{
			memset(s,'\0',sizeof(s));
			move_circle(&MouseX,&MouseY,&press, min, max,3);
			setfillstyle(1,BLACK);
			bar(180,85+60*3,210,105+60*3);
			setcolor(GREEN);
			sprintf(s,"%d",(min[3]-231)/3);
			outtextxy(178+2,66+15+60*3,s);
			continue;
		}
		else if(mouse_press(381,70+15+60*3,531,86+15+60*3) == 1)
		{
			memset(s,'\0',sizeof(s));
			move_circle2(&MouseX,&MouseY,&press, min, max,3);
			setfillstyle(1,BLACK);
			bar(552,85+60*3,600,105+60*3);
			setcolor(RED);
			sprintf(s,"%d",(max[3]-231)/3);
			outtextxy(552,66+15+60*3,s);
			continue;
		}
		if(mouse_press(231,70+15+60*4,381,86+15+60*4) == 1)
		{
			memset(s,'\0',sizeof(s));
			move_circle(&MouseX,&MouseY,&press, min, max,4);
			setfillstyle(1,BLACK);
			bar(180,85+60*4,210,105+60*4);
			setcolor(GREEN);
			sprintf(s,"%d",(min[4]-231)/3);
			outtextxy(178+2,66+15+60*4,s);
			continue;
		}
		else if(mouse_press(381,70+15+60*4,531,86+15+60*4) == 1)
		{
			memset(s,'\0',sizeof(s));
			move_circle2(&MouseX,&MouseY,&press, min, max,4);
			setfillstyle(1,BLACK);
			bar(552,85+60*4,600,105+60*4);
			setcolor(RED);
			sprintf(s,"%d",(max[4]-231)/3);
			outtextxy(552,66+15+60*4,s);
			continue;
		}
		if(mouse_press(240,390,400,430) == 2)
		{
			if(n == 0)       //防止重复标亮
			{
				MouseS = 1;
				lightbutton_fundamental();
				n = 1;
			}
			continue;
		}
		if(mouse_press(240,390,400,430) == 1)
		{	
			for (i = 0; i < 5; i++)
			{
				option[i][0] = (min[i] - 231) / 3;
				option[i][1] = (max[i] - 231) / 3;
			}
			F_search(count, option, sto_list);//选股函数
			*page = 3;
			break;
		}
		if(n != 0)
		{
			recoverbutton_fundamental();
			n = 0;
		}
		if(MouseS != 0)
		{
			MouseS = 0;
		}
	} 
	return;
}

/***********************************
FUNCTION: move_circle
DESCRIPTION: 左半部分的拖动条
INPUT: MouseX,MouseY,press,min,max,n
RETURN: 无
***********************************/
void move_circle(int* MouseX,int* MouseY,int* press,int* min,int* max,int n)
{
	while(1)
	{
		if(*press == 1 && *MouseX >= 231 && *MouseX <= 381)
		{
			if(*MouseX != min[n])
			{
				min[n] = *MouseX;
				clrmous(*MouseX, *MouseY);
				setcolor(WHITE);
				setfillstyle(1,WHITE);
				bar(231,70+15+60*n,531,86+15+60*n);
				fillellipse(231,78+15+60*n,8,8);
				fillellipse(531,78+15+60*n,8,8);
				setfillstyle(1,LIGHTGRAY);
				fillellipse(*(min + n),78+15+60*n,8,8);
				fillellipse(*(max + n),78+15+60*n,8,8);
			}
			return;
		}
		else if(*press == 1 && *MouseX<231 && *(min+n) !=231)
		{
			*(min+n) = 231;
			clrmous(*MouseX, *MouseY);
			setcolor(WHITE);
			setfillstyle(1,WHITE);
			bar(231,70+15+60*n,531,86+15+60*n);
			fillellipse(231,78+15+60*n,8,8);
			fillellipse(531,78+15+60*n,8,8);
			setfillstyle(1,LIGHTGRAY);
			fillellipse(231,78+15+60*n,8,8);
			fillellipse(*(max+n),78+15+60*n,8,8);
			return;
		}
		else if(*press == 1 && *MouseX>381 && *(min+n) != 381)
		{
			*(min+n) = 381;
			clrmous(*MouseX, *MouseY);
			setcolor(WHITE);
			setfillstyle(1,WHITE);
			bar(231,70+15+60*n,531,86+15+60*n);
			fillellipse(231,78+15+60*n,8,8);
			fillellipse(531,78+15+60*n,8,8);
			setfillstyle(1,LIGHTGRAY);
			fillellipse(381,78+15+60*n,8,8);
			fillellipse(*(max+n),78+15+60*n,8,8);
			return;
		}
		else 
			break;
	}
}

/**************************************
FUNCTION: move_circle2
DESCRIPTION: 右半部分的拖动条
INPUT: MouseX,MouseY,press,min,max,n
RETURN: 无
***************************************/
void move_circle2(int *MouseX,int *MouseY,int *press,int *min,int *max,int n)
{
	while(1)
	{
		clrmous(*MouseX, *MouseY);
		if(*press == 1&& *MouseX >= 381 && *MouseX <= 531)
		{
			if(*MouseX != *(max+n))
			{
				*(max+n) = *MouseX;
				clrmous(*MouseX, *MouseY);
				setcolor(WHITE);
				setfillstyle(1,WHITE);
				bar(231,70+15+60*n,531,86+15+60*n);
				fillellipse(231,78+15+60*n,8,8);
				fillellipse(531,78+15+60*n,8,8);
				setfillstyle(1,LIGHTGRAY);
				fillellipse(*(min+n),78+15+60*n,8,8);
				fillellipse(*(max+n),78+15+60*n,8,8);
				return;
			}
			return;
		}
		else if(*press == 1 && *MouseX < 381 && *(max+n) !=381)
		{
			*(max+n) = 381;
			clrmous(*MouseX, *MouseY);
			setcolor(WHITE);
			setfillstyle(1,WHITE);
			bar(231,70+15+60*n,531,86+15+60*n);
			fillellipse(231,78+15+60*n,8,8);
			fillellipse(531,78+15+60*n,8,8);
			setfillstyle(1,LIGHTGRAY);
			fillellipse(*(min+n),78+15+60*n,8,8);
			fillellipse(381,78+15+60*n,8,8);
			return;
		}
		else if(*press == 1 && *MouseX > 531 && *(max+n) != 531)
		{
			*(max+n) = 531;
			clrmous(*MouseX, *MouseY);
			setcolor(WHITE);
			setfillstyle(1,WHITE);
			bar(231,70+15+60*n,531,86+15+60*n);
			fillellipse(231,78+15+60*n,8,8);
			fillellipse(531,78+15+60*n,8,8);
			setfillstyle(1,LIGHTGRAY);
			fillellipse(*(min+n),78+15+60*n,8,8);
			fillellipse(531,78+15+60*n,8,8);
			return;
		}
		else 
			break;
	}
}

/**************************************
FUNCTION: fundamental_draw
DESCRIPTION: 绘制基本面界面
INPUT: void
RETURN: 无
***************************************/
void fundamental_draw(void)
{
	int i = 0;
	puthz(4,40,"启用",16,17,WHITE);
	puthz(57,40,"条件",16,17,WHITE);
	puthz(178,40,"最低",16,17,WHITE);
	puthz(231,40,"取值范围",16,17,WHITE);
	puthz(552,40,"最高",16,17,WHITE);
	puthz(605,40,"单位",16,17,WHITE);
	settextjustify(LEFT_TEXT,TOP_TEXT);
	settextstyle(3,0,2);

	
	for(i=0;i<5;i++)
	{
		setfillstyle(1,WHITE);
		bar(4+2,70+i*60+15,20+2,86+i*60+15);
		setcolor(GREEN);
		outtextxy(178+2,66+i*60+15,"0");
		bar(231,70+i*60+15,531,86+i*60+15);
		setcolor(WHITE);
		fillellipse(231,78+i*60+15,8,8);
		fillellipse(531,78+i*60+15,8,8);
		setcolor(RED);
		outtextxy(552,66+i*60+15,"100");
		setcolor(WHITE);
		outtextxy(605,66+i*60+15,"%");
		setfillstyle(1,LIGHTGRAY);
		fillellipse(231,78+i*60+15,8,8);
		fillellipse(531,78+i*60+15,8,8);
	}
	setcolor(BLUE);
	puthz(77-20,85,"市盈率",16,17,WHITE);
	puthz(77-20,85+60,"资产负债率",16,17,WHITE);
	puthz(77-20,85+60*2,"市净率",16,17,WHITE);
	puthz(77-20,85+60*3,"净利润率",16,17,WHITE);
	puthz(77-20,85+60*4,"权益比",16,17,WHITE);
	
	setfillstyle(1,RED);
	bar(240,390,400,430);
	puthz(271,398,"开始选股",24,25,WHITE);
}

/************************************
FUNCTION: fundam_judge
DESCRIPTION: 在判断出画勾
INPUT: count,n
RETURN: 无
*************************************/
void fundam_judge(int *count, int n)
{
	delay(300);                //解决鼠标press值不清楚的问题
	if(*(count+n) == 0)
	{
		setcolor(BLUE);
		line(8,94+60*n,13,99+60*n);              //画勾，从结果上来看，画出了勾即为选定，也即值为1
		line(13,99+60*n,19,86+60*n);
		*(count+n) = 1;
		return ;
	}
	else 
	{
		setcolor(WHITE);
		line(8,94+60*n,13,99+60*n);
		line(13,99+60*n,19,86+60*n);
		*(count+n) = 0;
		return ;
	}
}

/**********************************************
FUNCTION: lightbutton_fundamental
DESCRIPTION: 基本面点亮 
INPUT: void
RETURN: 无
***********************************************/
void lightbutton_fundamental(void)
{
	clrmous(MouseX, MouseY);
	delay(10);
	setcolor(WHITE);
	setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
	setfillstyle(1, RED);
	bar(240,390,400,430);
	rectangle(240,390,400,430);
	puthz(271,398,"开始选股",24,25,LIGHTRED);
}

void recoverbutton_fundamental(void)
{
	clrmous(MouseX, MouseY);
	setfillstyle(1,RED);
	bar(240,390,400,430);
	puthz(271,398,"开始选股",24,25,WHITE);
}
