#include"public.h"
#include"market.h"
#include"storead.h"
#include"techline.h"
#include"marketfc.h"
#include"qsearch.h"
#include"trade.h"
#include"traverse.h"

/*****************************************
market.c
COPYRIGHT:	weijingmin
FUNCTION:	market information
ABSTRACT:
		A.show market
		B.function calling and control
		C.button lighting and recovering
DATE:2019/10/4
******************************************/

/*********************************************
FUNCTION:sto_market
DESCRIPTION：行情界面总控
INPUT:cinfo,u，sto_num,sto_name
RETURN:无
***********************************************/
void sto_market(Ctr* cinfo, USER* u, int sto_num, char* sto_name)//要读取的某只股票的标号;用来从沪深过来的搜索
{
	int i;
	int j = 0;
	int key = 0;
	long int sto_date = 1;//要读取的某只股的信息的开始日期,有点小bug
	int page = 0;//左侧股票列表初始页
	int num = 0;	//按钮标号
	int pos[25] = { 0 };   //存放每一页初始字节的数组
	int sto_No[8] = { 0 };	//存放股票序号的数组，与参数表中的sto_num功能互补
	int day = 0;	//鼠标今日
	int preday = 0;	//鼠标昨日，防止闪烁
	int kind = 0;	//kind为0，则主图画4个分天数据，否则kind为1画3个
	int colorkind = 1;	//不同的colorkind对应不同的副图数据输出
	float max = 0.0, min = 0.0;//边界最大值和最小值
	int read_way = 1;//股票显示的方式，1为每次切换一天，2为30天，3为60天
	int sto = 1;	//用来给股票序号数组赋值的变量
	//int maxline = 0;	//用来存储数据库末端行数的整形数组
	struct tm* tp = NULL;	//时间显示结构体
	int minute;//显示时间
	STODAYFLOAT info[150];	//150天股票信息的结构体数组
	//MarketQueue* Q = NULL;	//股票队列
	MarketQueue Q;
	UPLINE upinfo;	//存放主图信息的结构体
	DOWNLINE downinfo;	//存放副图信息的结构体
	memset(cinfo,0,sizeof(Ctr));	   //将控制信息初始化
	cinfo->func = 4;     		       //用4标记行情函数
	clrmous(MouseX, MouseY);
	delay(100);
	cleardevice();
	draw_market();
	get_markettime(tp, &minute, 1);	//获得时间
	mainmenu_draw();
	control_judge(u);

	memset(info, 0.0, 150 * sizeof(STODAYFLOAT));
	pos[0] = 0;
	put_marketqueue(pos);//为队列分配空间,求得各页位置并初始化
	/*if ((Q = (MarketQueue*)malloc(sizeof(MarketQueue))) == NULL)
	{
		printf("memoryallocation runs wrong");
		delay(3000);
		exit(1);
	}*/
	klinequeue(&Q);//初始化后的文件指针位置
	sto_daybyday2float(sto_num, sto_date, 150, info);//1为股票序号，1为开始天数，150为读取天数
	get_border(info, &max, &min, 150);//150为读取天数
	Kline_draw(info, max, min);
	MA_draw(info, &upinfo, max, min);
	VOL_draw(info, &downinfo);
	sto = 1;
	for (i = 0; i < 8; i++)
	{
		sto_No[i] = sto;//赋初值1-8
		sto++;
	}
	//maxline = check_database(sto_num);//数据库末端的行数返回给maxline
	output_stoname(sto_name);//打印哈希搜索的结果
	while (1)
	{
		if(bioskey(1))
		{
			key = bioskey(0);
			cinfo->key = key;
			if (key == DOWN)
			{
				delay(50);
				page = (page + 1) % 25;//若到最后一面，则转到第一面
				setfillstyle(SOLID_FILL, BLACK);
				bar(1, 145, 71, 429);
				setlinestyle(SOLID_LINE, 0, 1);
				setcolor(RED);
				for (i = 0; i < 9; i++)
				{
					line(0, 144 + 36 * i, 72, 144 + 36 * i);
				}
				input_klinequeue(&Q, pos[page]);
				queuedown(&sto, sto_No, 8);//给用于搜索的sto_No赋每一页的值
				cinfo->key = 0;

				setfillstyle(SOLID_FILL, DARKGRAY);
				bar(73, 154, 79, 427);
				setfillstyle(SOLID_FILL, BLACK);
				bar(73, 170 + 10 * page, 79, 180 + 10 * page);//进度条变换
				continue;
			}
			else if (key == UP)
			{
				delay(50);
				page--;
				if (page == -1)
				{
					page = 24;//到最后一面
				}
				setfillstyle(SOLID_FILL, BLACK);
				bar(1, 145, 71, 429);
				setlinestyle(SOLID_LINE, 0, 1);
				setcolor(RED);
				for (i = 0; i < 9; i++)
				{
					line(0, 144 + 36 * i, 72, 144 + 36 * i);
				}
				input_klinequeue(&Q, pos[page]);
				queueup(&sto, sto_No, 8);//给用于搜索的sto_No赋每一页的值
				cinfo->key = 0;

				setfillstyle(SOLID_FILL, DARKGRAY);
				bar(73, 154, 79, 427);
				setfillstyle(SOLID_FILL, BLACK);
				bar(73, 170 + 10 * page, 79, 180 + 10 * page);//进度条变换
				continue;
			}
			else if (key == LEFT)
			{
				delay(50);
				memset(info, 0.0, 150 * sizeof(STODAYFLOAT));
				change_datadate(sto_num, &sto_date, info, &upinfo, &downinfo, &max, &min, read_way, 1);//maxline);//更改股票的显示日期
				colorkind = 1;
			}
			else if (key == RIGHT)
			{
				delay(50);
				memset(info, 0.0, 150 * sizeof(STODAYFLOAT));
				change_datadate(sto_num, &sto_date, info, &upinfo, &downinfo, &max, &min, read_way, 2);//maxline);//更改股票的显示日期
				colorkind = 1;
			}
			else if (key == ONE)
			{
				read_way = 1;
			}
			else if (key == TWO)
			{
				read_way = 2;
			}
			else if (key == THREE)
			{
				read_way = 3;
			}
			control_key(cinfo);  //键盘控制
			continue;
		}
		else if (!bioskey(1))
		{
			newmouse(&MouseX, &MouseY, &press);
			control_mouse(cinfo,u);
		}
		
		
		if(control_menu(cinfo))
		{
			draw_market();
			control_judge(u);
			Kline_draw(info, max, min);
			MA_draw(info, &upinfo, max, min);
			VOL_draw(info, &downinfo);
			get_markettime(tp, &minute, 1);//重新获得时间
			input_klinequeue(&Q, pos[page]);
			submenu_draw(cinfo->itemnum);
			output_stoname(sto_name);//打印哈希搜索的结果
		}
		
		if(cinfo->func != 4 && cinfo->func != 8)
		{
			clrmous(MouseX,MouseY);
			delay(50);                    //防止留痕
			return; 	 //如果按了其他功能键，就返回，如果是沪深，原函数里便有判断
		}
		
		/********************
		股票队列转换
		*********************/
		if (mouse_press(72, 427, 80, 434) == 1)
		{
			delay(50);
			page = (page + 1) % 25;//若到最后一面，则转到第一面
			setfillstyle(SOLID_FILL, BLACK);
			bar(1, 145, 71, 429);
			setlinestyle(SOLID_LINE, 0, 1);
			setcolor(RED);
			for (i = 0; i < 9; i++)
			{
				line(0, 144 + 36 * i, 72, 144 + 36 * i);
			}
			input_klinequeue(&Q, pos[page]);
			queuedown(&sto, sto_No, 8);//给用于搜索的sto_No赋每一页的值

			setfillstyle(SOLID_FILL, DARKGRAY);
			bar(73, 154, 79, 427);
			setfillstyle(SOLID_FILL, BLACK);
			bar(73, 170 + 10 * page, 79, 180 + 10 * page);//进度条变换
			continue;
		}
		if (mouse_press(72, 147, 80, 154) == 1)
		{
			delay(50);
			page--;
			if (page == -1)
			{
				page = 24;//到最后一面
			}
			/*cleardevice();
			draw_market();*/
			setfillstyle(SOLID_FILL, BLACK);
			bar(1, 145, 71, 429);
			setlinestyle(SOLID_LINE, 0, 1);
			setcolor(RED);
			for (i = 0; i < 9; i++)
			{
				line(0, 144 + 36 * i, 72, 144 + 36 * i);
			}
			input_klinequeue(&Q, pos[page]);
			queueup(&sto, sto_No, 8);//给用于搜索的sto_No赋每一页的值
			

			setfillstyle(SOLID_FILL, DARKGRAY);
			bar(73, 154, 79, 427);
			setfillstyle(SOLID_FILL, BLACK);
			bar(73, 170 + 10 * page, 79, 180 + 10 * page);//进度条变换
			continue;
		}
		/********************
		更改个股
		*********************/
		if (mouse_press(0, 144, 72, 432) == 1)
		{
			memset(info, 0.0, 150 * sizeof(STODAYFLOAT));
			//maxline = change_sto(&sto_num, sto_No, sto_date, info, &upinfo, &downinfo, &max, &min);//数据库末端的行数返回给maxline
			change_sto(&sto_num, sto_No, sto_date, info, &upinfo, &downinfo, &max, &min);//maxline);
			setfillstyle(SOLID_FILL, BLACK);
			bar(1, 121, 72, 143);//遮盖搜索的结果
			search_name(sto_num, sto_name);//股票序号到名称的搜索
			output_stoname(sto_name);//打印哈希搜索的结果
			kind = 0;//主图显示数据改为4个
			colorkind = 1;
		}

		/********************
		更改股票的显示日期
		*********************/
		if (mouse_press(0, 432, 8, 442) == 1)
		{
			delay(50);
			memset(info, 0.0, 150 * sizeof(STODAYFLOAT));
			change_datadate(sto_num, &sto_date, info, &upinfo, &downinfo, &max, &min, read_way, 1);//maxline);//更改股票的显示日期
			colorkind = 1;
		}
		if (mouse_press(64, 432, 72, 442) == 1)
		{
			delay(50);
			memset(info, 0.0, 150 * sizeof(STODAYFLOAT));
			change_datadate(sto_num, &sto_date, info, &upinfo, &downinfo, &max, &min, read_way, 2);//maxline);//更改股票的显示日期
			colorkind = 1;
		}

		if (MouseX > 621 && MouseX < 638 && MouseY> 25 && MouseY < 24 + 39)
		{
			if (mouse_press(621, 25, 638, 24 + 39) == 2)//未点击
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_market(621, 25, 638, 24 + 39, BLUE, 1);
					num = 1;
				}
				continue;
				//标亮函数
			}
			else if (mouse_press(621, 25, 638, 24 + 39) == 1 && cinfo->itemnum == 0)     //在有父菜单的时候不能点击
			{   //买入
				MouseS = 0;
				lightbutton_market(621, 25, 638, 24 + 39, BLUE, 1);
				if(strlen(u->user) == 0)
				{
					cpop(1);
				}
				else if(strlen(u->user) != 0 && strlen(u->bankcard) == 0)
				{
					cpop(2);
				}
				else if(strlen(u->user) != 0 && strlen(u->bankcard) != 0)
					trade(cinfo, u, 0, sto_num);           //跳转至买入第几只股
				if(cinfo->func != 4 && cinfo->func != 8)
				{
					clrmous(MouseX,MouseY);
					delay(10);
					return;            //可能要释放堆栈
				}        //如果跳回来非行情，则跳回
				else 
				{      //成功跳回行情
					cleardevice();
					mainmenu_draw();
					control_judge(u);
					draw_market();
					get_markettime(tp, &minute, 1);	//获得时间
					output_stoname(sto_name);//打印哈希搜索的结果
					Kline_draw(info, max, min);
					MA_draw(info, &upinfo, max, min);
					VOL_draw(info, &downinfo);
					input_klinequeue(&Q, pos[page]);
					submenu_draw(cinfo->itemnum);
					continue;
				}
			}
		}

		if (MouseX > 621 && MouseX < 638 && MouseY> 25 + 40 && MouseY < 24 + 40 + 39)
		{
			if (mouse_press(621, 25 + 40, 638, 24 + 40 + 39) == 2)//未点击
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_market(621, 25 + 40, 638, 24 + 40 + 39, BLUE, 2);
					num = 2;
				}
				continue;

				//标亮函数
			}
			else if (mouse_press(621, 25 + 40, 638, 24 + 40 + 39) == 1 && cinfo->itemnum == 0)
			{ //选股
				MouseS = 0;
				lightbutton_market(621, 25 + 40, 638, 24 + 40 + 39, BLUE, 2);
				cinfo->itemnum = 0;
				cinfo->func = 9;
				continue;
			}
		}

		if (MouseX > 621 && MouseX < 638 && MouseY> 25 + 40 + 49 && MouseY < 24 + 79 + 40)
		{
			if (mouse_press(621, 25 + 40 + 40, 638, 24 + 79 + 40) == 2)//未点击
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_market(621, 25 + 40 + 40, 638, 24 + 79 + 40, BLUE, 3);
					num = 3;
				}
				continue;
				//标亮函数
			}
			else if (mouse_press(621, 25 + 40 + 40, 638, 24 + 79 + 40) == 1 && cinfo->itemnum == 0)
			{ //沪深
				MouseS = 0;
				lightbutton_market(621, 25 + 40 + 40, 638, 24 + 79 + 40, BLUE, 3);
				cinfo->func = 0;
				continue;
			}
		}

		if (MouseX > 621 && MouseX < 638 && MouseY> 24 + 79 + 42 && MouseY < 24 + 79 + 42 + 38)
		{
			if (mouse_press(621, 24 + 79 + 42, 638, 24 + 79 + 42 + 38) == 2)//未点击
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_market(621, 24 + 79 + 42, 638, 24 + 79 + 42 + 38, BLUE, 4);
					num = 4;
				}
				continue;

				//标亮函数
			}
			else if (mouse_press(621, 24 + 79 + 42, 638, 24 + 79 + 42 + 38) == 1 && cinfo->itemnum == 0)
			{ //资产
				MouseS = 0;
				lightbutton_market(621, 24 + 79 + 42, 638, 24 + 79 + 42 + 38, BLUE, 4);
				if(strlen(u->user) == 0)
					cpop(1);
				else if(strlen(u->user) != 0 && strlen(u->bankcard) == 0)
					cpop(2);
				else if(strlen(u->user) != 0 && strlen(u->bankcard) != 0)
					personinfo(cinfo, u);
				if(cinfo->func != 4 && cinfo->func != 8)
				{
					clrmous(MouseX,MouseY);
					delay(100);
					return;            //可能要释放堆栈
				}        //如果跳回来非行情，则跳回
				else 
				{      //成功跳回行情
					cleardevice();
					mainmenu_draw();
					control_judge(u);
					draw_market();
					get_markettime(tp, &minute, 1);	//获得时间
					output_stoname(sto_name);//打印哈希搜索的结果
					Kline_draw(info, max, min);
					MA_draw(info, &upinfo, max, min);
					VOL_draw(info, &downinfo);
					input_klinequeue(&Q, pos[page]);
					submenu_draw(cinfo->itemnum);
					continue;
				}
			}
		}
		/*if (MouseX > 1 && MouseX < 72 && MouseY> 24 && MouseY < 48 && cinfo->itemnum == 0)
		{
			if (mouse_press(1, 24, 72, 48) == 2)//未点击1,24,72,48
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_market(1, 24, 72, 48, BLUE, 5);
					num = 5;
				}
				continue;

				//标亮函数
			}
			else if (mouse_press(1, 24, 72, 48) == 1)
			{
				MouseS = 0;
				lightbutton_market(1, 24, 72, 48, BLUE, 5);
				continue;
			}
		}

		if (MouseX > 1 && MouseX < 72 && MouseY> 48 && MouseY < 72 && cinfo->itemnum == 0)
		{
			if (mouse_press(1, 48, 72, 72) == 2)//未点击1,48,72,72
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_market(1, 48, 72, 72, BLUE, 6);
					num = 6;
				}
				continue;

				//标亮函数
			}
			else if (mouse_press(1, 48, 72, 72) == 1)
			{
				MouseS = 0;
				lightbutton_market(1, 48, 72, 72, BLUE, 6);
				continue;
			}
		}

		if (MouseX > 1 && MouseX < 72 && MouseY> 72 && MouseY < 96 && cinfo->itemnum == 0)
		{
			if (mouse_press(1, 72, 72, 96) == 2)//未点击1,72,72,96
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_market(1, 72, 72, 96, BLUE, 7);
					num = 7;
				}
				continue;

				//标亮函数
			}
			else if (mouse_press(1, 72, 72, 96) == 1)
			{
				MouseS = 0;
				lightbutton_market(1, 72, 72, 96, BLUE, 7);
				continue;
			}
		}

		if (MouseX > 1 && MouseX < 72 && MouseY> 96 && MouseY < 120 && cinfo->itemnum == 0)
		{
			if (mouse_press(1, 96, 72, 120) == 2)//未点击1,96,72,120
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_market(1, 96, 72, 120, BLUE, 8);
					num = 8;
				}
				continue;

				//标亮函数
			}
			else if (mouse_press(1, 96, 72, 120) == 1)
			{
				MouseS = 0;
				lightbutton_market(1, 96, 72, 120, BLUE, 8);
				continue;
			}
		}

		if (MouseX > 1 && MouseX < 72 && MouseY> 120 && MouseY < 144 && cinfo->itemnum == 0)
		{
			if (mouse_press(1, 120, 72, 144) == 2)//未点击1,120,72,144
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_market(1, 120, 72, 144, BLUE, 9);
					num = 9;
				}
				continue;

				//标亮函数
			}
			else if (mouse_press(1, 120, 72, 144) == 1)
			{
				MouseS = 0;
				lightbutton_market(1, 120, 72, 144, BLUE, 9);
				continue;
			}
		}*/

		/********************
		BIAS乖离率
		*********************/
		if (MouseX > 40 && MouseX < 80 && MouseY> 456 && MouseY < 480)
		{
			if (mouse_press(40, 456, 80, 480) == 2)//未点击
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_market(40, 456, 80, 480, BLUE, 10);
					num = 10;
				}
				continue;

				//标亮函数
			}
			else if (mouse_press(40, 456, 80, 480) == 1)
			{
				MouseS = 0;
				lightbutton_market(40, 456, 80, 480, BLUE, 10);
				setfillstyle(SOLID_FILL, BLACK);
				bar(81, 343, 619, 455);
				for (i = 8; i < 11; i++)
				{
					for (j = 0; j < 135; j++)
					{
						putpixel(80 + 4 * j, 72 + 40 * i, RED);
					}
				}
				colorkind = 4;
				viceinfo_show(1, info, &downinfo);
				continue;
			}
		}

		/********************
		BOLL布林线
		*********************/
		else if (MouseX > 80 && MouseX < 120 && MouseY> 456 && MouseY < 480)//BOLL布林线
		{
			if (mouse_press(80, 456, 120, 480) == 2)//未点击
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_market(80, 456, 120, 480, LIGHTMAGENTA, 11);
					num = 11;
				}
				continue;

				//标亮函数
			}
			else if (mouse_press(80, 456, 120, 480) == 1)
			{
				MouseS = 0;
				lightbutton_market(80, 456, 120, 480, BLUE, 11);
				setfillstyle(SOLID_FILL, BLACK);
				bar(81, 73, 619, 341);
				for (i = 0; i < 9; i++)
				{
					for (j = 0; j < 135; j++)
					{
						putpixel(80 + 4 * j, 72 + 40 * i, RED);
					}
				}
				kind = 1;//下一次循环只显示主图的三个数据
				majorinfo_show(2, info, &upinfo, max, min);
				continue;
			}
		}


		/********************
		BRAR人气意愿指标
		*********************/
		else if (MouseX > 120 && MouseX < 160 && MouseY> 456 && MouseY < 480)
		{
			if (mouse_press(120, 456, 160, 480) == 2)//未点击
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_market(120, 456, 160, 480, BLUE, 12);
					num = 12;
				}
				continue;

				//标亮函数
			}
			else if (mouse_press(120, 456, 160, 480) == 1)
			{
				MouseS = 0;
				lightbutton_market(120, 456, 160, 480, BLUE, 12);
				setfillstyle(SOLID_FILL, BLACK);
				bar(81, 343, 619, 455);
				for (i = 8; i < 11; i++)
				{
					for (j = 0; j < 135; j++)
					{
						putpixel(80 + 4 * j, 72 + 40 * i, RED);
					}
				}
				colorkind = 6;
				viceinfo_show(2, info, &downinfo);
				continue;
			}
		}


		/********************
		CR能量指标
		*********************/
		else if (MouseX > 160 && MouseX < 180 && MouseY> 456 && MouseY < 480)
		{
			if (mouse_press(160, 456, 180, 480) == 2)//未点击
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_market(160, 456, 180, 480, BLUE, 13);
					num = 13;
				}
				continue;

				//标亮函数
			}
			else if (mouse_press(160, 456, 180, 480) == 1)
			{
				MouseS = 0;
				lightbutton_market(160, 456, 180, 480, BLUE, 13);
				setfillstyle(SOLID_FILL, BLACK);
				bar(81, 343, 619, 455);
				for (i = 8; i < 11; i++)
				{
					for (j = 0; j < 135; j++)
					{
						putpixel(80 + 4 * j, 72 + 40 * i, RED);
					}
				}
				colorkind = 4;
				viceinfo_show(3, info, &downinfo);
				continue;
			}
		}

		/********************
		DMA平行线差
		*********************/
		else if (MouseX > 180 && MouseX < 210 && MouseY> 456 && MouseY < 480)
		{
			if (mouse_press(180, 456, 210, 480) == 2)//未点击
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_market(180, 456, 210, 480, BLUE, 14);
					num = 14;
				}
				continue;

				//标亮函数
			}
			else if (mouse_press(180, 456, 210, 480) == 1)
			{
				MouseS = 0;
				lightbutton_market(180, 456, 210, 480, BLUE, 14);
				setfillstyle(SOLID_FILL, BLACK);
				bar(81, 343, 619, 455);
				for (i = 8; i < 11; i++)
				{
					for (j = 0; j < 135; j++)
					{
						putpixel(80 + 4 * j, 72 + 40 * i, RED);
					}
				}
				colorkind = 6;
				viceinfo_show(4, info, &downinfo);
				continue;
			}
		}

		/********************
		OBV能量潮
		*********************/
		else if (MouseX > 210 && MouseX < 240 && MouseY> 456 && MouseY < 480)
		{
			if (mouse_press(210, 456, 240, 480) == 2)//未点击
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_market(210, 456, 240, 480, BLUE, 15);
					num = 15;
				}
				continue;

				//标亮函数
			}
			else if (mouse_press(210, 456, 240, 480) == 1)
			{
				MouseS = 0;
				lightbutton_market(210, 456, 240, 480, BLUE, 15);
				setfillstyle(SOLID_FILL, BLACK);
				bar(81, 343, 619, 455);
				for (i = 8; i < 11; i++)
				{
					for (j = 0; j < 135; j++)
					{
						putpixel(80 + 4 * j, 72 + 40 * i, RED);
					}
				}
				colorkind = 5;
				viceinfo_show(5, info, &downinfo);
				continue;
			}
		}

		/********************
		ENE轨道线
		*********************/
		else if (MouseX > 240 && MouseX < 270 && MouseY> 456 && MouseY < 480)//ENE轨道线
		{
			if (mouse_press(240, 456, 270, 480) == 2)//未点击
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_market(240, 456, 270, 480, LIGHTMAGENTA, 16);
					num = 16;
				}
				continue;

				//标亮函数
			}
			else if (mouse_press(240, 456, 270, 480) == 1)
			{
				MouseS = 0;
				lightbutton_market(240, 456, 270, 480, LIGHTMAGENTA, 16);
				setfillstyle(SOLID_FILL, BLACK);
				bar(81, 73, 619, 341);
				for (i = 0; i < 9; i++)
				{
					for (j = 0; j < 135; j++)
					{
						putpixel(80 + 4 * j, 72 + 40 * i, RED);
					}
				}
				kind = 1;
				majorinfo_show(3, info, &upinfo, max, min);
				continue;
			}
		}

		/********************
		随机指标KDJ
		*********************/
		else if (MouseX > 270 && MouseX < 300 && MouseY> 456 && MouseY < 480)//KDJ
		{
			if (mouse_press(270, 456, 300, 480) == 2)//未点击
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_market(270, 456, 300, 480, BLUE, 17);
					num = 17;
				}
				continue;

				//标亮函数
			}
			else if (mouse_press(270, 456, 300, 480) == 1)
			{
				MouseS = 0;
				lightbutton_market(270, 456, 300, 480, BLUE, 17);
				setfillstyle(SOLID_FILL, BLACK);
				bar(81, 343, 619, 455);
				for (i = 8; i < 11; i++)
				{
					for (j = 0; j < 135; j++)
					{
						putpixel(80 + 4 * j, 72 + 40 * i, RED);
					}
				}
				colorkind = 2;
				viceinfo_show(6, info, &downinfo);
				continue;
			}
		}

		/********************
		MACD异同移动平均线
		*********************/
		else if (MouseX > 300 && MouseX < 340 && MouseY> 456 && MouseY < 480)
		{
			if (mouse_press(300, 456, 340, 480) == 2)//未点击
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_market(300, 456, 340, 480, BLUE, 18);
					num = 18;
				}
				continue;

				//标亮函数
			}
			else if (mouse_press(300, 456, 340, 480) == 1)
			{
				MouseS = 0;
				lightbutton_market(300, 456, 340, 480, BLUE, 18);
				setfillstyle(SOLID_FILL, BLACK);
				bar(81, 343, 619, 455);
				for (i = 8; i < 11; i++)
				{
					for (j = 0; j < 135; j++)
					{
						putpixel(80 + 4 * j, 72 + 40 * i, RED);
					}
				}
				colorkind = 3;
				viceinfo_show(7, info, &downinfo);
				continue;
			}
		}



		/********************
		RSI相对强弱指标
		*********************/
		else if (MouseX > 340 && MouseX < 370 && MouseY> 456 && MouseY < 480)
		{
			if (mouse_press(340, 456, 370, 480) == 2)//未点击
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_market(340, 456, 370, 480, BLUE, 19);
					num = 19;
				}
				continue;

				//标亮函数
			}
			else if (mouse_press(340, 456, 370, 480) == 1)
			{
				MouseS = 0;
				lightbutton_market(340, 456, 370, 480, BLUE, 19);
				setfillstyle(SOLID_FILL, BLACK);
				bar(81, 343, 619, 455);
				for (i = 8; i < 11; i++)
				{
					for (j = 0; j < 135; j++)
					{
						putpixel(80 + 4 * j, 72 + 40 * i, RED);
					}
				}
				colorkind = 4;
				viceinfo_show(8, info, &downinfo);
				continue;
			}
		}

		/********************
		VOL成交量
		*********************/
		else if (MouseX > 370 && MouseX < 400 && MouseY> 456 && MouseY < 480)
		{
			if (mouse_press(370, 456, 400, 480) == 2)//未点击
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_market(370, 456, 400, 480, BLUE, 20);
					num = 20;
				}
				continue;

				//标亮函数
			}
			else if (mouse_press(370, 456, 400, 480) == 1)
			{
				MouseS = 0;
				lightbutton_market(370, 456, 400, 480, BLUE, 20);
				setfillstyle(SOLID_FILL, BLACK);
				bar(81, 343, 619, 455);
				for (i = 8; i < 11; i++)
				{
					for (j = 0; j < 135; j++)
					{
						putpixel(80 + 4 * j, 72 + 40 * i, RED);
					}
				}
				kind = 0;
				colorkind = 1;
				viceinfo_show(9, info, &downinfo);
				continue;
			}
		}

		/********************
		1 preday
		*********************/
		if (MouseX > 480 && MouseX < 530 && MouseY> 456 && MouseY < 480)
		{
			if (mouse_press(480, 456, 530, 480) == 2)//未点击
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_market(480, 456, 530, 480, YELLOW, 21);
					num = 21;
				}
				continue;

				//标亮函数
			}
			else if (mouse_press(480, 456, 530, 480) == 1)
			{
				MouseS = 0;
				lightbutton_market(480, 456, 530, 480, LIGHTMAGENTA, 21);
				read_way = 1;
				continue;
			}
		}

		/********************
		30 preday
		*********************/
		else if (MouseX > 530 && MouseX < 590 && MouseY> 456 && MouseY < 480)
		{
			if (mouse_press(530, 456, 590, 480) == 2)//未点击
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_market(530, 456, 590, 480, YELLOW, 22);
					num = 22;
				}
				continue;

				//标亮函数
			}
			else if (mouse_press(530, 456, 590, 480) == 1)
			{
				MouseS = 0;
				lightbutton_market(530, 456, 590, 480, LIGHTMAGENTA, 22);
				read_way = 2;
				continue;
			}
		}

		/********************
		90 preday
		*********************/
		else if (MouseX > 590 && MouseX < 640 && MouseY> 456 && MouseY < 480)
		{
			if (mouse_press(590, 456, 640, 480) == 2)//未点击
			{
				MouseS = 1;
				if (num == 0)//防止被重复标亮
				{
					lightbutton_market(590, 456, 640, 480, YELLOW, 23);
					num = 23;
				}
				continue;

				//标亮函数
			}
			else if (mouse_press(590, 456, 640, 480) == 1)
			{
				MouseS = 0;
				lightbutton_market(590, 456, 640, 480, LIGHTMAGENTA, 23);
				read_way = 3;
				continue;
			}
		}
		
		if (num != 0)
		{
			MouseS = 0;
			recoverbutton_market(num);
			delay(10);
			num = 0;
		}
		
		/********************
		股票实时数据显示
		*********************/
		if (MouseX < 620 && MouseX>80 && MouseY < 455 && MouseY > 100 && cinfo->itemnum == 0)
		{
			MouseS = 3;
			preday = day;
			day = ((620 - MouseX) / 6) - 1;
			if (day < 0)
			{
				day = 0;//防止溢出
			}
			mouse_datashow(info, upinfo, downinfo, &preday, &day, kind, colorkind);//鼠标与数据显示
			continue;
		}


		if (MouseS != 0)
		{
			MouseS = 0;
		}
	}
}

/*********************************************
FUNCTION:put_marketqueue
DESCRIPTION：调用队列构造
INPUT:pos
RETURN:无
***********************************************/
void put_marketqueue(int* pos)
{
	int i;
	for (i = 1; i < 26; i++)
	{
		pos[i] = count_klinequeue(pos[i - 1]);//递归，算每页在文件中开始的位置
	}
}

/*********************************************
FUNCTION:lightbutton_market
DESCRIPTION：行情界面按钮点亮
INPUT:x,y,x1,y1,color,flag
RETURN:无
***********************************************/
void lightbutton_market(int x, int y, int x1, int y1, int color, int flag)
{
	clrmous(MouseX, MouseY);
	//delay(50);
	if (flag == 1 || flag == 2 || flag == 3 || flag == 4)
	{
		setfillstyle(1, color);
		bar(x, y, x1, y1);
		switch (flag)
		{
		case 1:
			puthz(622, 27, "买", 16, 17, RED);
			puthz(622, 27 + 18, "入", 16, 17, RED);
			break;
		case 2:
			puthz(622, 27 + 40, "选", 16, 17, RED);
			puthz(622, 27 + 40 + 18, "股", 16, 17, RED);
			break;
		case 3:
			puthz(622, 27 + 2 * 40, "沪", 16, 17, RED);
			puthz(622, 27 + 2 * 40 + 18, "深", 16, 17, RED);
			break;
		case 4:
			puthz(622, 27 + 3 * 40, "资", 16, 17, RED);
			puthz(622, 27 + 3 * 40 + 18, "产", 16, 17, RED);
			break;
		}
	}
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(2, HORIZ_DIR, 6);
	setcolor(color);
	switch (flag)
	{
	/*case 5:
		puthz(4, 28, "上证指数", 16, 17, RED);
		break;
	case 6:
		puthz(4, 52, "深成指数", 16, 17, RED);
		break;
	case 7:
		puthz(4, 76, "股票财务", 16, 17, RED);
		break;
	case 8:
		puthz(4, 100, "股票走势", 16, 17, RED);
		break;
	case 9:
		puthz(4, 124, "综合排名", 16, 17, RED);
		break;*/
	case 10:
		outtextxy(42, 458, "BIAS");
		break;
	case 11:
		outtextxy(82, 458, "BOLL");
		break;
	case 12:
		outtextxy(122, 458, "BRAR");
		break;
	case 13:
		outtextxy(162, 458, "CR");
		break;
	case 14:
		outtextxy(182, 458, "DMA");
		break;
	case 15:
		outtextxy(212, 458, "OBV");
		break;
	case 16:
		outtextxy(242, 458, "ENE");
		break;
	case 17:
		outtextxy(272, 458, "KDJ");
		break;
	case 18:
		outtextxy(302, 458, "MACD");
		break;
	case 19:
		outtextxy(342, 458, "RSI");
		break;
	case 20:
		outtextxy(372, 458, "VOL");
		break;
	case 21:
		outtextxy(482, 458, "1pre");
		break;
	case 22:
		outtextxy(532, 458, "30pre");
		break;
	case 23:
		outtextxy(590, 458, "90pre");
		break;
	}
}

/*********************************************
FUNCTION:recoverbutton_market
DESCRIPTION：行情界面按钮点亮恢复
INPUT:num
RETURN:无
***********************************************/
void recoverbutton_market(int num)
{
	clrmous(MouseX, MouseY);
	if (num == 1 || num == 2 || num == 3 || num == 4)
	{
		setfillstyle(SOLID_FILL, LIGHTBLUE);
	}
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(2, HORIZ_DIR, 6);
	setcolor(RED);
	switch (num)
	{
	case 1:
		bar(621, 25, 638, 24 + 39);
		puthz(622, 27, "买", 16, 17, WHITE);
		puthz(622, 27 + 18, "入", 16, 17, WHITE);
		break;
	case 2:
		bar(621, 25 + 40, 638, 24 + 40 + 39);
		puthz(622, 27 + 40, "选", 16, 17, WHITE);
		puthz(622, 27 + 40 + 18, "股", 16, 17, WHITE);
		break;
	case 3:
		bar(621, 25 + 40 + 40, 638, 24 + 79 + 40);
		puthz(622, 27 + 2 * 40, "沪", 16, 17, WHITE);
		puthz(622, 27 + 2 * 40 + 18, "深", 16, 17, WHITE);
		break;
	case 4:
		bar(621, 24 + 79 + 42, 638, 24 + 79 + 42 + 38);
		puthz(622, 27 + 3 * 40, "资", 16, 17, WHITE);
		puthz(622, 27 + 3 * 40 + 18, "产", 16, 17, WHITE);
		break;
	case 5:
		puthz(4, 28, "上证指数", 16, 17, WHITE);
		break;
	case 6:
		puthz(4, 52, "深成指数", 16, 17, WHITE);
		break;
	case 7:
		puthz(4, 76, "股票财务", 16, 17, WHITE);
		break;
	case 8:
		puthz(4, 100, "股票走势", 16, 17, WHITE);
		break;
	/*case 9:
		puthz(4, 124, "综合排名", 16, 17, WHITE);
		break;*/
	case 10:
		outtextxy(42, 458, "BIAS");
		break;
	case 11:
		outtextxy(82, 458, "BOLL");
		break;
	case 12:
		outtextxy(122, 458, "BRAR");
		break;
	case 13:
		outtextxy(162, 458, "CR");
		break;
	case 14:
		outtextxy(182, 458, "DMA");
		break;
	case 15:
		outtextxy(212, 458, "OBV");
		break;
	case 16:
		outtextxy(242, 458, "ENE");
		break;
	case 17:
		outtextxy(272, 458, "KDJ");
		break;
	case 18:
		outtextxy(302, 458, "MACD");
		break;
	case 19:
		outtextxy(342, 458, "RSI");
		break;
	case 20:
		outtextxy(372, 458, "VOL");
		break;
	case 21:
		outtextxy(482, 458, "1pre");
		break;
	case 22:
		outtextxy(532, 458, "30pre");
		break;
	case 23:
		outtextxy(590, 458, "90pre");
		break;
	}
}

/*********************************************
FUNCTION:draw_market
DESCRIPTION：行情界面绘制
INPUT:void
RETURN:无
***********************************************/
void draw_market(void)
{
	int i,j;
	setbkcolor(BLACK);
	setcolor(RED);
	setlinestyle(SOLID_LINE, 0, 1);
	line(1, 24, 639, 24);
	line(1, 0, 1, 480);
	line(1, 479, 639, 479);
	line(80, 24, 80, 456);
	line(620, 24, 620, 456);
	line(1, 456, 639, 456);
	line(639, 0, 639, 480);
	line(620, 24 + 40, 639, 24 + 40);
	line(620, 24 + 2 * 40, 639, 24 + 2 * 40);
	line(620, 24 + 3 * 40, 639, 24 + 3 * 40);
	line(620, 24 + 4 * 40, 639, 24 + 4 * 40);
	line(80, 24 + 2 * 24, 620, 24 + 2 * 24);
	line(80, 342, 620, 342);
	for (i = 0; i < 11; i++)
	{
		for (j = 0; j < 135; j++)
		{
			putpixel(80 + 4 * j, 72 + 40 * i, RED);
		}
	}

	//画右侧菜单栏
	setfillstyle(SOLID_FILL, LIGHTBLUE);
	bar(621, 25, 638, 24 + 39);
	bar(621, 25 + 40, 638, 24 + 40 + 39);
	bar(621, 25 + 40 + 40, 638, 24 + 79 + 40);
	bar(621, 24 + 79 + 42, 638, 24 + 79 + 42 + 38);
	puthz(622, 27, "买", 16, 17, WHITE);
	puthz(622, 27 + 18, "入", 16, 17, WHITE);
	puthz(622, 27 + 40, "选", 16, 17, WHITE);
	puthz(622, 27 + 40 + 18, "股", 16, 17, WHITE);
	puthz(622, 27 + 2 * 40, "沪", 16, 17, WHITE);
	puthz(622, 27 + 2 * 40 + 18, "深", 16, 17, WHITE);
	puthz(622, 27 + 3 * 40, "资", 16, 17, WHITE);
	puthz(622, 27 + 3 * 40 + 18, "产", 16, 17, WHITE);

	//画左侧股票栏
	for (i = 0; i < 9; i++)
	{
		line(0, 144 + 36 * i, 72, 144 + 36 * i);
	}
	
	//画股票队列拖动条和时间选择拖动条
	setfillstyle(SOLID_FILL, DARKGRAY);
	bar(73, 145, 79, 455);
	line(72, 144, 72, 456);
	line(72, 144, 80, 144);
	line(0, 144 + 36 * 8 + 10, 72, 144 + 36 * 8 + 10);
	bar(1, 144 + 36 * 8 + 1, 71, 144 + 36 * 8 + 9);
	setfillstyle(SOLID_FILL, BLACK);
	bar(73, 170, 79, 180);

	setcolor(BLACK);
	line(73, 154, 79, 154);
	line(79, 154, 76, 147);
	line(73, 154, 76, 147);
	line(73, 427, 76, 434);
	line(76, 434, 79, 427);
	line(73, 427, 79, 427);
	setfillstyle(SOLID_FILL, BLACK);
	floodfill(76, 150, BLACK);
	floodfill(76, 430, BLACK);//画股票队列点击按钮
	line(1, 437, 8, 440);
	line(1, 437, 8, 434);
	line(8, 434, 8, 440);
	line(64, 434, 71, 437);
	line(64, 440, 71, 437);
	line(64, 434, 64, 440);
	floodfill(4, 437, BLACK);
	floodfill(68, 437, BLACK);//画股票时间切换点击按钮

	//画底测技术指标
	setcolor(RED);
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(2, HORIZ_DIR, 6);
	line(40, 456, 40, 479);
	puthz(4, 460, "指标", 16, 17, RED);

	line(80, 456, 80, 479);
	outtextxy(42, 458, "BIAS");//40,456,80,480

	line(120, 456, 120, 479);
	outtextxy(82, 458, "BOLL");//80,456,120,480

	line(160, 456, 160, 479);
	outtextxy(122, 458, "BRAR");//120,456,160,480

	line(180, 456, 180, 479);
	outtextxy(162, 458, "CR");//160,456,180,480

	line(210, 456, 210, 479);
	outtextxy(182, 458, "DMA");//180,456,210,480

	line(240, 456, 240, 479);
	outtextxy(212, 458, "OBV");//210,456,240,480

	line(270, 456, 270, 479);
	outtextxy(242, 458, "ENE");//240,456,270,480

	line(300, 456, 300, 479);
	outtextxy(272, 458, "KDJ");//270,456,300,480

	line(340, 456, 340, 479);
	outtextxy(302, 458, "MACD");//300,456,320,480

	line(370, 456, 370, 479);
	outtextxy(342, 458, "RSI");

	line(400, 456, 400, 479);
	outtextxy(372, 458, "VOL");


	line(480, 456, 480, 479);
	puthz(408, 460, "天数切换", 16, 17, RED);

	line(525, 456, 525, 479);
	outtextxy(482, 458, "1pre");
	line(585, 456, 585, 479);
	outtextxy(532, 458, "30pre");
	outtextxy(590, 458, "90pre");


	//画左侧菜单栏
	/*line(1, 48, 72, 48);
	line(72, 48, 80, 40);
	puthz(4, 28, "上证指数", 16, 17, WHITE);

	line(1, 72, 72, 72);
	line(72, 72, 80, 64);
	puthz(4, 52, "深成指数", 16, 17, WHITE);

	line(1, 96, 72, 96);
	line(72, 96, 80, 88);
	puthz(4, 76, "股票财务", 16, 17, WHITE);

	line(1, 120, 72, 120);
	line(72, 120, 80, 112);
	puthz(4, 100, "股票走势", 16, 17, WHITE);

	line(1, 144, 72, 144);
	line(72, 144, 80, 136);
	puthz(4, 124, "综合排名", 16, 17, WHITE);*/

	puthz(3, 88, "股票：", 24, 25, LIGHTCYAN);

	//画顶端信息栏
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(0, HORIZ_DIR, 1);
	puthz(82, 26, "时间", 16, 17, YELLOW);
	setcolor(YELLOW);
	outtextxy(80 + 17 * 2, 32, ":");

	puthz(190, 26, "今开", 16, 17, WHITE);
	setcolor(WHITE);
	outtextxy(188 + 17 * 2, 32, ":");

	puthz(298, 26, "最高", 16, 17, RED);
	setcolor(RED);
	outtextxy(296 + 17 * 2, 32, ":");

	puthz(406, 26, "最低", 16, 17, GREEN);
	setcolor(GREEN);
	outtextxy(404 + 17 * 2, 32, ":");

	puthz(514, 26, "今收", 16, 17, WHITE);
	setcolor(WHITE);
	outtextxy(516 + 17 * 2, 32, ":");

	puthz(82, 50, "涨跌", 16, 17, LIGHTRED);
	setcolor(LIGHTRED);
	outtextxy(80 + 17 * 2, 56, ":");

	puthz(190, 50, "涨幅", 16, 17, RED);
	setcolor(RED);
	outtextxy(188 + 17 * 2, 56, ":");

	puthz(298, 50, "振幅", 16, 17, YELLOW);
	setcolor(YELLOW);
	outtextxy(296 + 17 * 2, 56, ":");

	puthz(406, 50, "成交量", 16, 17, LIGHTBLUE);
	setcolor(LIGHTBLUE);
	outtextxy(404 + 17 * 3, 56, ":");

	puthz(514, 50, "成交额", 16, 17, LIGHTCYAN);
	setcolor(CYAN);
	outtextxy(512 + 17 * 3, 56, ":");

}