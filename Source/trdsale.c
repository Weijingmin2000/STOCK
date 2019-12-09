#include"trdsale.h"
#include"public.h"
#include"traverse.h"
#include"storead.h"
#include"trade.h"
#include"trdbuy.h"
#include"queue.h"

/******************************************************
trdsale.c
COPYRIGHT: dengshumin
FUNCTION:  stock sale
ABSTRACT:
		A.sale stock
		B.update hold and history
		C. button lightning and recovering
DATE: 2019/10/27
******************************************************/

/************************************************
FUNCTION: sale
DESCRIPTION: 用户卖出界面控制与实现
INPUT: trmenu_ord,trmenu_ord_last,cinfo,u
RETURN: 无
************************************************/
void sale(short int *trmenu_ord,short int *trmenu_ord_last,Ctr* cinfo,USER* u)
{
	struct tm *tp = NULL;
	time_t t;
	short int num = 0;                 // 点亮按钮编号
	short int input_flag = 0;          //证券代码输入判断
	char STK_CODE[7] = {'\0'};         //6位数的证券代码
	char STK_NAME[10] = {'\0'};        //股票名称
	char STK_PRICE[7] = {'\0'};        //带小数点最多6位的买入/卖出价格
	char STK_PRICE_MAR[7] = {'\0'};    //市价
	char STK_NUM[10] = {'\0'};         //最多9位，股票数量一般以100一手
	char STK_SUM[20] = {'\0'};         //总金额
	char STK_TIME[11] = {'\0'};        //成交日期
	char STK_AFFORD[10] = {'\0'};      //可用余额（卖出）
	char STK_TEMP[10] = {'\0'};        
	float stk_price = 0;               //股票价格（浮点数）
	float stk_price_mar = 0;           //市价
	float stk_balance = 0;             //余额
	double stk_sum = 0;
	unsigned long stk_num = 0;         //股票数量
	unsigned long stk_afford = 0;      //可用余额
	int minute = 0;
	int stk_second = 0;
	memset(cinfo,0,sizeof(Ctr));	    //将控制信息初始化
	cinfo->func = 3;          	   	    //用3标记交易函数的界面
	sale_draw();
	mainmenu_draw();
	control_judge(u);
	get_time(tp, &minute, 1);           //获取时间              
	get_balance1(u, &stk_balance);      //每次调用一次sale,从数据库中获取余额,并显示
	read_balance(stk_balance);          //读取余额
	stk_second = tp->tm_sec;
	while(1)
	{
		if(strlen(u->user) == 0)
			cinfo->func = 0;
		get_time(tp, &minute, 0);
		if(strlen(STK_NAME) != 0)
		{	
			if(tp->tm_sec < stk_second)          //增加一分钟
			{
				if((tp->tm_sec + 60 - stk_second) >= 8)         //隔8秒市价变动一次 
				{
					get_new_price(STK_PRICE_MAR, &stk_price_mar, 2);
					stk_second = tp->tm_sec;
				}
			}
			else 
			{
				if((tp->tm_sec - stk_second) >= 8)
				{
					get_new_price(STK_PRICE_MAR, &stk_price_mar, 2);
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
			trade_menu_draw(2);
			mainmenu_draw();
			control_judge(u);
			sale_draw();
			get_time(tp, &minute, 1);
			read_balance(stk_balance);
			submenu_draw(cinfo->itemnum);
			continue;
		}
		if(cinfo->func != 3 && cinfo->func != 13)
		{
			clrmous(MouseX,MouseY);
			delay(10);                    //防止留痕
			return; 	 //如果按了其他功能键，就返回，如果是沪深，原函数里便有判断
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
			{
				input_flag = 0;
				clrmous(MouseX,MouseY);
				setfillstyle(1,WHITE);
				MouseS = 0;
				bar(222,74,381,94);            //证券代码
				bar(222,138,361,158);          //卖出价格
				bar(222,170,381,190);		   //可用余额
				bar(222,234,361,254);          //卖出数量
				setfillstyle(1,BLACK);
				bar(222,106,381,126);          //证券名称
				bar(420, 138, 630, 158);       //当前市价
				bar(420,170,630,190);          //卖出价格高于卖五
				stk_num = 0;
				stk_price = 0;
				stk_afford = 0;
				stk_price_mar = 0;
				stk_sum = 0;
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
						if(get_stkname(STK_CODE,STK_NAME,STK_PRICE) != 0)
						{
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
					outtextxy(230,137,STK_PRICE);         //价格
					turn_a2f(STK_PRICE, &stk_price);
					if(get_sale_hold(u, STK_NAME, &stk_afford) != 0)
					{
						ultoa(stk_afford, STK_AFFORD, 10);
						if(stk_afford >= 100)
						{
							stk_hundred(stk_afford, STK_AFFORD);
							setcolor(BLACK);
							settextjustify(LEFT_TEXT,TOP_TEXT);
							settextstyle(2,0,6);
							outtextxy(230, 169, STK_AFFORD);
						}  //不足100则不输出
					}
					stk_price_mar = stk_price;
					memset(STK_PRICE_MAR,'\0',sizeof(STK_PRICE_MAR));
					strcpy(STK_PRICE_MAR,STK_PRICE);
					setfillstyle(1,BLACK);
					bar(420, 138, 540, 158);
					puthz(420,140,"当前市价",16,17,LIGHTCYAN);
					settextjustify(LEFT_TEXT,TOP_TEXT);
					settextstyle(2,0,6);
					setcolor(WHITE);
					outtextxy(500,137,STK_PRICE_MAR);             //显示当前市价
				}
				continue;
			}	
		}
		
		if(MouseX > 222 && MouseX < 362 && MouseY > 138 && MouseY < 158)          //卖出价格
		{
			if(mouse_press(222,138,362,158) == 2)
			{
				MouseS = 2;
				continue;
			}
			else if(mouse_press(222,138,362,158) == 1)
			{	 //点击价格框
				setfillstyle(1,WHITE);
				clrmous(MouseX,MouseY);
				MouseS = 0;
				memset(STK_PRICE,'\0',sizeof(STK_PRICE));
				stk_price = 0;      					  //点击过价格框后赋0
				bar(222,138,361,158);
				input_trade(STK_PRICE,222,138,6,WHITE);
				if(strlen(STK_PRICE) != 0)
				{   //价格有变
					turn_a2f(STK_PRICE,&stk_price);       //价格转换为浮点数;
					setfillstyle(1,BLACK);
					bar(420,170,630,190);                 //遮盖卖出价格高于卖五
				}
				continue;
			}	
		}
		
		if(MouseX > 362 && MouseX < 381 && MouseY > 138 && MouseY < 158)   //增量
		{
			if(mouse_press(362,138,381,148) == 1 )       //点击价格增加按钮
			{
				delay(150);
				stk_price = 0;
				turn_a2f(STK_PRICE,&stk_price);    		 //将输入的读出
				stk_price += 0.01;                 		 //读出后+0.01
				float_to_str(STK_PRICE,stk_price,2);     //将浮点数读入   
				setfillstyle(1,WHITE);
				bar(222,138,361,158);
				setfillstyle(1,BLACK);                   //卖出价格高于卖五
				bar(420,170,630,190); 
				setcolor(BLACK);
				settextjustify(LEFT_TEXT, TOP_TEXT);
				settextstyle(2, HORIZ_DIR, 6);
				outtextxy(230,137,STK_PRICE);
				continue;                           
			}
			else if(mouse_press(362,148,381,158) == 1 )  //点击价格降低
			{  
				delay(150);
				stk_price = 0;
				turn_a2f(STK_PRICE,&stk_price);
				if(stk_price >= 0.01)
					stk_price = stk_price - 0.01;
				else
					stk_price = 0;
				float_to_str(STK_PRICE,stk_price,2);  //将浮点数读入
				setfillstyle(1,WHITE);
				bar(222,138,361,158);
				setfillstyle(1,BLACK);
				bar(420,170,630,190); 
				setcolor(BLACK);
				settextjustify(LEFT_TEXT, TOP_TEXT);
				settextstyle(2, HORIZ_DIR, 6);
				outtextxy(230,137,STK_PRICE);
				continue;
			}
		}
		
		if(MouseX > 222 && MouseX < 381 && MouseY > 170 && MouseY <190)          //可用余额
		{
			if(mouse_press(222,170,381,190) == 2)
			{
				if(num == 0)
				{
					lightbutton_trade_sale(1);    //点亮可用余额
					num = 1;
				}
				continue;         //点亮
			}
			else if(mouse_press(222,170,381,190) == 1) //点击可用余额，更新可用余额
			{
				
				delay(100);
				new_saleafford(u, &stk_afford, STK_NAME, STK_AFFORD);
				continue;
			}
		}				//更新可用余额还得注意卖出时的逻辑
		
		if(MouseX > 225 && MouseX < 385 && MouseY > 195 && MouseY < 225)
		{
			if(mouse_press(227,200,262,221) == 2)
			{
				if(num == 0)
				{
					lightbutton_trade_sale(2);
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
				if(stk_judge(stk_afford, 2) != 0)  	 //以卖出余额1/2来输出在卖出数量上，若不足一百股则不输出
				{
					stk_num = (stk_afford / 100) * 100 / 2;
					stk_hundred(stk_num, STK_NUM);   	 //以整百形式输出
					stk_turn_a2i(STK_NUM, &stk_num);
					setcolor(BLACK);
					settextjustify(LEFT_TEXT, TOP_TEXT);
					settextstyle(2, HORIZ_DIR, 6);
					outtextxy(230,233,STK_NUM);
					stk_judgesum2(stk_num,stk_afford,stk_price,STK_SUM,STK_NUM,&stk_sum); 
				}
				continue;
			}
			if(mouse_press(267,200,302,221) == 2)
			{
				if(num == 0 )
				{
					lightbutton_trade_sale(3);
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
				if(stk_judge(stk_afford, 3) != 0)  
				{
					stk_num = (stk_afford / 100) * 100 / 3;
					stk_hundred(stk_num,STK_NUM);
					stk_turn_a2i(STK_NUM,&stk_num);
					setcolor(BLACK);
					settextjustify(LEFT_TEXT, TOP_TEXT);
					settextstyle(2, HORIZ_DIR, 6);
					outtextxy(230,233,STK_NUM);
					stk_judgesum2(stk_num,stk_afford,stk_price,STK_SUM,STK_NUM,&stk_sum);
				}
				continue;
			}
			if(mouse_press(307,200,342,221) == 2)
			{
				if(num == 0 )
				{
					lightbutton_trade_sale(4);
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
				if(stk_judge(stk_afford, 4) != 0)
				{					
					stk_num = (stk_afford / 100) * 100 / 4;
					stk_hundred(stk_num,STK_NUM);
					stk_turn_a2i(STK_NUM,&stk_num);
					setcolor(BLACK);
					settextjustify(LEFT_TEXT, TOP_TEXT);
					settextstyle(2, HORIZ_DIR, 6);
					outtextxy(230,233,STK_NUM);
					stk_judgesum2(stk_num,stk_afford,stk_price,STK_SUM,STK_NUM,&stk_sum);
				}
				continue;
			}
			
			if(mouse_press(347,200,382,221) == 2)
			{
				if(num == 0)
				{
					lightbutton_trade_sale(5);
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
				if(stk_judge(stk_afford, 5) != 0)  
				{
					stk_num = (stk_afford / 100) * 100 / 5;
					stk_hundred(stk_num,STK_NUM);
					stk_turn_a2i(STK_NUM,&stk_num);     
					setcolor(BLACK);
					settextjustify(LEFT_TEXT, TOP_TEXT);
					settextstyle(2, HORIZ_DIR, 6);
					outtextxy(230,233,STK_NUM);
					stk_judgesum2(stk_num,stk_afford,stk_price,STK_SUM,STK_NUM,&stk_sum);
				}   
				continue;
			}
		}
		
		if(MouseX > 222 && MouseX < 362 && MouseY > 234 && MouseY < 254)        //卖出数量
		{
			if(mouse_press(222,234,362,254) == 2)
			{
				MouseS = 2;
				continue;
			}
			else if(mouse_press(222,234,362,254) == 1)
			{
				setfillstyle(1,WHITE);
				clrmous(MouseX,MouseY);
				MouseS = 0;
				memset(STK_NUM,'\0',sizeof(STK_NUM));        
				stk_num = 0;
				bar(222,234,361,254);
				input_trade(STK_NUM,222,234,6,WHITE);
				if(strlen(STK_NUM) != 0)
				{
					stk_turn_a2i(STK_NUM,&stk_num);          //将卖出数量赋给stk_num
					stk_judgesum2(stk_num,stk_afford,stk_price,STK_SUM,STK_NUM,&stk_sum);
				}
				continue;
			}
		}
		
		if(MouseX > 362 && MouseX < 381 && MouseY > 234 && MouseY < 254)
		{
			if(mouse_press(362,234,381,244) == 1)   //卖出数量增加一手
			{
				delay(150);
				stk_turn_a2i(STK_NUM,&stk_num);     //输出卖出数量
				stk_num += 100;
				memset(STK_NUM,'\0',sizeof(STK_NUM));
				ultoa(stk_num,STK_NUM,10);
				setfillstyle(1,WHITE);
				bar(222,234,361,254);
				setcolor(BLACK);
				settextjustify(LEFT_TEXT, TOP_TEXT);
				settextstyle(2, HORIZ_DIR, 6);
				outtextxy(230,233,STK_NUM);
				stk_judgesum2(stk_num,stk_afford,stk_price,STK_SUM,STK_NUM,&stk_sum);
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
				stk_judgesum2(stk_num,stk_afford,stk_price,STK_SUM,STK_NUM,&stk_sum);
				continue;
			}	
		}
		
		
		if(MouseX > 163 && MouseX < 235 && MouseY > 274 && MouseY < 304)     //重置
		{
			if(mouse_press(163,274,235,304) == 2)
			{
				MouseS = 1;
				if(num == 0 )
				{
					lightbutton_trade_sale(6);
					num = 6;
				}
				continue;
			}
			else if(mouse_press(163,274,235,304) == 1)
			{   //点击重置
				delay(150);
				MouseS = 0;
				clrmous(MouseX,MouseY);
				setfillstyle(1,BLACK);
				bar(121,25,640,456);
				sale_draw();
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
					lightbutton_trade_sale(7);
					num = 7;
				}
				continue;
			}
			else if(mouse_press(289,274,361,304) == 1)   //卖出按钮
			{
				MouseS = 0;
				delay(150);
				clrmous(MouseX,MouseY);
				setfillstyle(1,BLACK);
				bar(420,277,550,297);
				if(stk_judgesale(STK_CODE,STK_NAME,STK_NUM,STK_SUM,STK_PRICE,STK_PRICE_MAR) && stk_judgesum2(stk_num,stk_afford,stk_price,STK_SUM,STK_NUM,&stk_sum) && (stk_afford != 0))
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
					stk_balance = stk_balance + stk_sum;
					input_balance(u,stk_balance);
					get_time(tp, &minute, 1);
					read_balance(stk_balance);  //成交之后重新读取
					stk_input_hst(u,STK_TIME,STK_CODE,STK_NAME,STK_NUM,STK_PRICE,STK_SUM,2);
					stk_input_hold(u,STK_TIME,STK_CODE,STK_NAME,STK_NUM,STK_PRICE,1);
					delay(300);
					puthz(420, 277, "卖出成功！", 16, 17, LIGHTCYAN);
					new_saleafford(u, &stk_afford, STK_NAME, STK_AFFORD);
				}
				continue;     //卖出的诸多判定,以及输入数据库
			}
		}
		if(num != 0) 
		{
			recoverbutton_trade_sale(num);
			num = 0;
		}
		if(strlen(STK_CODE) == 6)
		{
			if(input_flag == 0 )
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
FUNCTION: lightbutton_trade_sale
DESCRIPTION: 卖出界面点亮
INPUT: num
RETURN: 无
***************************************************/
void lightbutton_trade_sale(short int num)
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
		setfillstyle(1,LIGHTRED);
		bar(163,274,235,304);                 //y+20
		puthz(171,277,"重置",24,32,RED);
		setcolor(WHITE);
		rectangle(163,274,235,304);
	}
	else if(num == 7)
	{
		setfillstyle(1,LIGHTBLUE);
		bar(289,274,361,304);
		puthz(297,277,"卖出",24,32,BLUE);
		setcolor(WHITE);
		rectangle(289,274,361,304);
	}
}

/*******************************************************
FUNCTION: recoverbutton_trade_sale
DESCRIPTION: 卖出界面的恢复标亮
INPUT: num
RETURN: 无 
********************************************************/
void recoverbutton_trade_sale(short int num)
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
		setfillstyle(1,LIGHTRED);
		bar(163,274,235,304);                 //y+20
		puthz(171,277,"重置",24,32,WHITE);
	}
	else if(num == 7)
	{
		setfillstyle(1,LIGHTBLUE);
		bar(289,274,361,304);
		puthz(297,277,"卖出",24,32,WHITE); 
	}
}

/***********************************************
FUNCTION: get_sale_hold
DESCRIPTION: 根据证券名称获取该股可用余额
INPUT: u,stk_name,stk_afford
RETURN: 若获取到则返回位置+1,否则返回0
************************************************/
int get_sale_hold(USER* u, char* stk_name, unsigned long* stk_afford)
{   
	FILE* fp;
	Hold hold;
	int len = 0;
	int i = 0, j = 0;
	int l = 0, k = 0;
	int m = 0;
	int item = 0;
	char ch = '0';
	char STK_ADR[50] = {'\0'};
	memset(&hold, '\0', sizeof(Hold));
	strcpy(STK_ADR,"database\\USER\\");      //遇到'\0'结束
	strcat(STK_ADR,u->user);
	strcat(STK_ADR,"\\");
	strcat(STK_ADR,"hold.dat");
	if((fp = fopen(STK_ADR, "rb")) == NULL)
	{
		puthz(420,170,"不存在持仓股票",16,17,LIGHTBLUE);
		return; //如果不存在持仓直接返回，无事发生
	}
	fseek(fp, 0, SEEK_END);
	len = ftell(fp) / sizeof(Hold);  //计算持股股数
	for(i = 0;i < len; i++)
	{
		memset(&hold,'\0',sizeof(Hold));
		l = countline(fp,k);     //l为一行的长度+1('\n')
		for(j = 0,ch = '0',item = 1,m = 0; j < l; j++)
		{
			fseek(fp, j * sizeof(char) + k, SEEK_SET);
			fread(&ch, sizeof(char), 1, fp);
			if(ch != '\t' && ch != '\n' && ch != '\r' && ch != ' ')
			{
				if(item == 3)
					hold.sto_name[m] = ch;
				else if(item == 4)
					hold.sto_num[m] = ch;
			}
			m++;
			if(ch == '\t')
			{
				m = 0;
				item++;
			}
			else if ((ch == '\n' || ch == '\r' || i == len) && item == 12)
			{
				break;        //如果读到回车换行
			}
		}
		k = l + k;
		if(strcmp(hold.sto_name, stk_name) == 0)
		{    //若找到持仓
			stk_turn_a2i(hold.sto_num, stk_afford);
			if(fclose(fp) != 0)
			{
				delay(1000);
				printf("cannot close hold.dat");
				exit(1);
			}
			return i+1;   
		}
	}
	if(fclose(fp) != 0)
	{
		delay(1000);
		printf("cannot close hold.dat");
		exit(1);
	}
	return 0;
}

/***************************************************************
FUNCTION: stk_judgesum2
DESCRIPTION: 判断卖出数量与可卖数量的关系
INPUT: stk_num,stk_afford,stk_price,STK_SUM,STK_NUM,stk_sum
RETURN: 若满足需求则返回1，否则返回0
***************************************************************/
int stk_judgesum2(unsigned long stk_num,unsigned long stk_afford,float stk_price,char* STK_SUM,char* STK_NUM, double* stk_sum)
{
	if(stk_num > stk_afford)
	{
		setfillstyle(1,LIGHTBLUE);
		bar(121,334,361,354);
		puthz(129,336,"注意：卖出数量大于可卖数量",16,17,WHITE);
		return 0;
	}
	else 
	{
		memset(STK_SUM,'\0',sizeof(STK_SUM));
		setfillstyle(1,BLACK);
		bar(121,334,361,354);
		setfillstyle(1,LIGHTBLUE);
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

/***************************************************************************
FUNCTION: stk_judgesale
DESCRIPTION: 判断卖出价格与市价的关系 
INPUT: stk_code,stk_name,stk_num,stk_sum,stk_price,stk_price_mar
RETURN: 若满足需求则返回1，否则返回0
*****************************************************************************/
int stk_judgesale(char* stk_code,char* stk_name, char* stk_num, char* stk_sum, char* stk_price, char* stk_price_mar)
{
	float price = 0;
	float price_mar = 0;
	if((strlen(stk_code) != 0) && (strlen(stk_name) != 0) && (strlen(stk_num) != 0) && (strlen(stk_price) != 0) && (strlen(stk_sum) != 0) && (strlen(stk_price_mar) != 0))
	{
		turn_a2f(stk_price,&price);
		turn_a2f(stk_price_mar,&price_mar);
		if(price >= price_mar)
		{
			if((price - price_mar) > 0.05)
			{
				setfillstyle(1,BLACK);
				bar(420,170,630,190);
				puthz(420,172,"卖出价格高于卖五",16,17,LIGHTCYAN);
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
			puthz(420,172,"卖出价格低于市价",16,17,LIGHTCYAN);
			return 1;
		}
	}
	else
		return 0;
}

/********************************************************
FUNCTION: new_saleafford
DESCRIPTION: 更新股票可用余额
INPUT: u,stk_afford,STK_AFFORD
RETURN: 无
********************************************************/
void new_saleafford(USER* u, unsigned long* stk_afford, char* STK_NAME, char* STK_AFFORD)
{
	clrmous(MouseX,MouseY);
	*stk_afford = 0;		
	setfillstyle(1,WHITE);			
	bar(222,170,381,190);                   //归零以及覆盖
	setfillstyle(1,BLACK);
	bar(420,170,630,190);                   //卖五
	if(get_sale_hold(u, STK_NAME, stk_afford) != 0)
	{ 
		ultoa(*stk_afford, STK_AFFORD, 10);
		if(*stk_afford >= 100)
		{
			stk_hundred(*stk_afford, STK_AFFORD);
			setcolor(BLACK);
			settextjustify(LEFT_TEXT,TOP_TEXT);
			settextstyle(2,0,6);
			outtextxy(230, 169, STK_AFFORD);
		}   //不足100则不输出		
	} //重新遍历数据库确定可用余额
}