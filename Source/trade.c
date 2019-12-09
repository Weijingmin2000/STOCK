#include"trdbuy.h"
#include"public.h"
#include"trade.h"
#include"traverse.h"
#include"storead.h"
#include"trdsale.h"


/************************************************
trade.c
COPYRIGHT: dengshumin
FUNCTION:  user trade control
ABSTRACT:
		A.stock buy
		B.stock sale
		C.history deal
DATE: 2019/10/20
*************************************************/


/**********************************
FUNCTION: trade
DESCRIPTION: 实现交易界面的总控制
INPUT: cinfo, u, t, sto_num
RETURN: 无
***********************************/
//                             操作，       第几只股
void trade(Ctr* cinfo,USER* u, short int t, int sto_num) 
{
	short int trmenu_ord = t;                   //菜单按钮号
	short int trmenu_ord_last = t;
	memset(cinfo,0,sizeof(Ctr));	   //将控制信息初始化
	cinfo->func = 3;          	   	   //用3标记交易函数的界面
	clrmous(MouseX,MouseY);
	delay(10);
	cleardevice();
	mainmenu_draw();
	trade_menu_draw(trmenu_ord);
	//buy_draw();
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
		if(control_menu(cinfo))
		{
			clrmous(MouseX,MouseY);
			setfillstyle(1,BLACK);
			bar(0,25,639,480);
			mainmenu_draw();
			control_judge(u);
			trade_menu_draw(trmenu_ord);
			if(trmenu_ord == 0)
				buy_draw();
			else if(trmenu_ord == 1)
				sale_draw();
			else if(trmenu_ord == 2)
				history_draw();
			submenu_draw(cinfo->itemnum);
		}
		if(cinfo->func != 3 && cinfo->func != 13)
		{
			clrmous(MouseX,MouseY);
			delay(10);                    //防止留痕
			return; 	 //如果按了其他功能键，就返回，如果是沪深，原函数里便有判断
		}
		change_trdmenu(&trmenu_ord,&trmenu_ord_last);
		if(trmenu_ord == 0)
		{
			buy(&trmenu_ord,&trmenu_ord_last, cinfo, u, sto_num);
		}
		else if(trmenu_ord == 1)          //卖出界面下
		{	
			sale(&trmenu_ord,&trmenu_ord_last, cinfo, u);
		}
		else if(trmenu_ord == 2)          //历史成交
		{
			trd_history(&trmenu_ord,&trmenu_ord_last, cinfo, u);
		}

	}
}

/******************************************
FUNCTION: trade_menu_draw
DESCRIPTION: 绘制交易左侧菜单
ATTENTION: 根据参数来选择点亮哪个菜单
INPUT: flag
RETURN: 无
******************************************/
void trade_menu_draw(int flag)
{
	setcolor(RED);
	setlinestyle(0,0,1);
	line(70,24,120,64);
	puthz(12,32,"交易",32,36,RED);
	if(flag == 0)
	{
		puthz(63,25+50,"买入",24,25,CYAN);
		puthz(63,65+50,"卖出",24,25,WHITE);
		puthz(13,105+50,"历史成交",24,25,WHITE);
	}
	else if(flag == 1)
	{
		puthz(63,25+50,"买入",24,25,WHITE);
		puthz(63,65+50,"卖出",24,25,CYAN);
		puthz(13,105+50,"历史成交",24,25,WHITE);
	}
	else if(flag == 2)
	{
		puthz(63,25+50,"买入",24,25,WHITE);
		puthz(63,65+50,"卖出",24,25,WHITE);
		puthz(13,105+50,"历史成交",24,25,CYAN);
	}
	setcolor(RED);
	line(120,64,120,459);
/*	setfillstyle(1,WHITE);
	bar(0,456,640,480);
	puthz(10,459,"可用余额",16,17,BLACK);
	setcolor(DARKGRAY);
	setlinestyle(0,0,1);
	line(89,456,89,480);*/
}

/*************************************
FUNCTION: buy_draw
DESCRIPTION: 绘制买入界面
INPUT: void
RETURN: 无
*************************************/
void buy_draw(void)
{
	clrmous(MouseX,MouseY);
	setlinestyle(0,0,1);
	setcolor(LIGHTGRAY);
	puthz(160,40,"买入股票",24,26,RED);        //135~381
	
	setfillstyle(1,WHITE);
	puthz(135,76,"证券代码",16,17,RED);     //间隔32
	bar(222,74,381,94);                     //宽20
	
	puthz(135,108,"证券名称",16,17,RED);          //x从222到381
	
	puthz(135,140,"买入价格",16,17,RED);
	bar(222,138,381,158);
	line(362,138,362,158);
	line(362,148,381,148);
	
	puthz(135,172,"可买（股）",16,17,RED);
	bar(222,170,381,190);

	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(2, HORIZ_DIR, 6);
	setcolor(WHITE);
	outtextxy(230,200,"1/2");
	
	outtextxy(270,200,"1/3");
	
	outtextxy(310,200,"1/4");
	
	outtextxy(350,200,"1/5");

	setcolor(LIGHTGRAY);
	puthz(135,236,"买入数量",16,17,RED);      //(135,234,381,254)
	bar(222,234,381,254);
	line(362,234,362,254);
	line(362,244,381,244);

	setfillstyle(1,LIGHTBLUE);
	bar(163,274,235,304);                 //y+20
	puthz(171,277,"重置",24,32,WHITE);

	setfillstyle(1,LIGHTRED);
	bar(289,274,361,304);
	puthz(297,277,"买入",24,32,WHITE);
	
	setcolor(RED);
	setfillstyle(1,RED);
	line(372,139,378,146);
	line(378,146,366,146);
	line(366,146,372,139);
	floodfill(372,140,RED);
	line(372,157,378,150);
	line(378,150,366,150);
	line(366,150,372,157);
	floodfill(372,156,RED);
	line(372,235,378,242);
	line(378,242,366,242);
	line(366,242,372,235);
	floodfill(372,236,RED);
	line(372,253,378,246);
	line(378,246,366,246);
	line(366,246,372,253);
	floodfill(372,252,RED);       //画三角形
}

/*************************************
FUNCTION: sale_draw
DESCRIPTION: 绘制卖出界面
INPUT: void
RETURN: 无
*************************************/
void sale_draw(void)
{
/*	int tr1[] = {372,139,378,146,366,146,372,139};
	int tr2[] = {372,157,378,150,366,150,372,157};
	int tr3[] = {372,203,378,210,366,210,372,203};
	int tr4[] = {372,221,378,214,366,214,372,221};*/
	setcolor(LIGHTGRAY);
	puthz(160,40,"卖出股票",24,26,LIGHTBLUE);        //135~381
	
	setfillstyle(1,WHITE);
	puthz(135,76,"证券代码",16,17,LIGHTBLUE);     //间隔32
	bar(222,74,381,94);                     //宽20
	
	puthz(135,108,"证券名称",16,17,LIGHTBLUE);          //x从222到381
//	puthz(232,108,"平安银行",16,17,WHITE);
	
	puthz(135,140,"卖出价格",16,17,LIGHTBLUE);
	bar(222,138,381,158);
	line(362,138,362,158);
	line(362,148,381,148);
	
	puthz(135,172,"可用余额",16,17,LIGHTBLUE);
	bar(222,170,381,190);

	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(2, HORIZ_DIR, 6);
	setcolor(WHITE);
	outtextxy(230,200,"1/2");
	
	outtextxy(270,200,"1/3");
	
	outtextxy(310,200,"1/4");
	
	outtextxy(350,200,"1/5");

	setcolor(LIGHTGRAY);
	puthz(135,236,"卖出数量",16,17,LIGHTBLUE);      //(135,234,381,254)
	bar(222,234,381,254);
	line(362,234,362,254);
	line(362,244,381,244);

	setfillstyle(1,LIGHTRED);
	bar(163,274,235,304);                 //y+20
	puthz(171,277,"重置",24,32,WHITE);

	setfillstyle(1,LIGHTBLUE);
	bar(289,274,361,304);
	puthz(297,277,"卖出",24,32,WHITE);
	
	setcolor(RED);
	setfillstyle(1,RED);
	line(372,139,378,146);
	line(378,146,366,146);
	line(366,146,372,139);
	floodfill(372,140,RED);
	line(372,157,378,150);
	line(378,150,366,150);
	line(366,150,372,157);
	floodfill(372,156,RED);
	line(372,235,378,242);
	line(378,242,366,242);
	line(366,242,372,235);
	floodfill(372,236,RED);
	line(372,253,378,246);
	line(378,246,366,246);
	line(366,246,372,253);
	floodfill(372,252,RED);       //画三角形
	
}

/*************************************
FUNCTION: history_draw
DESCRIPTION: 绘制历史成交界面
INPUT: void
RETURN: 无
*************************************/
void history_draw(void)
{
	int i;
	//setfillstyle(1,WHITE);
	//bar(121,26,639,458);
	setcolor(LIGHTGRAY);
	rectangle(121,26,639,456);
	for(i=0;i<7;i++)
	{
		line(120+74*(i+1),26,120+74*(i+1),455);
		if(i == 0)
			puthz(120+74*i+4,32,"成交日期",16,17,WHITE);
		else if(i == 1)
			puthz(120+74*i+4,32,"证券代码",16,17,WHITE);
		else if(i == 2)
			puthz(120+74*i+4,32,"证券名称",16,17,WHITE);
		else if(i == 3)
			puthz(120+74*i+21,32,"操作",16,17,WHITE);
		else if(i == 4)
			puthz(120+74*i+4,32,"成交数量",16,17,WHITE);
		else if(i == 5)
			puthz(120+74*i+4,32,"成交均价",16,17,WHITE);
		else if(i == 6)
			puthz(120+74*i+4,32,"成交金额",16,17,WHITE);
	}
	for(i=0;i<17;i++)
	{
		line(121,26+24*(i+1),639,26+24*(i+1));
	}
}

/************************************************
FUNCTION: change_trdmenu
DESCRIPTION: 通过点击来改变函数菜单号
INPUT: trmenu_ord,trmenu_ord_last
RETURN: 如果改变了菜单选择返回1，否则返回0
*************************************************/
int change_trdmenu(short int *trmenu_ord,short int *trmenu_ord_last)
{
	if (mouse_press(60,75,115,100) == 2)
	{
		MouseS = 1;
		return 0;
	}
	else if(mouse_press(60,115,115,140) == 2)
	{
		MouseS = 1;
		return 0;
	}
	else if(mouse_press(10,155,115,180) == 2)
	{
		MouseS = 1;
		return 0;
	}
	else if (mouse_press(60,75,115,100) == 1)
	{
		*trmenu_ord = 0;
	}
	else if(mouse_press(60,115,115,140) == 1)
	{
		*trmenu_ord = 1;
	}
	else if(mouse_press(10,155,115,180) == 1)
	{
		*trmenu_ord = 2;
	}
	if(*trmenu_ord != *trmenu_ord_last)
	{
		clrmous(MouseX,MouseY);
		setfillstyle(1,BLACK);
		bar(121,25,640,455);
		if(*trmenu_ord == 0)
		{
			setfillstyle(1,BLACK);
			bar(60,75,115,100);
			puthz(63,25+50,"买入",24,25,CYAN);
			//buy_draw();
		}
		else if(*trmenu_ord == 1)
		{
			setfillstyle(1,BLACK);
			bar(60,115,115,140);
			puthz(63,65+50,"卖出",24,25,CYAN);
			//sale_draw();
		}
		else if(*trmenu_ord == 2)
		{
			setfillstyle(1,BLACK);
			bar(10,155,115,180);
			puthz(13,105+50,"历史成交",24,25,CYAN);
			//history_draw();
		}
		if(*trmenu_ord_last == 0)
		{
			setfillstyle(1,BLACK);
			bar(60,75,115,100);
			puthz(63,25+50,"买入",24,25,WHITE);
		}
		else if(*trmenu_ord_last == 1)
		{
			setfillstyle(1,BLACK);
			bar(60,115,115,140);
			puthz(63,65+50,"卖出",24,25,WHITE);
		}
		else if(*trmenu_ord_last == 2)
		{
			setfillstyle(1,BLACK);
			bar(10,155,115,180);
			puthz(13,105+50,"历史成交",24,25,WHITE);
		}
		*trmenu_ord_last = *trmenu_ord;
		return 1;
	}
	if(MouseS != 0)
		MouseS = 0;
	return 0;
}

/******************************************
FUNCTION: input_trade
DESCRIPTION: 股票的输入法
ATTENTION:   限制了其只能输入数字
INPUT: id,x1,y1,charnum,color
RETURN: 无
******************************************/
void input_trade(char* id, int x1, int y1, int charnum, int color)//输入的字符串，输入的xy，输入的字符限制，输入框的颜色
{
	int i = 0;
	char t;
	int s;
	clrmous(MouseX, MouseY);
	delay(20);
	setfillstyle(SOLID_FILL, color);
	setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
	setcolor(BLACK);
	settextstyle(2, 0, 6);
	settextjustify(LEFT_TEXT, TOP_TEXT);
	line(x1 + 10, y1 + 4, x1 + 10, y1 + 16);
	while(bioskey(1))
	{
		s=bioskey(1);//吃
	}
	while (1)
	{
	
		t = bioskey(0); 
		if (i < charnum)
		{
			if (t != '\n'
				&& t != '\r'
				&& t != ' '
				&& t != 033)//Esc
			{
				if ((t != '\b' && t >= '0' && t <= '9' ) || t == '.' )
				{
					*(id + i) = t;
					*(id + i + 1) = '\0';
					bar(x1 + 8 + i * 10, y1 + 3, x1 + 12 + i * 10, y1 + 20); //遮盖光标
					outtextxy(x1 + 8 + i * 10, y1-1, id + i);//输出t字符(16)
					i++;
					line(x1 + 10 + i * 10, y1 + 4, x1 + 10 + i * 10, y1 + 16);
				}
				else if (t == '\b' && i > 0)//退格键
				{
					bar(x1 + 8 + i * 10, y1 + 3, x1 + 12 + i * 10, y1 + 20);//遮盖光标
					bar(x1 - 3 + i * 10, y1 + 3, x1 + 7 + i * 10, y1 + 20);//遮盖文字
					i--;//减少一个字数
					line(x1 + 10 + i * 10, y1 + 4, x1 + 10 + i * 10, y1 + 16);//绘制光标
					*(id + i) = '\0';
					*(id + i + 1) = '\0';
				}
			}
			else
			{
				setfillstyle(SOLID_FILL, color);
				bar(x1 + 8 + i * 10, y1 + 3, x1 + 12 + i * 10, y1 + 20);//遮盖光标
				break;
			}
		}
		else if (i >= charnum)
		{
			if (t != '\n'
				&& t != '\r'
				&& t != ' '
				&& t != 033)//Esc
			{
				if (t == '\b' && i > 0)//退格键
				{
					bar(x1 + 8 + i * 10, y1 + 3, x1 + 12 + i * 10, y1 + 20);//遮盖光标
					bar(x1 - 3 + i * 10, y1 + 3, x1 + 7 + i * 10, y1 + 20);//遮盖文字
					i--;     //减少一个字数
					line(x1 + 10 + i * 10, y1 + 4, x1 + 10 + i * 10, y1 + 16);//绘制光标
					*(id + i) = '\0';
				}
			}
			else
			{
				setfillstyle(SOLID_FILL, color);
				bar(x1 + 8 + i * 10, y1 + 3, x1 + 12 + i * 10, y1 + 20);//遮盖光标  
				break;
			}
		}
	}
}


/***********************************************************
FUNCTION: trd_history
DESCRIPTION: 历史成交界面的控制函数
ATTENTION:   可以通过按UP和DOWN键来改变展示的历史成交
INPUT: trmenu_ord,trmenu_ord_last,cinfo,u
RETURN: 无
***********************************************************/
void trd_history(short int *trmenu_ord,short int *trmenu_ord_last,Ctr* cinfo,USER* u)
{ //历史成交，读取
	
	struct tm *tp = NULL;
	int page = 1;
	int sum_page = 0;
	int minute = 0;
	float stk_balance = 0;    		    //余额
	history_draw(); 
	get_time(tp, &minute, 1);           //获取时间              
	get_balance1(u, &stk_balance);      //每次调用一次buy,从数据库中获取余额,并显示
	read_balance(stk_balance);          //读出余额
	sum_page = get_hst(u, 1);            //求出总页数
	while(1)
	{
		if(strlen(u->user) == 0)
			cinfo->func = 0;
		get_time(tp, &minute, 0); 
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
		if(sum_page != 0)
		{
			if(cinfo->key == DOWN)
			{
				setfillstyle(1,BLACK);
				bar(121,24,640,455);
				history_draw(); 
				page = (page + 1) % sum_page;
				if(page == 0)             //第0页无意义
					page = sum_page;
				get_hst(u,page);
				cinfo->key = 0;
				continue;    
			}
			if(cinfo->key == UP)
			{
				setfillstyle(1,BLACK);
				bar(121,24,640,455);
				history_draw(); 
				page = (page - 1) % sum_page;   
				if(page == 0)
					page = sum_page;
				get_hst(u,page);
				cinfo->key = 0;
				continue;
			}
		}
		control_key(cinfo);
		if(control_menu(cinfo))
		{
			setfillstyle(1,BLACK);
			bar(0,25,639,480);
			trade_menu_draw(3);
			history_draw();
			get_time(tp, &minute, 1); 
			read_balance(stk_balance);
			get_hst(u,page);
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
	}
}

/********************************************
FUNCTION: float_to_str
DESCRIPTION: 将浮点数转换为字符串储存
INPUT: str,flt,bits
RETURN: 无
********************************************/
void float_to_str(char *str,float flt,int bits)		//bits为要画的限定小数位数
{
	int i;
	unsigned long integer;//整数部分
	float decimal;//带bits的小数部分
	int part;//四舍五入后的小数部分(用整数表达)
	float temp_bit;//bits的后一位需要四舍五入
	char ch_integer[5] = { '\0' };
	char ch_decimals[3] = { '\0' };
	
	*str = NULL;
	//如果为绝对值小于1的负数
	if (flt > -1.0 && flt < 0.0)
	{
		strcat(str, "-");//拷贝负号
	}
	integer = (unsigned long)flt;
	ultoa(integer, ch_integer, 10);
	strcat(str, ch_integer);//拷贝整数部分
	strcat(str, ".");

	decimal = fabs(flt - (int)flt);//小数部分
	for (i = 0; i < bits; i++)
	{
		decimal *= 10;//根据需要打印的位数化成整数
	}

	//bits的后一位temp_bits四舍五入
	temp_bit = decimal - (int)decimal;
	if (temp_bit < 0.5)
	{
		part = (int)decimal;
	}
	else if (temp_bit >= 0.5)
	{
		part = (int)decimal + 1;
	}

	//如果小数第一位为0，补全第一位
	if (part < 10 && part != 0)
	{
		strcat(str, "0");
	}
	//没有小数部分，补全0
	else if (part == 0)
	{
		strcat(str, "00");
	}

	if (part != 0)
	{
		itoa(part, ch_decimals, 10);
		strcat(str, ch_decimals);//拷贝小数部分
	}
}

/**************************************************************
FUNCTION: stk_turn_a2i
DESCRIPTION: 将字符串转换成无符号长整型
ATTENTION:   如果后面为int型指针传入，则可能会引起精度损失
INPUT: str,num
RETURN: 无
***************************************************************/
void stk_turn_a2i(char* str, unsigned long* num)	 //STK_NUM切换成int
{
	unsigned long k = 0;
	int i = 0;
	
	for (i = 0; str[i] != '\0' && str[i] != '\t'; i++)
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			k *= 10;
			k += (unsigned long)(str[i] - '0');
		}
	}
	*num = k;       //传出
}

/***********************************************
FUNCTION: stk_judge
DESCRIPTION: 判断除以某个数是否还大于0
INPUT: stk_afford,divisor
RETURN: 若大于则返回1，否则返回0
************************************************/
int stk_judge(unsigned long stk_afford, int divisor)   //divisor除数
{
	if((stk_afford / divisor) >= 100)
		return 1;
	else 
		return 0;
}

/*******************************************************
FUNCTION: stk_hundred
DESCRIPTION: 将可买股以整百形式输出进字符串中
ATTENTION:   可买股数是只读形式不会发生改变
INPUT: stk_afford,STK_AFFORD
RETURN: 无
********************************************************/
void stk_hundred(unsigned long stk_afford,char* STK_AFFORD)  //可买股以整百形式,stk_afford不变
{
	memset(STK_AFFORD,'\0',sizeof(STK_AFFORD));
	stk_afford = stk_afford/100;
	stk_afford = stk_afford*100;
	ultoa(stk_afford,STK_AFFORD,10);
}


