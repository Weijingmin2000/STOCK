#include<io.h>
#include<direct.h>
#include"trdbuy.h"
#include"public.h"
#include"trade.h"
#include"traverse.h"
#include"storead.h"

/*********************************************
trdbuy.c
COPYRIGHT: dengshumin
FUNCTION:  stock buy
ABSTRACT:  
		A. buy stock
		B. update hold and history
		C. button lightning and recovering
DATE: 2019/10/23
**********************************************/

/********************************************************
FUNCTION: buy
DESCRIPTION: 用户买入界面控制与实现
ATTENTION:   stk_locat实现了从行情买入的定位操作
INPUT: trmenu_ord,trmenu_ord_last,cinfo,u,stk_locat
RETURN: 无
********************************************************/
void buy(short int *trmenu_ord,short int *trmenu_ord_last,Ctr* cinfo,USER* u,int stk_locat)
{
	struct tm *tp = NULL;
	time_t t;
	short int num = 0;                  // 点亮按钮编号
	short int input_flag = 0;
	char STK_CODE[7] = {'\0'};        	//6位数的证券代码
	char STK_NAME[10] = {'\0'};       	//股票名称
	char STK_PRICE[7] = {'\0'};       	//带小数点最多6位的买入/卖出价格
	char STK_NUM[10] = {'\0'};        	//最多9位，股票数量一般以100一手
	char STK_AFFORD[10] = {'\0'};     	//可买（股），最多9位
	char STK_SUM[20] = {'\0'};        	//总金额
	char STK_TIME[11] = {'\0'};         //成交日期
	char STK_TEMP[10] = {'\0'};         //临时数组
	char STK_PRICE_MAR[7] = {'\0'};     //股票市价
	float stk_price = 0;                //股票价格（浮点数）
	float stk_balance = 0;    		    //余额
	float stk_price_mar = 0;            //股票市价（浮点数）
	double stk_sum = 0;                 //股票总价
	unsigned long stk_num = 0;          //股票数量
	unsigned long stk_afford = 0;       //可买股
	int minute = 0;
	int stk_second = 0;
	memset(cinfo,0,sizeof(Ctr));	    //将控制信息初始化
	cinfo->func = 3;          	   	    //用3标记交易函数的界面
	mainmenu_draw();
	control_judge(u);
	buy_draw();
	get_time(tp, &minute, 1);           //获取时间              
	get_balance1(u, &stk_balance);      //每次调用一次buy,从数据库中获取余额,并显示
	read_balance(stk_balance);          //读出余额
	if(stk_locat != 0)
	{                  					//如果传入的股票位置不为0
		get_buy(stk_locat,STK_CODE, STK_NAME, STK_PRICE);             //将其获取
		settextjustify(LEFT_TEXT, TOP_TEXT);
		settextstyle(2, HORIZ_DIR, 6);
		setcolor(BLACK);
		outtextxy(230, 73, STK_CODE);
		if(STK_NAME[0] >= '!' && STK_NAME[0] <= 'z')
		{
			setcolor(WHITE);
			settextjustify(LEFT_TEXT, TOP_TEXT);
			settextstyle(2, HORIZ_DIR, 6);
			outtextxy(232, 106, "*ST");
			puthz(261,108,STK_NAME + 3, 16, 17, WHITE);
		}
		else
		{
			puthz(232,108,STK_NAME,16,17,WHITE);	
		}
		if(strlen(STK_PRICE) != 0)
		{
			memset(STK_PRICE_MAR,'\0',sizeof(STK_PRICE_MAR));
			setcolor(BLACK);
			settextjustify(LEFT_TEXT, TOP_TEXT);
			settextstyle(2, HORIZ_DIR, 6);
			outtextxy(230,137,STK_PRICE);
			turn_a2f(STK_PRICE,&stk_price);     	//将输入的读出
			strcpy(STK_PRICE_MAR, STK_PRICE);       //接收市价
			stk_price_mar = stk_price;              //市价等于最初读出价格
			puthz(420,140,"当前市价",16,17,LIGHTRED);
			setcolor(WHITE);
			outtextxy(500,137,STK_PRICE_MAR);             //显示当前市价
			stk_afford = (unsigned long)(stk_balance / stk_price);	 //计算可买股
			if(stk_afford >= 100)
			{
				stk_hundred(stk_afford,STK_AFFORD);     
				setcolor(BLACK);
				settextjustify(LEFT_TEXT, TOP_TEXT);
				settextstyle(2, HORIZ_DIR, 6);
				outtextxy(230,169,STK_AFFORD);          //计算出后即输出
			}
		}
	}
	stk_second = tp->tm_sec;
	while(1)
	{
		if(strlen(u->user) == 0)
			cinfo->func = 0;
		get_time(tp, &minute, 0);
		if(strlen(STK_NAME) != 0)              //在有股票显示的情况下          
		{	
			if(tp->tm_sec < stk_second)          //增加一分钟
			{
				if((tp->tm_sec + 60 - stk_second) >= 8)         //隔8秒市价变动一次 
				{
					get_new_price(STK_PRICE_MAR, &stk_price_mar, 1);
					stk_second = tp->tm_sec;
				}
			}
			else 
			{
				if((tp->tm_sec - stk_second) >= 8)
				{
					get_new_price(STK_PRICE_MAR, &stk_price_mar, 1);
					stk_second = tp->tm_sec;
				}
			}
		}
		read_balance(stk_balance);
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
		if(control_menu(cinfo))
		{
			mainmenu_draw();
			control_judge(u);
			trade_menu_draw(1);
			buy_draw();
			get_time(tp, &minute, 1); 
			read_balance(stk_balance);
			submenu_draw(cinfo->itemnum);
			continue;
		}
		if(cinfo->func != 3 && cinfo->func != 13)
		{
			clrmous(MouseX,MouseY);
			delay(10);                 					    //防止留痕
			return; 										//如果按了其他功能键，就返回，如果是沪深，原函数里便有判断
		}
		if(change_trdmenu(trmenu_ord,trmenu_ord_last))      //如果切换菜单
			return;
		if(MouseX > 222 && MouseX < 381 && MouseY > 74 && MouseY < 94)
		{
			if(mouse_press(222,74,381,94) == 2)
			{
				MouseS = 2;
				continue;
			}
			else if(mouse_press(222,74,381,94) == 1)
			{ 	//输入框也是一个重置按钮
				input_flag = 0;
				clrmous(MouseX,MouseY);
				setfillstyle(1,WHITE);
				MouseS = 0;
				bar(222,74,381,94);            //证券代码
				bar(222,138,361,158);          //买入价格
				bar(222,170,381,190);		   //可买股
				bar(222,234,361,254);          //将其他的重置
				setfillstyle(1,BLACK);         //遮盖
				bar(222,106,381,126);          //证券名称
				bar(420, 138, 630, 158);       //当前市价
				bar(420,170,630,190);          //买入价格低于买五 
				//setfillstyle(1,WHITE);
				stk_num = 0;
				stk_price = 0;
				stk_afford = 0;
				stk_price_mar = 0;
				memset(STK_CODE,'\0',sizeof(STK_CODE));
				memset(STK_NAME,'\0',sizeof(STK_NAME));
				memset(STK_PRICE,'\0',sizeof(STK_PRICE));
				memset(STK_AFFORD,'\0',sizeof(STK_AFFORD));
				memset(STK_NUM,'\0',sizeof(STK_NUM));
				memset(STK_PRICE_MAR,'\0',sizeof(STK_PRICE_MAR));
				memset(STK_SUM,'\0',sizeof(STK_SUM));
				input_trade(STK_CODE,222,74,6,WHITE);
				if(strlen(STK_CODE) != 0)
				{
					if(strlen(STK_CODE) == 6)
					{
						if(get_stkname(STK_CODE,STK_NAME,STK_PRICE) != 0)    //遍历查找算法,并将位置返回
						{         //查找到	
							if(STK_NAME[0] >= '!' && STK_NAME[0] <= 'z')
							{
								setcolor(WHITE);
								settextjustify(LEFT_TEXT, TOP_TEXT);
								settextstyle(2, HORIZ_DIR, 6);
								outtextxy(232, 106, "*ST");
								puthz(261,108,STK_NAME + 3, 16, 17, WHITE);
							}
							else
							{
								puthz(232,108,STK_NAME,16,17,WHITE);	
							}
						}	   
					}
				}
				if(strlen(STK_PRICE) != 0)
				{
					setcolor(BLACK);
					settextjustify(LEFT_TEXT, TOP_TEXT);
					settextstyle(2, HORIZ_DIR, 6);
					outtextxy(230,137,STK_PRICE);
					turn_a2f(STK_PRICE, &stk_price);     	//将输入的读出
					stk_afford = (unsigned long)(stk_balance / stk_price);	 //计算可买股
					if(stk_afford >= 100)
					{
						stk_hundred(stk_afford,STK_AFFORD);     
						setcolor(BLACK);
						settextjustify(LEFT_TEXT, TOP_TEXT);
						settextstyle(2, HORIZ_DIR, 6);
						outtextxy(230,169,STK_AFFORD);          //计算出后即输出
					}
					stk_price_mar = stk_price;                	//数据库价格等于市价
					memset(STK_PRICE_MAR,'\0',sizeof(STK_PRICE_MAR));
					strcpy(STK_PRICE_MAR,STK_PRICE);
					setfillstyle(1,BLACK);
					bar(420, 138, 540, 158);
					puthz(420,140,"当前市价",16,17,LIGHTRED);
					settextjustify(LEFT_TEXT,TOP_TEXT);
					settextstyle(2,0,6);
					setcolor(WHITE);
					outtextxy(500,137,STK_PRICE_MAR);             //显示当前市价
				}
				continue;					
			}
		}

		if(MouseX > 222 && MouseX < 362 && MouseY > 138 && MouseY < 158)
		{    //价格框
			if(mouse_press(222,138,362,158) == 2)
			{
				MouseS = 2;
				continue;
			}
			else if(mouse_press(222,138,362,158) == 1)
			{
				setfillstyle(1,WHITE);
				MouseS = 0;
				memset(STK_PRICE,'\0',sizeof(STK_PRICE));
				stk_price = 0;            //置0
				clrmous(MouseX,MouseY);
				bar(222,138,361,158);
				input_trade(STK_PRICE,222,138,6,WHITE);
				if(strlen(STK_PRICE) != 0)
				{       //价格变动
					turn_a2f(STK_PRICE,&stk_price);       	//价格转换为浮点数
					stk_afford =(unsigned long)(stk_balance / stk_price);	//余额除以价格即为可买股
					stk_hundred(stk_afford,STK_AFFORD);     //转字符
					setfillstyle(1,WHITE);					//可买股输出
					bar(222,170,381,190);                   //覆盖可买股
					setfillstyle(1,BLACK);
					bar(420,170,630,190);                   //遮盖买入价格低于买五？
					setcolor(BLACK);
					settextjustify(LEFT_TEXT, TOP_TEXT);
					settextstyle(2, HORIZ_DIR, 6);
					outtextxy(230,169,STK_AFFORD);        		//计算出后即输出
				}   //正常
				continue;
			}	
		}
		if(MouseX > 362 && MouseX < 381 && MouseY > 138 && MouseY < 158)
		{
			if(mouse_press(362,138,381,148) == 1 )       //点击价格增加按钮
			{
				stk_price = 0;
				delay(150);
				turn_a2f(STK_PRICE,&stk_price);     	//将输入的读出
				stk_price += 0.01;                 		//读出后+0.01
				float_to_str(STK_PRICE,stk_price,2);    //将浮点数读入   
				setfillstyle(1,WHITE);
				bar(222,138,361,158);
				setfillstyle(1,BLACK);
				bar(420,170,630,190);                   //遮盖买入时的市价信息
				setcolor(BLACK);
				settextjustify(LEFT_TEXT, TOP_TEXT);
				settextstyle(2, HORIZ_DIR, 6);
				outtextxy(230,137,STK_PRICE);
				continue;                           
			}
			else if(mouse_press(362,148,381,158) == 1 )
			{
				delay(150);
				stk_price = 0;
				turn_a2f(STK_PRICE,&stk_price);
				if(stk_price >= 0.01)
					stk_price = stk_price - 0.01;
				else
					stk_price = 0;
				float_to_str(STK_PRICE,stk_price,2);	 //将浮点数读入
				setfillstyle(1,WHITE);
				bar(222,138,361,158);
				setfillstyle(1,BLACK);
				bar(420,170,630,190);                   //遮盖买入时的市价信息
				setcolor(BLACK);
				settextjustify(LEFT_TEXT, TOP_TEXT);
				settextstyle(2, HORIZ_DIR, 6);
				outtextxy(230,137,STK_PRICE);
				continue;
			}
		}
		
		if(MouseX > 222 && MouseX < 381 && MouseY > 170 && MouseY < 190)
		{
			if(mouse_press(222,170,381,190) == 2)
			{
				if(num == 0 )
				{
					lightbutton_trade_buy(1);    //点亮可买股
					num = 1;
				}
				continue;         //点亮
			}
			else if(mouse_press(222,170,381,190) == 1) //点击可买股，更新可买股数
			{
				new_afford(&stk_afford, stk_balance, stk_price, STK_AFFORD);
				continue;
			}
		}
		if(MouseX > 225 && MouseX < 385 && MouseY > 195 && MouseY < 225)
		{
			if(mouse_press(227,200,262,221) == 2 )
			{
				if(num == 0 )
				{
					lightbutton_trade_buy(2);
					num = 2;
				}
				continue;
			}
			else if(mouse_press(227,200,262,221) == 1)     //点击1/2框
			{
				clrmous(MouseX,MouseY);
				delay(150);
				setfillstyle(1,WHITE);					
				bar(222,234,362,254);
				if(stk_judge(stk_afford, 2) != 0);  		 //以可买股数1/2来输出在买入数量上，若不足一百股则不输出
				{
					stk_num = (stk_afford / 100) * 100 / 2;
					stk_hundred(stk_num, STK_NUM);    //以整百形式输出
					stk_turn_a2i(STK_NUM, &stk_num);
					setcolor(BLACK);
					settextjustify(LEFT_TEXT, TOP_TEXT);
					settextstyle(2, HORIZ_DIR, 6);
					outtextxy(230,233,STK_NUM);
					stk_judgesum(stk_num,stk_afford,stk_price,STK_SUM,STK_NUM,&stk_sum); 
				}
				continue;
			}
			if(mouse_press(267,200,302,221) == 2)
			{
				if(num == 0 )
				{
					lightbutton_trade_buy(3);
					num = 3;
				}
				continue;
			}
			else if(mouse_press(267,200,302,221) == 1)
			{
				clrmous(MouseX,MouseY);
				delay(150);
				setfillstyle(1,WHITE);					
				bar(222,234,362,254);
				if(stk_judge(stk_afford, 3) != 0);   //以可买股数1/3来输出在买入数量上，若不足一百股则不输出
				{
					stk_num = (stk_afford / 100) * 100 / 3;
					stk_hundred(stk_num,STK_NUM);
					stk_turn_a2i(STK_NUM,&stk_num);
					setcolor(BLACK);
					settextjustify(LEFT_TEXT, TOP_TEXT);
					settextstyle(2, HORIZ_DIR, 6);
					outtextxy(230,233,STK_NUM);
					stk_judgesum(stk_num,stk_afford,stk_price,STK_SUM,STK_NUM,&stk_sum);
				}
				continue;
			}
			if(mouse_press(307,200,342,221) == 2)
			{
				if(num == 0 )
				{
					lightbutton_trade_buy(4);
					num = 4;
				}
				continue;
			}
			else if(mouse_press(307,200,342,221) == 1)
			{
				clrmous(MouseX,MouseY);
				delay(150);
				setfillstyle(1,WHITE);					
				bar(222,234,362,254);
				if(stk_judge(stk_afford, 4) != 0);   //以可买股数1/4来输出在买入数量上，若不足一百股则不输出
				{
					stk_num = (stk_afford / 100) * 100 / 4;
					stk_hundred(stk_num,STK_NUM);
					stk_turn_a2i(STK_NUM,&stk_num);
					setcolor(BLACK);
					settextjustify(LEFT_TEXT, TOP_TEXT);
					settextstyle(2, HORIZ_DIR, 6);
					outtextxy(230,233,STK_NUM);
					stk_judgesum(stk_num,stk_afford,stk_price,STK_SUM,STK_NUM,&stk_sum);
				}
				continue;
			}
			if(mouse_press(347,200,382,221) == 2)
			{
				if(num == 0 )
				{
					lightbutton_trade_buy(5);
					num = 5;
				}
				continue;
			}
			else if(mouse_press(347,200,382,221) == 1)
			{
				clrmous(MouseX,MouseY);
				delay(150);
				setfillstyle(1,WHITE);					
				bar(222,234,362,254);
				if(stk_judge(stk_afford, 5) != 0);   //以可买股数1/5来输出在买入数量上，若不足一百股则不输出
				{
					stk_num = (stk_afford / 100) * 100 / 5;
					stk_hundred(stk_num,STK_NUM);
					stk_turn_a2i(STK_NUM,&stk_num);     
					setcolor(BLACK);
					settextjustify(LEFT_TEXT, TOP_TEXT);
					settextstyle(2, HORIZ_DIR, 6);
					outtextxy(230,233,STK_NUM);
					stk_judgesum(stk_num,stk_afford,stk_price,STK_SUM,STK_NUM,&stk_sum);
				}   
				continue;
			}
		}
		if(MouseX > 222 && MouseX < 362 && MouseY > 234 && MouseY <254)
		{
			if(mouse_press(222,234,362,254) == 2)
			{
				MouseS = 2;
				continue;
			}
			else if(mouse_press(222,234,362,254) == 1)
			{
				setfillstyle(1,WHITE);
				MouseS = 0;
				clrmous(MouseX,MouseY);
				memset(STK_NUM,'\0',sizeof(STK_NUM));          
				stk_num = 0;           //置零，stk_num的值和屏幕一样
				bar(222,234,361,254);
				input_trade(STK_NUM,222,234,6,WHITE);
				if(strlen(STK_NUM) != 0)
				{
					stk_turn_a2i(STK_NUM,&stk_num);
					stk_judgesum(stk_num,stk_afford,stk_price,STK_SUM,STK_NUM,&stk_sum);
				}
				continue;
			}
		}
		if(MouseX > 362 && MouseX < 381 && MouseY > 234 && MouseY < 254)
		{
			if(mouse_press(362,234,381,244) == 1)   //买入数量加一手
			{
				delay(150);
				//stk_num = 0;
				stk_turn_a2i(STK_NUM,&stk_num);   //输出
				stk_num += 100;
				memset(STK_NUM,'\0',sizeof(STK_NUM));
				ultoa(stk_num,STK_NUM,10);
				setfillstyle(1,WHITE);
				bar(222,234,361,254);
				setcolor(BLACK);
				settextjustify(LEFT_TEXT, TOP_TEXT);
				settextstyle(2, HORIZ_DIR, 6);
				outtextxy(230,233,STK_NUM);
				stk_judgesum(stk_num,stk_afford,stk_price,STK_SUM,STK_NUM,&stk_sum);
				continue;
			}
			else if(mouse_press(362,244,381,254) == 1)
			{
				delay(150);
				stk_turn_a2i(STK_NUM,&stk_num);   //输出
				if(stk_num >= 100)
					stk_num -= 100;
				else 
					stk_num = 0;
				memset(STK_NUM,'\0',sizeof(STK_NUM));
				ultoa(stk_num,STK_NUM,10);         
				setfillstyle(1,WHITE);
				bar(222,234,361,254);
				setcolor(BLACK);
				settextjustify(LEFT_TEXT, TOP_TEXT);
				settextstyle(2, HORIZ_DIR, 6);
				outtextxy(230,233,STK_NUM);     //每次改变的值都输出在屏幕上
				stk_judgesum(stk_num,stk_afford,stk_price,STK_SUM,STK_NUM,&stk_sum);
				continue;
			}
			continue;
		}
		if(MouseX > 163 && MouseX < 235 && MouseY > 274 && MouseY < 304)
		{
			if(mouse_press(163,274,235,304) == 2)
			{
				MouseS = 1;
				if(num == 0 )
				{
					lightbutton_trade_buy(6);
					num = 6;
				}
				continue;
			}
			else if(mouse_press(163,274,235,304) == 1)
			{ 	//点击重置
				delay(150);
				MouseS = 0;
				clrmous(MouseX,MouseY);
				setfillstyle(1,BLACK);
				bar(121,25,640,456);         //全部重置
				buy_draw();
				num = 0;
				input_flag = 0;
				memset(STK_CODE,'\0',sizeof(STK_CODE));
				memset(STK_NAME,'\0',sizeof(STK_NAME));
				memset(STK_PRICE,'\0',sizeof(STK_PRICE));
				memset(STK_PRICE_MAR,'\0',sizeof(STK_PRICE_MAR));
				memset(STK_AFFORD,'\0',sizeof(STK_AFFORD));
				memset(STK_NUM,'\0',sizeof(STK_NUM));
				memset(STK_SUM,'\0',sizeof(STK_SUM));
				stk_num = 0;
				stk_price = 0;
				stk_afford = 0;
				stk_price_mar = 0;
				stk_sum = 0;
				continue;
			}
		}		
		
		if(MouseX > 289 && MouseX < 361 && MouseY > 274 && MouseY < 304)
		{
			if(mouse_press(289,274,361,304) == 2)
			{
				MouseS = 1;
				if(num == 0 )
				{
					lightbutton_trade_buy(7);
					num = 7;
				}
				continue;
			}
			else if(mouse_press(289,274,361,304) == 1)   //买入按钮
			{
				MouseS = 0;
				delay(150);
				setfillstyle(1,BLACK);
				bar(420,277,550,297);      //买入成功？
				if(stk_judgebuy(STK_CODE,STK_NAME,STK_NUM,STK_PRICE,STK_SUM,STK_PRICE_MAR) && stk_judgesum(stk_num,stk_afford,stk_price,STK_SUM,STK_NUM,&stk_sum) && (stk_num != 0))
				{	//全输入、判断总价、市价
					t = time(NULL);
					tp = localtime(&t);
					memset(STK_TIME,'\0',sizeof(STK_TIME));
					memset(STK_TEMP,'\0',sizeof(STK_TEMP));
					ultoa(1900 + tp->tm_year,STK_TEMP,10);
					strcat(STK_TIME,STK_TEMP);
					strcat(STK_TIME,"/");
					memset(STK_TEMP,'\0',sizeof(STK_TEMP));
					ultoa(1 + tp->tm_mon,STK_TEMP,10);
					strcat(STK_TIME,STK_TEMP);
					strcat(STK_TIME,"/");
					memset(STK_TEMP,'\0',sizeof(STK_TEMP));
					ultoa(tp->tm_mday,STK_TEMP,10);
					strcat(STK_TIME,STK_TEMP);
					//读取成交日期
					stk_balance = stk_balance - stk_sum;
					input_balance(u,stk_balance);
					get_time(tp, &minute, 1);
					read_balance(stk_balance);  //成交之后重新读取
				//	printf("%d",tp->tm_sec);
					stk_input_hst(u,STK_TIME,STK_CODE,STK_NAME,STK_NUM,STK_PRICE,STK_SUM,1);
					stk_input_hold(u,STK_TIME,STK_CODE,STK_NAME,STK_NUM,STK_PRICE,0);
					delay(300);
					puthz(420, 277, "买入成功！", 16, 17, YELLOW);
					new_afford(&stk_afford, stk_balance, stk_price, STK_AFFORD);   //重新计算可买股
				}  //成交日期，证券代码，证券名称，操作，成交数量，成交均价，成交金额
				continue;      //买入的诸多判定,以及输入数据库
			} //买入之后有BC崩的现象
		}
		if(num != 0)
		{
			recoverbutton_trade_buy(num);
			num = 0;
		}
		if(strlen(STK_CODE) == 6)
		{
			if(input_flag == 0)
			{
				clrmous(MouseX,MouseY);       //去痕
				settextstyle(2,0,6);
				setfillstyle(1,LIGHTBLUE);
				bar(222,74,381,94);
				setcolor(WHITE);
				settextjustify(LEFT_TEXT, TOP_TEXT);
				settextstyle(2, HORIZ_DIR, 6);
				outtextxy(230,73,STK_CODE);
				input_flag = 1;                   	 //作为标亮代码框记号防止重复标亮
			}
			continue;
		}
		if(input_flag != 0)
		{
			input_flag = 0;
		}
	}
}

/***************************************************
FUNCTION: lightbutton_trade_buy
DESCRIPTION: 买入界面点亮
INPUT: num
RETURN: 无
***************************************************/
void lightbutton_trade_buy(short int num)
{
	delay(10);
	clrmous(MouseX,MouseY);
	setcolor(LIGHTGRAY);
	setlinestyle(0,0,1);
	if(num == 1)
	{
		setlinestyle(0,0,3);
		setcolor(RED);
		rectangle(222,170,381,190);
	}
	else if(num == 2)
	{
		line(262,200,262,220);
		line(227,221,262,221);
	}
	else if(num == 3)
	{
		line(302,200,302,220);
		line(267,221,302,221);
	}
	else if(num == 4)
	{
		line(342,200,342,220);
		line(307,221,342,221);
	}
	else if(num == 5)
	{
		line(382,200,382,220);
		line(347,221,382,221);
	}
	else if(num == 6)
	{
		setfillstyle(1,LIGHTBLUE);
		bar(163,274,235,304);                 //y+20
		puthz(171,277,"重置",24,32,BLUE);
		setcolor(WHITE);
		rectangle(163,274,235,304);
	}
	else if(num == 7)
	{
		setfillstyle(1,LIGHTRED);
		bar(289,274,361,304);
		puthz(297,277,"买入",24,32,RED);
		setcolor(WHITE);
		rectangle(289,274,361,304);
	}
}

/*******************************************************
FUNCTION: recoverbutton_trade_buy
DESCRIPTION: 买入界面的恢复标亮
INPUT: num
RETURN: 无 
********************************************************/

void recoverbutton_trade_buy(short int num)
{
	delay(10);
	clrmous(MouseX, MouseY);
	setcolor(BLACK);
	setlinestyle(0,0,1);
	if(num == 1)
	{
		setlinestyle(0,0,3);
		setcolor(WHITE);
		rectangle(222,170,381,190);
	}
	else if(num == 2)
	{
		line(262,200,262,220);
		line(227,221,262,221);
	}
	else if(num == 3)
	{
		line(302,200,302,220);
		line(267,221,302,221);
	}
	else if(num == 4)
	{
		line(342,200,342,220);
		line(307,221,342,221);
	}
	else if(num == 5)
	{
		line(382,200,382,220);
		line(347,221,382,221);
	}
	else if(num == 6)
	{
		setfillstyle(1,LIGHTBLUE);
		bar(163,274,235,304);                 //y+20
		puthz(171,277,"重置",24,32,WHITE);
	}
	else if(num == 7)
	{
		setfillstyle(1,LIGHTRED);
		bar(289,274,361,304);
		puthz(297,277,"买入",24,32,WHITE); 
	}
}

/*******************************************************************
FUNCTION: stk_judgesum
DESCRIPTION: 判断买入数量与可买数量的关系并给与提示
INPUT: stk_num,stk_afford,stk_price,STK_SUM,STK_NUM,stk_sum
return: 若买入数量大于可买股返回1，否则返回0
*******************************************************************/
int stk_judgesum(unsigned long stk_num,unsigned long stk_afford,float stk_price,char* STK_SUM,char* STK_NUM, double* stk_sum)     //买入数量 可买股 总金额
{
	if(stk_num > stk_afford)
	{
		setfillstyle(1,LIGHTRED);
		bar(121,334,361,354);
		puthz(129,336,"注意：买入数量大于可买数量",16,17,WHITE);
		return 0;
	}
	else 
	{
		memset(STK_SUM,'\0',sizeof(STK_SUM));
		setfillstyle(1,BLACK);
		bar(121,334,361,354);
		setfillstyle(1,LIGHTRED);
		bar(151,334,361,354);
		setcolor(WHITE);
		settextjustify(RIGHT_TEXT,TOP_TEXT);
		settextstyle(2, HORIZ_DIR, 6);
		outtextxy(214,334,STK_NUM);
		puthz(214,336,"股",16,17,WHITE);
		puthz(240,336,"金额",16,17,YELLOW);
		settextjustify(LEFT_TEXT,TOP_TEXT);
		setcolor(YELLOW);
		outtextxy(279,334,":");
		*stk_sum = stk_num*stk_price;
		float_to_str(STK_SUM,*stk_sum,2);
		settextjustify(RIGHT_TEXT,TOP_TEXT);
		outtextxy(361,334,STK_SUM);
		return 1;
	}
}

/************************************************************************
FUNCTION: stk_judgebuy
DESCRIPTION: 判断总价、市价
INPUT: stk_code,stk_name,stk_num,stk_price,stk_sum,stk_price_mar
RETURN: 若满足要求则返回1，否则返回0
************************************************************************/
int stk_judgebuy(char* stk_code,char* stk_name,char* stk_num,char* stk_price,char* stk_sum,char* stk_price_mar)
{
	float price = 0;
	float price_mar = 0;
	if((strlen(stk_code) != 0) && (strlen(stk_name) != 0) && (strlen(stk_num) != 0) && (strlen(stk_price) != 0) && (strlen(stk_sum) != 0) && (strlen(stk_price_mar) != 0))
	{
		turn_a2f(stk_price,&price);
		turn_a2f(stk_price_mar,&price_mar);
		if(price <= price_mar)
		{
			if((price_mar - price) > 0.05)
			{
				setfillstyle(1,BLACK);
				bar(420,170,630,190);
				puthz(420,172,"买入价格低于买五",16,17,YELLOW);
				return 0;
			}
			else
			{
				setfillstyle(1,BLACK);
				bar(420,170,630,190);
				return 1;
			}
		}
		else
		{
			setfillstyle(1,BLACK);
			bar(420,170,630,190);
			puthz(420,172,"买入价格高于市价",16,17,YELLOW);
			return 1;
		}
	}
	else 
		return 0;
}

/**********************************************************************
FUNCTION: stk_input_hst
DESCRIPTION: 将买入信息存入历史成交
INPUT: u,stk_time,stk_code,stk_name,stk_num,stk_price,stk_sum,i
RETURN: 无
**********************************************************************/
void stk_input_hst(USER* u,char* stk_time,char* stk_code,char* stk_name,char* stk_num,char* stk_price,char* stk_sum,int i)
{	 //最后一个是操作参数
	FILE* fp;
	Hstdata *hst_buy;       				 //历史成交型
	char STK_ADR[50] = {'\0'};    		     //地址段
	if((hst_buy = (Hstdata*)malloc(sizeof(Hstdata))) == NULL)
	{
		printf("memoryallocation runs wrong");
		delay(3000);
		exit(1);
	}                                        //调用堆区
	memset(hst_buy,'\0',sizeof(Hstdata));    //初始化
	strcpy(STK_ADR,"database\\USER\\");      //遇到'\0'结束
	strcat(STK_ADR,u->user);
	//可创建文件夹
	if(access(STK_ADR, 0) == -1)             //不存在文件夹
	{
		if(mkdir(STK_ADR) != 0)              //未成功创建
		{
			printf("The file opens failued");
		}
	}//若存在则直接打开
	strcat(STK_ADR,"\\");
	strcat(STK_ADR,"history.dat");
	if((fp=fopen(STK_ADR,"ab+"))==NULL)
	{
		printf("can't find the file history.dat while init the data of stock!\n");
		delay(1000);
		exit(1);
	}
	fseek(fp,0,SEEK_END);      //定位到最后
	strcpy(hst_buy->sto_date,stk_time);
	strcpy(hst_buy->sto_code,stk_code);
	strcpy(hst_buy->sto_name,stk_name);
	strcpy(hst_buy->sto_num,stk_num);
	strcpy(hst_buy->sto_price,stk_price);
	strcpy(hst_buy->sto_amount,stk_sum);
	if(i == 1)
	{
		hst_buy->sto_flag[0] = '1';
	}
	else if(i == 2)
	{
		hst_buy->sto_flag[0] = '2';
	}
	hst_buy->sto_date[11] = '\t';
	hst_buy->sto_code[6] = '\t';
	hst_buy->sto_name[9] = '\t';
	hst_buy->sto_flag[1] = '\t';
	hst_buy->sto_num[9] = '\t';
	hst_buy->sto_price[6] = '\t';
	hst_buy->sto_amount[19] = '\n';    //处理格式
	fwrite(hst_buy,sizeof(Hstdata),1,fp);
	if (fclose(fp) != 0)
	{
		printf("\n cannot close Database");
		exit(1);
	}
	if(hst_buy != NULL)
	{
		free(hst_buy);
		hst_buy = NULL;
	}
	return;
}

/*********************************************************************
FUNCTION: stk_input_hold
DESCRIPTION: 将交易后信息更改存入持仓
ATTENTION: 参数i为0代表买入，为1时代表卖出
INPUT:  u,stk_time,stk_code,stk_name,stk_num,stk_price,i
RETURN: 无
*********************************************************************/
void stk_input_hold(USER* u, char* stk_time,char* stk_code, char* stk_name,char* stk_num,char* stk_price,int i)      //持仓
{	//成交日期，证券代码，证券名称，成交数量，成交均价
	FILE* fp;
	Hold* hold;       				         //持仓型
	int l = 0;                               //文件位置
	int day = 0;                             //时间
	int prime[3] = {0};                      //最初的持仓时间
	int now[3] = {0};                        //现在的持仓时间
	unsigned long num = 0;               	 //持股数
	unsigned long num_prime = 0;             //历史持股数
	float price = 0;                         //成本价格
	float price_prime = 0;                   //股票均价
	double sum = 0;               			 //股票总价
	double sum_prime = 0;                    //持仓金额
	char STK_ADR[50] = {'\0'};    		     //地址段
	if((hold = (Hold*)malloc(sizeof(Hold))) == NULL)
	{
		printf("memoryallocation runs wrong");
		delay(3000);
		exit(1);
	}                                        //堆区调用，及时释放
	memset(hold,'\0',sizeof(Hold));  		 //初始化
	strcpy(STK_ADR,"database\\USER\\");      //遇到'\0'结束
	strcat(STK_ADR,u->user);
	//可创建文件夹
	if(access(STK_ADR, 0) == -1)             //不存在文件夹
	{
		if(mkdir(STK_ADR) != 0)              //未成功创建
		{
			printf("The file opens failued");
		}
	}//若存在则直接打开
	strcat(STK_ADR,"\\");
	strcat(STK_ADR,"hold.dat");
	if(access(STK_ADR, 0) == -1)              //如果不存在持仓文件
	{
		if((fp = fopen(STK_ADR,"ab+"))==NULL)
		{
			printf("can't find the file hold.dat while init the data of stock!\n");
			delay(1000);
			exit(1);
		}
	}
	else      								  //若存在，则以rb形式
	{
		if((fp = fopen(STK_ADR, "rb+")) == NULL)
		{
			printf("can't find the file hold.dat while init the data of stock!\n");
			delay(1000);
			exit(1);
		}
	}
	
	if(i == 0)
	{
		l = get_hold_stk(hold, stk_name, STK_ADR);   //由证券名称获取持仓股票
		if(l == 0)
		{
			memset(hold,'\0',sizeof(Hold));
			strcpy(hold->sto_date,stk_time);
			strcpy(hold->sto_code,stk_code);
			strcpy(hold->sto_name,stk_name);
			strcpy(hold->sto_num,stk_num);
			strcpy(hold->sto_price,stk_price);
			hold->sto_day[0] = '0';
			hold->sto_date[10] = '\t';
			hold->sto_code[6] = '\t';  
			hold->sto_name[9] = '\t';
			hold->sto_num[9] = '\t';
			hold->sto_price[6] = '\t';
			hold->sto_day[3] = '\n';
			fseek(fp, 0, SEEK_END);
			fwrite(hold, sizeof(Hold), 1, fp);
			if(hold != NULL)
			{
				free(hold);
				hold = NULL;
			}
			if(fclose(fp) != 0)
			{
				printf("\n cannot close Database");
				exit(1);
			}
			return;
		}
		else 												//若搜索到，则会在函数里覆盖
		{
			itoa(day, hold->sto_day, 10);
			stk_turn_a2i(stk_num, &num);      				//成交数量
			turn_a2f(stk_price,&price);                     //成交价格
			sum = price * num;                              //总金额
		
			stk_turn_a2i(hold->sto_num, &num_prime);        //持股数
			turn_a2f(hold->sto_price,&price_prime);         //原成本价格
			sum_prime = price_prime * num_prime;            //原价格总金
	
			sum = sum + sum_prime;                          //总金额
			num = num + num_prime;                          //买入的总数量
			price = sum / num;                              //新的成本总价
		
			memset(hold->sto_num, '\0', sizeof(hold->sto_num));
			ultoa(num, hold->sto_num, 10);                  //存入hold中
			float_to_str(hold->sto_price, price, 2);        //price存入
			
			turn_a2i(hold->sto_date, prime, prime + 1, prime + 2);
			turn_a2i(stk_time, now, now + 1, now + 2);
			day = get_hold_day(prime, now);
			memset(hold->sto_day, '\0', sizeof(hold->sto_day));
			itoa(day, hold->sto_day, 10);                   //持仓天数
		
			hold->sto_date[10] = '\t';
			hold->sto_code[6] = '\t';  
			hold->sto_name[9] = '\t';
			hold->sto_num[9] = '\t';
			hold->sto_price[6] = '\t';
			hold->sto_day[3] = '\n';
		
			l = l - 1;   
			fseek(fp, l * sizeof(Hold), SEEK_SET);
			fwrite(hold, sizeof(Hold), 1, fp);              //写入持仓
			if(hold != NULL)
			{
				free(hold);
				hold = NULL;
			}
			if(fclose(fp) != 0)
			{
				printf("\n cannot close Database");
				exit(1);
			}
			return ;                                       //写入并释放然后返回
		}
	}
	else if(i == 1)
	{
		l = get_hold_stk(hold, stk_name, STK_ADR);  	   //位置
		stk_turn_a2i(hold->sto_num, &num_prime);           //持仓股票
		stk_turn_a2i(stk_num, &num);                       //成交数量
		//printf("%s",stk_num);
	//	printf("\n%ld",num);
	//	printf("\n%ld",num_prime);
		num = num_prime - num;                     		   //卖出后的数量
		
		
		memset(hold->sto_num, '\0', sizeof(hold->sto_num));
		if(num == 0)
		{  //买完后数量为0
			memset(hold, '\0', sizeof(Hold));  			   //用NULl，应该得往前移覆盖掉？
		}
		else
		{   //不为0
			ultoa(num, hold->sto_num, 10);                 //存入hold中
			turn_a2i(hold->sto_date, prime, prime + 1, prime + 2);
			turn_a2i(stk_time, now, now + 1, now + 2);
			day = get_hold_day(prime, now);
			memset(hold->sto_day, '\0', sizeof(hold->sto_day));
			itoa(day, hold->sto_day, 10);                  //持仓天数
		}
		
		hold->sto_date[10] = '\t';
		hold->sto_code[6] = '\t';  
		hold->sto_name[9] = '\t';
		hold->sto_num[9] = '\t';
		hold->sto_price[6] = '\t';
		hold->sto_day[3] = '\n';
		l = l - 1;   
		
		fseek(fp, l * sizeof(Hold), SEEK_SET);
		fwrite(hold, sizeof(Hold), 1, fp);              //写入持仓
		if(hold != NULL)
		{
			free(hold);
			hold = NULL;
		}
		if(fclose(fp) != 0)
		{
			printf("\n cannot close Database");
			exit(1);
		}
		return ;                                       //写入并释放然后返回
	}
	if(hold != NULL)
	{
		free(hold);
		hold = NULL;
	}
	if(fclose(fp) != 0)
	{
		printf("\n cannot close Database");
		exit(1);
	}
}

/*************************************************
FUNCTION: get_new_price
DESCRIPTION: 更新市价
INPUT: STK_PRICE,stk_price_mar,color
RETURN: 无
**************************************************/
void get_new_price(char* STK_PRICE_MAR, float* stk_price_mar,int color)                     //获取新的市价
{
	int i = 0, j = 0;
	srand(time(NULL));
	j = rand() % 2;
	//printf("%f",*stk_price_mar);
	if(j == 0)
	{ 
		i = rand() % 4;         //0,1,2,3
		if(i == 1)
			*stk_price_mar = *stk_price_mar + 0.01;
		else if(i == 2)
			*stk_price_mar = *stk_price_mar + 0.02;
		else if(i == 3)
			*stk_price_mar = *stk_price_mar + 0.03;
	}
	else if(j == 1)
	{
		i = rand() % 4;         //0,1,2,3
		if(i == 1)
			*stk_price_mar = *stk_price_mar - 0.01;
		else if(i == 2)
			*stk_price_mar = *stk_price_mar - 0.02;
		else if(i == 3)
			*stk_price_mar = *stk_price_mar - 0.03;
	}
	//printf("%f",*stk_price_mar);
	memset(STK_PRICE_MAR,'\0',sizeof(STK_PRICE_MAR));
	float_to_str(STK_PRICE_MAR, *stk_price_mar, 2);
	setfillstyle(1,BLACK);
	bar(420, 138, 630, 158);
	if(color == 1)
	{
		puthz(420,140,"当前市价",16,17,LIGHTRED);
	}
	else if(color == 2)
	{
		puthz(420,140,"当前市价",16,17,LIGHTCYAN);
	}
	settextjustify(LEFT_TEXT,TOP_TEXT);
	settextstyle(2,0,6);
	setcolor(WHITE);
	outtextxy(500,137,STK_PRICE_MAR);             //显示当前市价
}

/*********************************************************
FUNCTION: new_afford
DESCRIPTION: 更新可买股数
INPUT: stk_afford,stk_balance,stk_price,STK_AFFORD
RETURN: 无
*********************************************************/

void new_afford(unsigned long* stk_afford,float stk_balance, float stk_price, char* STK_AFFORD)
{
	clrmous(MouseX,MouseY);
	*stk_afford = 0;
	setfillstyle(1,WHITE);					
	bar(222,170,381,190);                    //归零以及覆盖
	setfillstyle(1,BLACK);
	bar(420,170,630,190);                   //遮盖买入时的市价信息
	*stk_afford = (unsigned long)(stk_balance / stk_price);	
	if(*stk_afford >= 100)
	{
		stk_hundred(*stk_afford,STK_AFFORD);			//根据输入的买入价格以及自己的余额计算可买股数(可做成函数)
		setcolor(BLACK);
		settextjustify(LEFT_TEXT, TOP_TEXT);
		settextstyle(2, HORIZ_DIR, 6);
		outtextxy(230,169,STK_AFFORD);          //计算出后即输出
	}
}