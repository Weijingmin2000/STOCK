#include"public.h"
#include"control.h"
#include"open.h"
#include"queue.h"
#include"trade.h"
#include"market.h"
#include"qsearch.h"
#include"traverse.h"
#include"techslc.h"
#include"person.h"
#include"about.h"

/*****************************************
control.c
COPYRIGHT:	dengshumin
FUNCTION:	control mouse,key,and func
ABSTRACT:
		A.control and invoke function
		B.button lightning and recovering
		C.menu and submenu
DATE:2019/10/1
******************************************/



/******************************************
FUNCTION:control_main
DESCRIPTION:调用其他函数，沪深界面鼠标键盘的控制
INPUT:*func,u
RETURN:无
******************************************/
void control_main(int* func,USER* u)       //u为用户信息指针
{
	int i = 0, j = 0, k = 0, pre_k = 1;
	int page = 0;
	int minute;
	StoQueue* Q = NULL;
	struct tm *tp = NULL;
	Ctr cinfo;
	int pos[10] = {0};   				//存放每一页初始字节的数组
	int sto_No[20] = { 0 };				//存放股票序号的数组，与行情搜索功能互补
	int sto_list[200] = { 0 };
	int sto = 1;						//用来给股票序号数组赋值的变量
	int sto_num = 0;					//用来哈希搜索的整形
	char name[20] = { '\0' };			//用来哈希搜索的名称

	pos[0] = 0;
	memset(&cinfo,0,sizeof(Ctr));  		//初始化控制信息
	for (i = 1; i < 10; i++)
	{
		pos[i] = countqueue(pos[i-1]);//递归，算每页在文件中开始的位置
	}
	if ((Q = (StoQueue*)malloc(sizeof(StoQueue))) == NULL)
	{
		printf("memoryallocation runs wrong in control.c");
		delay(3000);
		exit(1);
	}
	clrmous(MouseX, MouseY);
	delay(100);
	cleardevice();
	draw_stoqueue();
	mainmenu_draw();
	control_judge(u);
	get_time(tp, &minute, 1);
	initqueue(Q);//初始化后的文件指针位置
	for (j = 0; j < 20; j++)
	{
		sto_No[j] = sto;//赋初值1-20
		sto++;
	}
	i = 0;//初始化页面
	if(strlen(u->user) != 0)
		update_hold_day(tp,u);              //更新持股天数
	while(1)
	{
		//tp->tm_
		if(!bioskey(1))
		{
			newmouse(&MouseX,&MouseY,&press);
			control_mouse(&cinfo,u);
		}
		else if(bioskey(1))
		{
			cinfo.key = bioskey(0);
			if (cinfo.key == DOWN)
			{
				queuedown(&sto, sto_No, 20);	//给用于搜索的sto_No赋每一页的值
				i=(i+1) % 10;					//若到最后一面，则转到第一面
				clrmous(MouseX,MouseY);
				cleardevice();
				draw_stoqueue();
				mainmenu_draw();
				control_judge(u);
				inputqueue(Q, pos[i]);
				
				save_bk_mou(MouseX,MouseY);
				drawmous(MouseX,MouseY);   		//重现鼠标       
				cinfo.key = 0;
				get_time(tp, &minute, 1);
				continue;
			}
			else if (cinfo.key == UP)
			{
				queueup(&sto, sto_No, 20);		//给用于搜索的sto_No赋每一页的值
				i--;
				if (i == -1)
				{
					i = 9;//到最后一面
					//break;
				}
				clrmous(MouseX,MouseY);
				cleardevice();
				draw_stoqueue();
				mainmenu_draw();
				control_judge(u);
				inputqueue(Q, pos[i]);
				save_bk_mou(MouseX,MouseY);
				drawmous(MouseX,MouseY);
				cinfo.key = 0;
				get_time(tp, &minute, 1);
				continue;
			}   //先判断up和down键
			else
				control_key(&cinfo);
		} //至此，每次键盘控制后都清零了

		if(cinfo.func == 2)          //注销键退出
		{
			*func = cinfo.func;
			destroyqueue(Q);    	 //销毁队列
			return;   				 //返回的时候，栈区的Ctr变量会释放掉
		}
		if(control_menu(&cinfo))
		{
			clrmous(MouseX,MouseY);
			draw_stoqueue();
			control_judge(u);
			inputqueue(Q, pos[i]);
			get_time(tp, &minute, 1);
			submenu_draw(cinfo.itemnum);
		}
		if(control_func(&cinfo, u, sto_list, &page))
		{
			cinfo.ord = 0;
			cinfo.ord_last = 0;
			cinfo.itemnum = 0;
			cinfo.itemnum_last = 0;
			draw_stoqueue();
			inputqueue(Q, pos[i]);
			get_time(tp, &minute, 1);
			continue;
		}
		if (MouseY > 50 && MouseY < 456 && cinfo.itemnum == 0)
		{
			/*if (mouse_press(0, 50, 640, 456) == 3)*/
			
			if (mouse_press(0, 50, 640, 456) == 2)
			{
				continue;
			}
			else if (mouse_press(0, 50, 640, 456) == 1)//搜索算法(wjm)
			{
				if ((sto_num = queuesearch(sto_No, &pre_k, &k)) != 0)  //先跳转回来
				{
					search_name(sto_num, name);//股票序号到名称的搜索
					if (strlen(name) == 0)
					{
						//printf("wrong");
					}
					sto_market(&cinfo, u, sto_num, name);
					if (cinfo.func == 0)
					{
						cleardevice();
						mainmenu_draw();
						control_judge(u);
						draw_stoqueue();
						inputqueue(Q, pos[i]);
						get_time(tp, &minute, 1);           //再根据func的值进入不同函数
						continue;
					}
					continue;
				}
			}
		/*	else if (mouse_press(0, 50, 640, 456) == 3)
			{//右键点击加入自选股
				sto_num = sto_No[(MouseY - 50) / 20];
				add_fav(u, sto_num);
			}*/
		}
	}
}

/************************************************************************
FUNCTION:control_mouse
DESCRIPTION: 鼠标点击应对菜单功能
ATTENTION: 因为第2、3、6个菜单无子菜单，所以在点击之后函数功能号发生改变，
			但最终菜单号又变为空
INPUT:cinfo,u
RETURN:无
************************************************************************/
void control_mouse(Ctr* cinfo,USER* u)    //通过鼠标来选择菜单项并给菜单、函数、子菜单标号
{
	if(cinfo->itemnum == 0)
	{
		if(mouse_press(0,0,60,24) == 1)
		{
			cinfo->itemnum = 1;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
			//return 可能要返回循环
		}
		else if(mouse_press(60,0,120,24) == 1)
		{
			cinfo->itemnum = 2;
			cinfo->func = 0;         //沪深函数
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(120,0,180,24) == 1)
		{
			cinfo->itemnum = 3;
			cinfo->func = 8;         //行情函数
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(180,0,240,24) == 1)
		{
			cinfo->itemnum = 4;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(240,0,300,24) == 1)
		{
			cinfo->itemnum = 5;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(300,0,360,24) == 1)
		{
			cinfo->itemnum = 6;
			if(strlen(u->user) != 0)
			{
				if(strlen(u->bankcard) != 0)      //登录了而且开了户
				{
					cinfo->func = 14;        //个人菜单
				}
				else//未开户
				{
					cpop(2);
				}
			}
			else
			{
				clrmous(MouseX, MouseY);
				cpop(1);
				//cinfo->itemnum = 0;
			}
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		return;
	}
	else if(cinfo->itemnum == 1)
	{
		if(mouse_press(0,0,60,24) == 1)
		{
			cinfo->itemnum = 1;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(60,0,120,24) == 1)
		{
			cinfo->itemnum = 2;
			cinfo->func = 0;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(120,0,180,24) == 1)
		{
			cinfo->itemnum = 3;
			cinfo->func = 8;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(180,0,240,24) == 1)
		{
			cinfo->itemnum = 4;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(240,0,300,24) == 1)
		{
			cinfo->itemnum = 5;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(300,0,360,24) == 1)
		{
			cinfo->itemnum = 6;
			if(strlen(u->user) != 0)
			{
				if(strlen(u->bankcard) != 0)      //登录了而且开了户
				{
					cinfo->func = 14;        //个人
				}
				else//未开户
				{
					cpop(2);
				}
			}
			else
			{
				clrmous(MouseX, MouseY);
				cpop(1);
				//cinfo->itemnum = 0;
			}
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(360,0,640,480) == 1||
				mouse_press(0,120,640,480) == 1||
				mouse_press(60,24,640,480) == 1)
				{
					cinfo->itemnum = 0;
					//cinfo->itemnum_last = 0;
					cinfo->ord = 0;
					cinfo->ord_last = 0;
				}
		if(mouse_press(0,24,60,48) == 1)
		{
			if(strlen(u->user) == 0)   //如果未登录
			{
				cinfo->func = 2;        //函数号为2并回到上层
				//destroyqueue(Q);      //销毁队列
				return ;
			}                         //若登录则不响应，即跳过
		}
		else if(mouse_press(0,48,60,72) == 1)
		{
			delay(1000);
			closegraph();
			exit(1);
		}
		else if(mouse_press(0,72,60,96) == 1)
		{
			if(strlen(u->user) != 0)  //若已登录S
			{
				//cinfo->func = 2;
				memset(u,'\0',sizeof(USER));   //直接将登陆状态取消
				control_judge(u);
				return;      //回到循环中去
			}
		}
		else if(mouse_press(0,96,60,120) == 1)
		{
			cinfo->func = 15; //关于界面
		}
		else if(mouse_press(0,24,60,48) == 2)
		{
			cinfo->ord = 1;
		}
		else if(mouse_press(0,48,60,72) == 2)
		{
			cinfo->ord = 2;
		}
		else if(mouse_press(0,72,60,96) == 2)
		{
			cinfo->ord = 3;
		}
		else if(mouse_press(0,96,60,120) == 2)
		{
			cinfo->ord = 4;
		}
	}
	else if(cinfo->itemnum == 2)
	{
		if(mouse_press(0,0,60,24) == 1)
		{
			cinfo->itemnum = 1;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(60,0,120,24) == 1)
		{
			cinfo->itemnum = 2;
			cinfo->func = 0;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(120,0,180,24) == 1)
		{
			cinfo->itemnum = 3;
			cinfo->func = 8;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(180,0,240,24) == 1)
		{
			cinfo->itemnum = 4;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(240,0,300,24) == 1)
		{
			cinfo->itemnum = 5;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(300,0,360,24) == 1)
		{
			cinfo->itemnum = 6;
			if(strlen(u->user) != 0)
			{
				if(strlen(u->bankcard) != 0)      //登录了而且开了户
				{
					cinfo->func = 14;        //个人菜单
				}
				else//未开户
				{
					cpop(2);
				}
			}
			else
			{
				clrmous(MouseX, MouseY);
				cpop(1);
				//cinfo->itemnum = 0;
			}
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
	}
	else if(cinfo->itemnum == 3)
	{
		if(mouse_press(0,0,60,24) == 1)
		{
			cinfo->itemnum = 1;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(60,0,120,24) == 1)
		{
			cinfo->itemnum = 2;
			cinfo->func = 0;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(120,0,180,24) == 1)
		{
			cinfo->itemnum = 3;
			cinfo->func = 8;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(180,0,240,24) == 1)
		{
			cinfo->itemnum = 4;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(240,0,300,24) == 1)
		{
			cinfo->itemnum = 5;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(300,0,360,24) == 1)
		{
			cinfo->itemnum = 6;
			if(strlen(u->user) != 0)
			{
				if(strlen(u->bankcard) != 0)      //登录了而且开了户
				{
					cinfo->func = 14;        //个人菜单
				}
				else//未开户
				{
					cpop(2);
				}
			}
			else
			{
				clrmous(MouseX, MouseY);
				cpop(1);
			//	cinfo->itemnum = 0;
			}
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
	}
	else if(cinfo->itemnum == 4)
	{
		if(mouse_press(0,0,60,24) == 1)
		{
			cinfo->itemnum = 1;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(60,0,120,24) == 1)
		{
			cinfo->itemnum = 2;
			cinfo->func = 0;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(120,0,180,24) == 1)
		{
			cinfo->itemnum = 3;
			cinfo->func = 8;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(180,0,240,24) == 1)
		{
			cinfo->itemnum = 4;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(240,0,300,24) == 1)
		{
			cinfo->itemnum = 5;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(300,0,360,24) == 1)
		{
			cinfo->itemnum = 6;
			if(strlen(u->user) != 0)
			{
				if(strlen(u->bankcard) != 0)      //登录了而且开了户
				{
					cinfo->func = 14;        //个人菜单
				}
				else//未开户
				{
					cpop(2);
				}
			}
			else
			{
				clrmous(MouseX, MouseY);
				cpop(1);
			//	cinfo->itemnum = 0;
			}
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(360,0,640,480) == 1||
				mouse_press(0,0,180,480) == 1||
				mouse_press(180,96,640,480) == 1||
				mouse_press(260,24,640,480) == 1)
				{
					cinfo->itemnum = 0;
					cinfo->ord = 0;
					cinfo->ord_last = 0;
				}
		if(mouse_press(180,24,260,48) == 2)
		{
			cinfo->ord = 5;
		}
		else if(mouse_press(180,48,260,72) == 2)
		{
			cinfo->ord = 6;
		}
		else if(mouse_press(180,72,260,96) == 2)
		{
			cinfo->ord = 7;
		}
		else if(mouse_press(180,24,260,48) == 1)
		{
			cinfo->func = 9; //基本面  
		}
		else if(mouse_press(180,48,260,72) == 1)
		{
			cinfo->func = 10; //技术面
		}
	/*	else if(mouse_press(180,72,260,96) == 1)
		{
			cinfo->func = 11; //快速选股
		}*/
	}
	else if(cinfo->itemnum == 5)
	{
		if(mouse_press(0,0,60,24) == 1)
		{
			cinfo->itemnum = 1;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(60,0,120,24) == 1)
		{
			cinfo->itemnum = 2;
			cinfo->func = 0;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(120,0,180,24) == 1)
		{
			cinfo->itemnum = 3;
			cinfo->func = 8;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(180,0,240,24) == 1)
		{
			cinfo->itemnum = 4;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(240,0,300,24) == 1)
		{
			cinfo->itemnum = 5;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(300,0,360,24) == 1)
		{
			cinfo->itemnum = 6;
			if(strlen(u->user) != 0)
			{
				if(strlen(u->bankcard) != 0)      //登录了而且开了户
				{
					cinfo->func = 14;        //个人菜单
				}
				else//未开户
				{
					cpop(2);
				}
			}
			else
			{
				clrmous(MouseX, MouseY);
				cpop(1);
			//	cinfo->itemnum = 0;
			}
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(360,0,640,480) == 1||
				mouse_press(0,0,240,480) == 1||
				mouse_press(240,72,640,480) == 1||
				mouse_press(300,24,640,480) == 1)
				{
					cinfo->itemnum = 0;
				//	cinfo->itemnum_last = 0;
					cinfo->ord = 0;
					cinfo->ord_last = 0;
				}
		if(mouse_press(240,24,300,48) == 2)
		{
			cinfo->ord = 8;
		}
		else if(mouse_press(240,48,300,72) == 2)
		{
			cinfo->ord = 9;
		}
		else if(mouse_press(240,24,300,48) == 1)
		{
			if(strlen(u->user) == 0)   //未登录
			{
				cpop(1);//提示请先登录
				cinfo->itemnum = 0;
			}
			else                    //已登录
			{
				cinfo->func = 12;
			}
		}
		else if(mouse_press(240,48,300,72) == 1)
		{   //换股
			if(strlen(u->user) == 0)
			{
				cpop(1);//提示请先登录
				cinfo->itemnum = 0;
			}
			else 
			{
				if(strlen(u->bankcard) == 0)
				{
					cpop(2); //提示请先开户
					cinfo->itemnum = 0;
				}
				else 
				{
					cinfo->func = 13;  //换股函数
				}
			}
		}
	}
	else if(cinfo->itemnum == 6)
	{
		if(mouse_press(0,0,60,24) == 1)
		{
			cinfo->itemnum = 1;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(60,0,120,24) == 1)
		{
			cinfo->itemnum = 2;
			cinfo->func = 0;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(120,0,180,24) == 1)
		{
			cinfo->itemnum = 3;
			cinfo->func = 8;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(180,0,240,24) == 1)
		{
			cinfo->itemnum = 4;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(240,0,300,24) == 1)
		{
			cinfo->itemnum = 5;
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(300,0,360,24) == 1)
		{
			cinfo->itemnum = 6;
			if(strlen(u->user) != 0)
			{
				if(strlen(u->bankcard) != 0)      //登录了而且开了户
				{
					cinfo->func = 14;        //个人菜单
				}
				else//未开户
				{
					cpop(2);
				}
			}
			else
			{
				clrmous(MouseX, MouseY);
				cpop(1);
			//	cinfo->itemnum = 0;
			}
			cinfo->ord = 0;
			cinfo->ord_last = 0;
		}
		else if(mouse_press(360,0,640,480) == 1||
				mouse_press(0,0,360,480) == 1||
				mouse_press(360,72,640,480) == 1)
				{
					cinfo->itemnum = 0;
					cinfo->itemnum_last = 0;
					cinfo->ord = 0;
					cinfo->ord_last = 0;
				}
	}
	if(cinfo->itemnum == 2 || cinfo->itemnum == 3 || cinfo->itemnum == 6)
		cinfo->itemnum = 0;   //如果选择沪深、行情、个人,则将他转变为空菜单
	delay(30);
}

/**************************************************
FUNCTION:control_key
DESCRIPTION: 通过键盘快捷键控制菜单
INPUT: cinfo
RETURN:无
***************************************************/
void control_key(Ctr* cinfo)              //键盘控制函数
{
	if(cinfo->key != 0)
	{
		if(cinfo->key == F1)
		{
			cinfo->itemnum = 1;
		}
		else if(cinfo->key == F2)
		{
			cinfo->itemnum = 2;
			cinfo->func = 0;
		}
		else if(cinfo->key == F3)
		{
			cinfo->itemnum = 3;
			cinfo->func = 8;
		}
		else if(cinfo->key == F4)
		{
			cinfo->itemnum = 4;
		}
		else if(cinfo->key == F5)
		{
			cinfo->itemnum = 5;
		}
		else if(cinfo->key == F6)
		{
			cinfo->itemnum = 6;
			cinfo->func = 14;
		}	
		cinfo->key = 0;            //每次作用完之后把key清零
	}
}

/*************************************************************************
FUNCTION:control_func
DESCRIPTION: 函数的切换控制
ATTENTION: 在每次实现了函数之后，存在着一个判定，是否返回主界面即沪深界面
INPUT: cinfo,u,sto_list,page
RETURN: 若返回主界面则返回1，否则返回0
**************************************************************************/
int control_func(Ctr* cinfo,USER* u,int* sto_list,int* page)    //只在主控函数中使用，别的函数先返回cinfo->func的值，再在主控函数中调用
{
	char name[20] = { 0 };										//用来sto_num搜索到name的字符串
	int sto_num = 0;
	if(cinfo->func == 8)
	{
		cinfo->itemnum = 0;
		cinfo->itemnum_last = 0;

		search_name(1, name);
		if (strlen(name) == 0)
		{
			//printf("wrong");
		}
		sto_market(cinfo, u, 1, name);							//行情函数,1为第一只股
		memset(name, '\0', sizeof(name));
		
		if(cinfo->func == 0)          							//说明是返回至沪深
		{
			clrmous(MouseX,MouseY);
			cleardevice();
			mainmenu_draw();
			control_judge(u);
			return 1;
		}; //行情函数
	}
	else if(cinfo->func == 9)
	{
		cinfo->itemnum = 0;
		cinfo->itemnum_last = 0;
		*page = 4;
		sto_num = technical_control(cinfo, u, sto_list, page);		//基本面

		search_name(sto_num, name);									//股票序号到名称的搜索
		if (strlen(name) == 0)
		{
			//printf("wrong");
		}
		if (sto_num != 0)
		{
			sto_market(cinfo, u, sto_num, name);
			memset(name, '\0', sizeof(name));
		}

		if(cinfo->func == 0)         								 //说明是按了沪深返回
		{
			clrmous(MouseX,MouseY);
			cleardevice();
			mainmenu_draw();
			control_judge(u);
			return 1;
		} 							
	}
	else if(cinfo->func == 10)
	{
		cinfo->itemnum = 0;
		cinfo->itemnum_last = 0;
		*page = 1;
		sto_num = technical_control(cinfo, u, sto_list, page);		//技术面

		search_name(sto_num, name);//股票序号到名称的搜索
		if (sto_num != 0)
		{
			sto_market(cinfo, u, sto_num, name);
			memset(name, '\0', sizeof(name));
		}

		if(cinfo->func == 0)     									    //说明是按了沪深返回
		{
			clrmous(MouseX,MouseY);
			cleardevice();
			mainmenu_draw();
			control_judge(u);
			return 1;
		}
	}
	else if(cinfo->func == 12)
	{
		cinfo->itemnum = 0;
		cinfo->itemnum_last = 0;
		opaccount(cinfo,u);			  //开户函数
		if(cinfo->func == 0)          //说明是按了沪深返回来的
		{
			clrmous(MouseX,MouseY);
			cleardevice();
			mainmenu_draw();
			control_judge(u);
			return 1;
		}
	}
	else if(cinfo->func == 13)
	{
		cinfo->itemnum = 0;
		cinfo->itemnum_last = 0;
		trade(cinfo, u, 0, 0);  			  //换股函数
		if(cinfo->func == 0)       			   //说明是按了沪深返回
		{
			clrmous(MouseX,MouseY);
			cleardevice();
			mainmenu_draw();
			control_judge(u);
			return 1;
		}
	}
	else if(cinfo->func == 14)      //个人
	{
		cinfo->itemnum = 0;
		cinfo->itemnum_last = 0;
		personinfo(cinfo, u);        //个人函数
		if(cinfo->func == 0)
		{
			clrmous(MouseX,MouseY);
			cleardevice();
			mainmenu_draw();
			control_judge(u);
			return 1;
		}
	}
	else if(cinfo->func == 15)       //关于
	{
		cinfo->itemnum = 0;
		cinfo->itemnum_last = 0;
		About(cinfo, u);
		if(cinfo->func == 0)
		{
			clrmous(MouseX, MouseY);
			cleardevice();
			mainmenu_draw();
			control_judge(u);
			return 1;
		}
	}	
	return 0;
}

/********************************************************************
FUNCTION: control_menu
DESCRIPTION: 菜单切换与展开以及子菜单标亮控制
INPUT: cinfo
RETURN: 若原来有菜单则返回为1，如果为子菜单标亮或者是空菜单则会返回为0
********************************************************************/
int control_menu(Ctr* cinfo)          //返值为0说明是标亮变换或者是原来为空菜单
{
	if(cinfo->itemnum != cinfo->itemnum_last)
	{
		cinfo->ord = 0;              //菜单切换的时候把ord和ord_last清零,防止不正常标亮
		cinfo->ord_last = 0;
		
		if(cinfo->itemnum_last != 0)
		{
			cleardevice();
			clrmous(MouseX,MouseY);
			mainmenu_draw();
			//submenu_draw(cinfo->itemnum);
			cinfo->itemnum_last = cinfo->itemnum;
			return 1;
		}
		else 
		{
			clrmous(MouseX,MouseY);
			submenu_draw(cinfo->itemnum);
			cinfo->itemnum_last = cinfo->itemnum;
			return 0;
		}
	}
	
	if(cinfo->ord != cinfo->ord_last)
	{
		lightbutton_menu(cinfo->ord);
		recoverbutton_menu(cinfo->ord_last);
		cinfo->ord_last = cinfo->ord;
		return 0;
	}
	return 0;
}

/*************************************
FUNCTION: mainmenu_draw
DESCRIPTION: 绘制主菜单
INPUT: void
RETURN: 无
**************************************/
void mainmenu_draw(void)
{
	setbkcolor(BLACK);
	setfillstyle(1,LIGHTGRAY);
	setcolor(RED);
	setlinestyle(0,0,2);
	bar(0,0,640,24);                        //宽24
	rectangle(0,0,639,479);

	puthz(10,5,"系统",16,24,BLACK);     
	line(60,1,60,24);

	puthz(70,5,"沪深",16,24,BLACK);
	line(120,1,120,24);

	puthz(130,5,"行情",16,24,BLACK);
	line(180,1,180,24);

	puthz(190,5,"选股",16,24,BLACK);
	line(240,1,240,24);

	puthz(250,5,"交易",16,24,BLACK);
	line(300,1,300,24);

	puthz(310,5,"个人",16,24,BLACK);
	line(360,1,360,24);
	
//	bar(0,480,640,456);
}

/**********************************************
FUNCTION: submenu_draw
DESCRIPTION: 绘制子菜单
INPUT: cinfo->itemnum
RETURN:无
**********************************************/
void submenu_draw(short int n)
{
	setfillstyle(1,WHITE);
	switch(n)
	{
		case 1:
			clrmous(MouseX,MouseY);
			delay(10);
			bar(0,24,60,120);
			puthz(10,29,"登陆",16,24,DARKGRAY);             //0,25,60,48
			puthz(10,53,"退出",16,24,DARKGRAY);             //0,48,60,72
			puthz(10,77,"注销",16,24,DARKGRAY);             //0,72,60,96
			puthz(10,101,"关于",16,24,DARKGRAY);             //0,96,60,120
			break;
		case 2:
			clrmous(MouseX,MouseY);
			delay(10);
			break;
		case 3:
			clrmous(MouseX,MouseY);
			delay(10);
			break;
		case 4:
			clrmous(MouseX,MouseY);
			delay(10);
			bar(180,24,260,72);                                //选股菜单
			puthz(183,29,"基本面",16,18,DARKGRAY);                //180,25,260,48
			puthz(183,53,"技术面",16,18,DARKGRAY);                //180,48,260,72
		//	puthz(183,77,"快速选股",16,18,DARKGRAY);              //180,72,260,96
			break;
		case 5:
			clrmous(MouseX,MouseY);
			delay(10);
			bar(240,24,300,72);                                //交易菜单
			puthz(250,29,"开户",16,24,DARKGRAY);                  //240,25,300,48
			puthz(250,53,"换股",16,24,DARKGRAY);                  //240,48,300,72
			break;
		case 6:
			clrmous(MouseX,MouseY);
			delay(10);
			//bar(300,24,380,72);                                //个人菜单
			//puthz(303,29,"资产",16,18,BLACK);                  //300,25,380,48
			//puthz(303,53,"自选股",16,18,BLACK);                //300,48,380,72
			break;
	}
}

/************************************
FUNCTION: lightbutton_menu
DESCRIPTION: 菜单的点亮
INPUT: cinfo->ord
RETURN: 无
*************************************/
void lightbutton_menu(short int ord)
{
	clrmous(MouseX, MouseY);
	delay(10);
	setfillstyle(1,LIGHTBLUE);
	switch(ord)
	{
		case 1:
			bar(0,24,60,48);
			puthz(10,29,"登陆",16,24,WHITE);
			break;
		case 2:
			bar(0,48,60,72);
			puthz(10,53,"退出",16,24,WHITE);
			break;
		case 3:
			bar(0,72,60,96);
			puthz(10,77,"注销",16,24,WHITE);
			break;
		case 4:
			bar(0,96,60,120);
			puthz(10,101,"关于",16,24,WHITE);
			break;
		case 5:
			bar(180,24,260,48);
			puthz(183,29,"基本面",16,18,WHITE);
			break;
		case 6:
			bar(180,48,260,72);
			puthz(183,53,"技术面",16,18,WHITE);
			break;
	/*	case 7:
			bar(180,72,260,96);
			puthz(183,77,"快速选股",16,18,WHITE);
			break;*/
		case 8:
			bar(240,24,300,48);
			puthz(250,29,"开户",16,24,WHITE); 
			break;
		case 9:
			bar(240,48,300,72);
			puthz(250,53,"换股",16,24,WHITE);
			break;
	}
}

/*****************************************
FUNCTION: recoverbutton_menu
DESCRIPTION: 子菜单的恢复
INPUT: cinfo->ord_last
RETURN: 无
*****************************************/
void recoverbutton_menu(short int ord_last)
{	
	clrmous(MouseX, MouseY);
	setfillstyle(1,WHITE);
	switch(ord_last)
	{
		case 1:
			bar(0,24,60,48);
			puthz(10,29,"登陆",16,24,DARKGRAY);
			break;
		case 2:
			bar(0,48,60,72);
			puthz(10,53,"退出",16,24,DARKGRAY); 
			break;
		case 3:
			bar(0,72,60,96);
			puthz(10,77,"注销",16,24,DARKGRAY); 
			break;
		case 4:
			bar(0,96,60,120);
			puthz(10,101,"关于",16,24,DARKGRAY);
			break;
		case 5:
			bar(180,24,260,48);
			puthz(183,29,"基本面",16,18,DARKGRAY);                
			break;
		case 6:
			bar(180,48,260,72);
			puthz(183,53,"技术面",16,18,DARKGRAY);
			break;
	/*	case 7:
			bar(180,72,260,96);
			puthz(183,77,"快速选股",16,18,DARKGRAY); 
			break;*/
		case 8:
			bar(240,24,300,48);
			puthz(250,29,"开户",16,24,DARKGRAY);
			break;
		case 9:
			bar(240,48,300,72);
			puthz(250,53,"换股",16,24,DARKGRAY);
			break;
	}
}

/******************************************
FUNCTION: draw_stoqueue
DESCRIPTION: 绘制沪深界面
INPUT: void
RETURN: 无
******************************************/
void draw_stoqueue(void)//代码	名称	涨幅%	现价	涨跌	买价	卖价	总量	现量	涨速%	换手%	今开
{
	setbkcolor(BLACK);
	puthz(5, 30, "代码", 16, 17, LIGHTGRAY);
	puthz(60, 30, "名称", 16, 17, LIGHTGRAY);

	puthz(115, 30, "涨幅", 16, 17, LIGHTGRAY);
	setcolor(LIGHTGRAY);
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(1, HORIZ_DIR, 1);
	outtextxy(148, 25, "%");

	puthz(172, 30, "现价", 16, 17, LIGHTGRAY);
	puthz(220, 30, "涨跌", 16, 17, LIGHTGRAY);
	puthz(270, 30, "买价", 16, 17, LIGHTRED);
	puthz(320, 30, "卖价", 16, 17, LIGHTGREEN);
	puthz(377, 30, "总量", 16, 17, LIGHTGRAY);
	puthz(440, 30, "现量", 16, 17, LIGHTGRAY);

	puthz(488, 30, "涨速", 16, 17, LIGHTRED);
	setcolor(LIGHTRED);
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(1, HORIZ_DIR, 1);
	outtextxy(521, 25, "%");

	puthz(540, 30, "换手", 16, 17, LIGHTGRAY);
	setcolor(LIGHTGRAY);
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(1, HORIZ_DIR, 1);
	outtextxy(573, 25, "%");

	puthz(597, 30, "今开", 16, 17, LIGHTGRAY);
	setlinestyle(SOLID_LINE, 0, 1);
	line(45, 24, 45, 456);
	line(112, 24, 112, 456);
	line(162, 24, 162, 456);
	line(210, 24, 210, 456);
	line(260, 24, 260, 456);
	line(310, 24, 310, 456);
	line(360, 24, 360, 456);
	line(425, 24, 425, 456);
	line(483, 24, 483, 456);
	line(537, 24, 537, 456);
	line(587, 24, 587, 456);
	line(0, 456, 640, 456);
}

/*****************************************
FUNCTION: control_judge
DESCRIPTION: 判断用户状态
INPUT: u
RETURN: 无
*****************************************/
void control_judge(USER* u)
{
	setfillstyle(1, LIGHTGRAY);
	bar(560, 1, 640, 23);          //遮盖
	if(strlen(u->user) == 0)
	{
		puthz(566,5,"游客状态",16,17,LIGHTCYAN);
	}
	else
	{
		puthz(566,5,"用户登录",16,17,LIGHTBLUE);
	}
}

/*********************************
FUNCTION: cpop
DESCRIPTION: 弹窗提示
INPUT: i
RETURN: 无
**********************************/
void cpop(int i)
{
	clrmous(MouseX, MouseY);
	if(i == 1)
	{
		setfillstyle(SOLID_FILL, LIGHTGRAY);
		bar(180, 150, 460, 330);
		setfillstyle(SOLID_FILL, LIGHTRED);
		bar(200, 170, 440, 310);
		puthz(250, 200, "请先登录", 16, 20, YELLOW);
		puthz(250, 260, "将在", 16, 20, YELLOW);
		settextjustify(LEFT_TEXT, TOP_TEXT);
		settextstyle(2, HORIZ_DIR, 6);
		setcolor(WHITE);
		outtextxy(295, 260, "3");
		puthz(310, 260, "秒后返回", 16, 20, YELLOW);
		delay(1000);
		setfillstyle(SOLID_FILL, LIGHTRED);
		bar(295, 260, 308, 273);
		outtextxy(295, 260, "2");
		delay(1000);
		bar(295, 260, 308, 273);
		outtextxy(295, 260, "1");
		delay(1000);
	}
	else if(i == 2)
	{
		setfillstyle(SOLID_FILL, LIGHTGRAY);
		bar(180, 150, 460, 330);
		setfillstyle(SOLID_FILL, LIGHTRED);
		bar(200, 170, 440, 310);
		puthz(250, 200, "请先开户", 16, 20, YELLOW);
		puthz(250, 260, "将在", 16, 20, YELLOW);
		settextjustify(LEFT_TEXT, TOP_TEXT);
		settextstyle(2, HORIZ_DIR, 6);
		setcolor(WHITE);
		outtextxy(295, 260, "3");
		puthz(310, 260, "秒后返回", 16, 20, YELLOW);
		delay(1000);
		setfillstyle(SOLID_FILL, LIGHTRED);
		bar(295, 260, 308, 273);
		outtextxy(295, 260, "2");
		delay(1000);
		bar(295, 260, 308, 273);
		outtextxy(295, 260, "1");
		delay(1000);
	}
}