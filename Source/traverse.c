#include<io.h>
#include<direct.h>
#include"public.h"
#include"traverse.h"
#include"queue.h"
#include"trade.h"
#include"storead.h"
#define MAXHST 17
#define MAXHOLD 8



/**********************************************
traverse.c
COPYRIGHT: dengshumin
FUNCTION:  get information from database
DATE: 2019/10/29
***********************************************/

/****************************************************
FUNCTION: get_stkname
DESCRIPTION: 根据证券代码获取证券名称以及价格
INPUT: stk_code,stk_name,stk_price
RETURN: 若找到股票则返回位置，否则返回0
****************************************************/
int get_stkname(char *stk_code,char *stk_name,char *stk_price) //返回值是stock的位置
{
	FILE *fp;
	int len;
	int i = 0, l = 0;   //股票数和长度
	int j = 0, k = 0;
	int m = 0;
	int item = 1;
	char ch = '0';
	char code[7] = {'\0'};
	char name[10] = {'\0'};
	char price[7] = {'\0'};
	if ((fp = fopen("Database\\1.dat", "rb")) == NULL)
	{
		printf("cannot open file stoname.dat");
		delay(2000);
		exit(1);
	}
	for(i = 0;i < 200;i++)  //共200股票
	{
		memset(code,'\0',7*sizeof(char));
		memset(name,'\0',10*sizeof(char));   	   //每次换股重置一波
		memset(price,'\0',sizeof(price));
		l = countline(fp,k);   //l为一行的长度+1('\n')
		for(j = 0,ch = '0',item = 1,m = 0; j < l; j++)
		{
			fseek(fp, j * sizeof(char) + k, SEEK_SET); 
			fread(&ch, sizeof(char), 1, fp);
			//代码、名称
			if (ch != '\t' && ch != '\n' && ch != '\r' && ch != ' ')
			{
				if(item == 1)
					code[m] = ch;
				else if(item == 2)
					name[m] = ch;
				else if(item == 6)
					price[m] = ch;
			}
			m++;
			if(ch == '\t')
			{
				m = 0;
				item++;
			}
			else if ((ch == '\n' || ch == '\r' || i == len) && item == 12)
			{
				break;
			}
		}
		if(strcmp(code,stk_code) == 0)
		{
			memset(stk_name,'\0',10*sizeof(char));
			memset(stk_price,'\0',7*sizeof(char));
			strcpy(stk_name,name);
			strcpy(stk_price,price);
			
			if (fclose(fp) != 0)
			{
				printf("cannot close 1.dat");
				delay(2000);
				exit(1);
			}
			return i+1;     	 //返回值是stock的位置i,说明是第几只股
		}
		k = k + l;  //k为文件指针内部位置
	}
	if (fclose(fp) != 0)
	{
		printf("cannot close 1.dat");
		delay(2000);
		exit(1);
	}
	return 0;    //没找到对应股票 	
}


/**************************************************
FUNCTION: get_markettime
DESCRIPTION: 获取行情时间并显示
INPUT: tp,minute,flag
RETURN: 无
***************************************************/
void get_markettime(struct tm* tp, int* minute, int flag)
{
	char time_str1[10] = { '\0' };   //时间
	char time_str2[11] = { '\0' };   //日期
	char time_temp[10] = { '\0' };
	struct tm* time_p;
	time_t t;
	t = time(&t);
	time_p = localtime(&t);
	itoa(1900 + time_p->tm_year, time_temp, 10);
	strcat(time_str2, time_temp);
	strcat(time_str2, "/");

	memset(time_temp, '\0', sizeof(time_temp));
	itoa(1 + time_p->tm_mon, time_temp, 10);
	strcat(time_str2, time_temp);
	strcat(time_str2, "/");

	memset(time_temp, '\0', sizeof(time_temp));
	itoa(time_p->tm_mday, time_temp, 10);
	strcat(time_str2, time_temp);
	//	strcat(time_str2,"/");   //年月日

	memset(time_temp, '\0', sizeof(time_temp));
	if (time_p->tm_hour >= 10)
		itoa(time_p->tm_hour, time_temp, 10);
	else
	{
		time_temp[0] = '0';
		time_temp[1] = '0' + time_p->tm_hour;
	}
	strcat(time_str1, time_temp);
	strcat(time_str1, ":");

	memset(time_temp, '\0', sizeof(time_temp));
	if (time_p->tm_min >= 10)
		itoa(time_p->tm_min, time_temp, 10);
	else
	{
		time_temp[0] = '0';
		time_temp[1] = '0' + time_p->tm_min;
	}
	strcat(time_str1, time_temp);   	//几时几分

	if (*minute != time_p->tm_min || flag == 1)
	{
		settextjustify(LEFT_TEXT, TOP_TEXT);
		settextstyle(2, 0, 5);
		setcolor(LIGHTCYAN);
		outtextxy(3, 40, time_str2);
		outtextxy(3, 64, time_str1);
		*minute = time_p->tm_min;
	}

	tp->tm_year = time_p->tm_year;
	tp->tm_mon = time_p->tm_mon;
	tp->tm_mday = time_p->tm_mday;
	tp->tm_hour = time_p->tm_hour;
	tp->tm_min = time_p->tm_min;
	tp->tm_sec = time_p->tm_sec;
}

/**************************************************************
FUNCTION: get_time
DESCRIPTION: 在沪深和交易界面获取时间并每隔一分钟显示
INPUT: tp,minute,flag
RETURN: 无
***************************************************************/
void get_time(struct tm *tp, int *minute,int flag)
{
	char time_str1[10] = {'\0'};   //时间
	char time_str2[11] = {'\0'};   //日期
	char time_temp[10] = {'\0'};
	struct tm *time_p;
	time_t t;
	t = time(&t);
	time_p = localtime(&t);
	itoa(1900+time_p->tm_year,time_temp,10);
	strcat(time_str2,time_temp);
	strcat(time_str2,"/");
	
	memset(time_temp,'\0',sizeof(time_temp));
	itoa(1+time_p->tm_mon,time_temp,10);
	strcat(time_str2,time_temp);
	strcat(time_str2,"/");
	
	memset(time_temp,'\0',sizeof(time_temp));
	itoa(time_p->tm_mday,time_temp,10);
	strcat(time_str2,time_temp);
//	strcat(time_str2,"/");   //年月日
	
	memset(time_temp,'\0',sizeof(time_temp));
	if(time_p->tm_hour >= 10)
		itoa(time_p->tm_hour,time_temp,10);
	else
	{
		time_temp[0] = '0';
		time_temp[1] = '0' + time_p->tm_hour;
	}
	strcat(time_str1,time_temp);
	strcat(time_str1,":");
	
	memset(time_temp,'\0',sizeof(time_temp));
	if(time_p->tm_min >= 10)
		itoa(time_p->tm_min,time_temp,10);
	else
	{
		time_temp[0] = '0';
		time_temp[1] = '0' + time_p->tm_min;
	}
	strcat(time_str1,time_temp);   	//几时几分

	if(*minute != time_p->tm_min || flag == 1)
	{
		settextjustify(LEFT_TEXT,TOP_TEXT);
		settextstyle(2,0,5);
		setfillstyle(1,WHITE);
		bar(0, 456, 640, 480);
		setcolor(MAGENTA);
		outtextxy(590,454,time_str1);
		outtextxy(560,465,time_str2);
		*minute = time_p->tm_min;
	}
	tp->tm_year = time_p->tm_year;
	tp->tm_mon = time_p->tm_mon;
	tp->tm_mday = time_p->tm_mday;
	tp->tm_hour = time_p->tm_hour;
	tp->tm_min = time_p->tm_min;
	tp->tm_sec = time_p->tm_sec;
}


/**************************************
FUNCTION: get_balance1
DESCRIPTION: 从数据库中读取用户余额
INPUT: us, stk_balance
RETURN: 无
**************************************/
void get_balance1(USER* us, float* stk_balance)
{
	int i;
	int len;
	FILE* fp;		
	USER* u = NULL;
	if ((fp = fopen("database\\UserData.dat", "rb+" )) == NULL)
	{
		printf("cannot open file UserDat.dat");
		delay(3000);
		exit(1);
	}
	fseek(fp, 0, SEEK_END);//文件指针定位到最后一位
	len = ftell(fp) / sizeof(USER);//计算用户个数
	for (i = 0; i < len; i++)
	{
		if ((u = (USER*)malloc(sizeof(USER))) == NULL)
		{
			printf("memoryallocation runs wrong");
			delay(3000);
			exit(1);
		}
		fseek(fp, i * sizeof(USER), SEEK_SET);//每次往后移一个用户
		fread(u, sizeof(USER), 1, fp);		  //将信息读入u
		if (strcmp(us->user, u->user) == 0)
		{
			memset(us->balance,'\0',sizeof(us->balance));
			strcpy(us->balance,u->balance);    //将余额读入us
			if (u != NULL)
			{
				free(u);
				u = NULL;
			}//及时释放
		}
		if (u != NULL)
		{
			free(u);
			u = NULL;
		}
	}
	if (u != NULL)
	{
		free(u);
		u = NULL;
	}
	if (fclose(fp) != 0)
	{
		printf("\n cannot close Database");
		exit(1);
	}    	//读取后将释放 
	//float_to_str(us->balance,10000.00,2);
	turn_a2f(us->balance,stk_balance);      	   //将余额以浮点数传出
	/*puthz(10,459,"可用余额",16,17,BLACK);
	setcolor(DARKGRAY);
	setlinestyle(0,0,1);
	line(89,456,89,480);
	settextjustify(LEFT_TEXT,TOP_TEXT);
	settextstyle(2,0,6);
	outtextxy(99,458,us->balance);*/
}

/***************************************************************************
FUNCTION: input_balance
DESCRIPTION: 在买入或卖出股票之后，且余额发生变化后，余额写入数据库中
INPUT: us,stk_balance
RETURN: 无
***************************************************************************/
void input_balance(USER* us,float stk_balance)
{
	int i;
	int len;
	FILE* fp;		
	USER* u = NULL;
	float_to_str(us->balance,stk_balance,2); //存入已有用户信息中
	if ((fp = fopen("database\\UserData.dat", "rb+" )) == NULL)
	{
		printf("cannot open file UserDat.dat");
		delay(3000);
		exit(1);
	}
	fseek(fp, 0, SEEK_END);//文件指针定位到最后一位
	len = ftell(fp) / sizeof(USER);//计算用户个数
	for (i = 0; i < len; i++)
	{
		if ((u = (USER*)malloc(sizeof(USER))) == NULL)
		{
			printf("memoryallocation runs wrong");
			delay(3000);
			exit(1);
		}
		fseek(fp, i * sizeof(USER), SEEK_SET);//每次往后移一个用户
		fread(u, sizeof(USER), 1, fp);		  //将信息读入u
		if (strcmp(us->user, u->user) == 0)
		{
			memset(u->balance,'\0',sizeof(u->balance));     //重置u
			strcpy(u->balance,us->balance);                 //将信息复制
			fseek(fp, i * sizeof(USER), SEEK_SET);
			fwrite(u, sizeof(USER), 1, fp);       //将修改过的余额重新写入 
			if (u != NULL)
			{
				free(u);
				u = NULL;
			}//及时释放
			if (fclose(fp) != 0)
			{
				printf("\n cannot close Database");
				exit(1);
			}
			return;
		}
		if (u != NULL)
		{
			free(u);
			u = NULL;
		}
	}
	if (u != NULL)
	{
		free(u);
		u = NULL;
	}
	if (fclose(fp) != 0)
	{
		printf("\n cannot close Database");
		delay(2000);
		exit(1);
	}
	return;
}

/*****************************************
FUNCTION: read_balance
DESCRIPTION: 将当前余额读出
INPUT: stk_balance
RETURN: 无
******************************************/
void read_balance(float stk_balance)
{
	char STK_BALANCE[15] = {'\0'};
	float_to_str(STK_BALANCE,stk_balance,2);
	puthz(10,459,"可用余额",16,17,BLACK);
	setcolor(DARKGRAY);
	setlinestyle(0,0,1);
	line(89,456,89,480);
	settextjustify(LEFT_TEXT,TOP_TEXT);
	settextstyle(2,0,6);
	outtextxy(99,458,STK_BALANCE);
}

/************************************************
FUNCTION: get_hst
DESCRIPTION: 获取历史交易记录
INPUT: u,page
RETURN: 返回有多少页
************************************************/
int get_hst(USER* u,int page)  //第page页
{
	FILE* fp;
	Hstdata hst;        //历史成交
	char STK_ADR[50] = {'\0'};
	int len;
	int i = 0,l = 0;    
	int j = 0,k = 0;
	int m = 0;
	int t = 0;          //相对位置
	int item = 1;
	char ch = '0';
	strcpy(STK_ADR,"database\\USER\\");   //遇到'\0'结束
	strcat(STK_ADR,u->user);
	strcat(STK_ADR,"\\");
	strcat(STK_ADR,"history.dat");
	if((fp=fopen(STK_ADR,"rb"))==NULL)
	{
		//printf("can't find the file history.dat while init the data of stock!\n");
		//delay(1000);
		//exit(1);
		return 0;  //如果打不开就是0页
	}      //只读
	fseek(fp, 0, SEEK_END);
	len = ftell(fp) / sizeof(Hstdata); //计算交易次数且一次最多显示17次
	k = (page - 1) * MAXHST * sizeof(Hstdata);                          //第page页的起始位置(也许int65535会不够用)
	for(i = MAXHST * (page - 1);i < len && i< MAXHST * page; i++)	    //一次最多17只
	{
		memset(&hst,'\0',sizeof(Hstdata));   //初始化
		//k = (page - 1) * MAXHST * sizeof(Hstdata);       //初始时文件位置
		l = countline(fp,k);   //l为一行的长度+1('\n')
		for(j = 0,ch = '0',item = 1,m = 0; j < l; j++)
		{
			fseek(fp, j * sizeof(char) + k,SEEK_SET);
			fread(&ch, sizeof(char),1 , fp);
			if(ch != '\t' && ch != '\n' && ch != '\r' && ch != ' ')
			{
				if(item == 1)
					hst.sto_date[m] = ch;
				else if(item == 2)
					hst.sto_code[m] = ch;
				else if(item == 3)
					hst.sto_name[m] = ch;
				else if(item == 4)
					hst.sto_flag[m] = ch;
				else if(item == 5)
					hst.sto_num[m] = ch;
				else if(item == 6)
					hst.sto_price[m] = ch;
				else if(item == 7)
					hst.sto_amount[m] = ch;
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
		k = k + l;  //k为文件指针内部位置
		t = i - (page - 1) * MAXHST;          //相对位置
		if(hst.sto_flag[0] == '1')   //买入
		{
			setcolor(LIGHTRED);
			puthz(120+74*3+21,26 + 24 * (t + 1) + 5,"买入",16,17,LIGHTRED);
			puthz(120 + 74 * 2 + 5,26 + 24 * (t + 1) + 5,hst.sto_name,16,17,LIGHTRED); //证券名称
		}
		else if(hst.sto_flag[0] == '2')
		{
			setcolor(LIGHTBLUE);     //卖出
			puthz(120 + 74 * 3 + 21, 26 + 24 * (t + 1) + 5,"卖出",16,17,LIGHTBLUE);
			puthz(120 + 74 * 2 + 5, 26 + 24 * (t + 1) + 5, hst.sto_name,16,17,LIGHTBLUE); //证券名称
		}
		settextjustify(LEFT_TEXT,TOP_TEXT);
		settextstyle(2,0,4);
		outtextxy(120 + 74 * 0 + 4,26 + 24 * (t + 1) + 5,hst.sto_date);  //成交日期
		
		settextstyle(2,0,5);
		outtextxy(120 + 74 * 1 + 4,26 + 24 * (t + 1) + 5,hst.sto_code);  //证券代码
		
		outtextxy(120 + 74 * 4 + 4,26 + 24 * (t + 1) + 5,hst.sto_num);   //成交数量
		
		outtextxy(120 + 74 * 5 + 4,26 + 24 * (t + 1) + 5,hst.sto_price); //成交均价
		
		outtextxy(120 + 74 * 6 + 4,26 + 24 * (t + 1) + 5,hst.sto_amount);//成交金额
	}   //显示完毕
	if((len % MAXHST) != 0)
	{
		if (fclose(fp) != 0)
		{
			printf("cannot close 1.dat");
			delay(2000);
			exit(1);
		}
		return len / MAXHST + 1;           //返回有多少页
	}
	else                                   //若刚好满一页
	{
		if (fclose(fp) != 0)
		{
			printf("cannot close 1.dat");
			delay(2000);
			exit(1);
		}
		return len / MAXHST ;           //返回有多少页
	}
}


/*******************************************************************
FUNCTION: get_hold_stk
DESCRIPTION: 获取持仓数据库中某一只股
ATTENTION: 在买入或卖出股票后，在持仓中进行单只股票的查找
INPUT: hold,STK_NAME,STK_ADR
RETURN: 若找到股票则返回位置，否则返回0
*******************************************************************/
int get_hold_stk(Hold* hold, char* STK_NAME, char* STK_ADR)         //获取持仓数据库中某一只股
{      //直接用hold接收
	FILE* fp;
	int len;
	int i = 0, l = 0;   //股票数和长度
	int j = 0, k = 0;
	int m = 0;
	int item = 1;
	char ch = '0';
	if((fp = fopen(STK_ADR,"rb")) == NULL)
	{
		return;
	}	//如果打不开就返回
	fseek(fp, 0, SEEK_END);
	len = ftell(fp) / sizeof(Hold);  //计算持股股数
	for(i = 0;i < len; i++)
	{
		memset(hold,'\0',sizeof(Hold));
		l = countline(fp,k);     //l为一行的长度+1('\n')
		for(j = 0,ch = '0',item = 1,m = 0; j < l; j++)
		{
			fseek(fp, j * sizeof(char) + k, SEEK_SET);
			fread(&ch, sizeof(char), 1, fp);
			if(ch != '\t' && ch != '\n' && ch != '\r' && ch != ' ')
			{
				if(item == 1)
					hold->sto_date[m] = ch;
				else if(item == 2)
					hold->sto_code[m] = ch;
				else if(item == 3)
					hold->sto_name[m] = ch;
				else if(item == 4)
					hold->sto_num[m] = ch;
				else if(item == 5)
					hold->sto_price[m] = ch;
				else if(item == 6)
					hold->sto_day[m] = ch;
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
		k = k + l;
		if(strcmp(STK_NAME,hold->sto_name) == 0)
		{    //如果查找到股票
			if (fclose(fp) != 0)
			{
				printf("cannot close hold.dat");
				delay(2000);
				exit(1);
			}
		//	printf("%d",i+1);
			return i + 1 ;
		}
	} //没找到股票
	memset(hold,'\0',sizeof(Hold));
	if (fclose(fp) != 0)
	{
		printf("cannot close hold.dat");
		delay(2000);
		exit(1);
	}
	return 0;   //没找到对应股票
}

/************************************************************
FUNCTION: get_hold_day
DESCRIPTION: 获取并更新持仓时间
INPUT: prime,now
RETURN: 持仓天数
*************************************************************/
int get_hold_day(int* prime,int* now)
{
	int y1, m1; 
	int y2, m2;	
	long d1, d2;
	m1 = (prime[1] + 9) % 12;  //用于判断日期是否大于3月（2月是判断闰年的标识），还用于纪录到3月的间隔月数。
	y1 = prime[0] - m1 / 10;   //如果是1月和2月，则不包括当前年（因为是计算到0年3月1日的天数）。
	d1 = 365*y1 + y1/4 - y1/100 + y1/400 + (m1*306 + 5)/10 + (prime[2] - 1);
	//其中 365*y1 是不算闰年多出那一天的天数.
	//y1/4 - y1/100 + y1/400  是加所有闰年多出的那一天，
	m2 = (now[1] + 9) % 12;
	y2 = now[0] - m2/10;
	d2 = 365*y2 + y2/4 - y2/100 + y2/400 + (m2*306 + 5)/10 + (now[2] - 1);
	//(m2*306 + 5)/10 用于计算到当前月到3月1日间的天数，306=365-31-28（1月和2月），5是全年中不是31天月份的个数
	//(day_start - 1)用于计算当前日到1日的间隔天数。
	return (int)(d2 - d1);
}

/******************************************************
FUNCTION: get_hold
DESCRIPTION: 获取持仓信息
INPUT: u,page
RETURN: 返回有多少页
*******************************************************/
int get_hold(USER* u,int page)             
{  //在个人界面获取持仓信息 
	FILE* fp;
	Hold hold;
	char STK_ADR[50] = {'\0'};  
	int i = 0, j = 0;
	int l= 0, k = 0;
	int m = 0, t = 0;
	int len = 0, item = 0;
	char ch = '0';
	memset(&hold, '\0', sizeof(Hold));
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
	if((fp = fopen(STK_ADR, "rb")) == NULL)
	{
		return 0;
	}
	fseek(fp, 0, SEEK_END);
	len = ftell(fp) / sizeof(Hold);          //计算持股股数
	k = (page - 1) * MAXHOLD * sizeof(Hold);
	for(i = MAXHOLD * (page - 1); i < MAXHOLD * page && i < len; i++)
	{
		memset(&hold, '\0', sizeof(Hold));  //显示8只以内的
		l = countline(fp,k);                 //
		for(j = 0,ch = '0',item = 1,m = 0;j < l;j++)
		{
			fseek(fp, j * sizeof(char) + k, SEEK_SET);//显示
			fread(&ch, sizeof(char), 1, fp);
			if(ch != '\t' && ch != '\n' && ch != '\r' && ch != ' ')
			{
				if(item == 1)
					hold.sto_date[m] = ch;
				else if(item == 2)
					hold.sto_code[m] = ch;
				else if(item == 3)
					hold.sto_name[m] = ch;
				else if(item == 4)
					hold.sto_num[m] = ch;
				else if(item == 5)
					hold.sto_price[m] = ch;
				else if(item == 6)
					hold.sto_day[m] = ch;
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
		k = k + l;            //k为文件指针内部位置
		t = i - (page - 1) * MAXHOLD;
		settextjustify(LEFT_TEXT,TOP_TEXT);
		settextstyle(2, 0, 5);
		setcolor(RED);
		outtextxy(53, 300 + 20 * (t + 1), hold.sto_date);
		
		settextstyle(2,0,6);
		setcolor(GREEN);
		outtextxy(63 + 90 * 1, 297 + 20 * (t + 1), hold.sto_code);
		
		puthz(58 + 90 * 2, 297 + 20 * (t + 1) + 2, hold.sto_name, 16, 17, BLUE);
		
		setcolor(LIGHTMAGENTA);
		outtextxy(63 + 90 * 3, 297 + 20 * (t + 1), hold.sto_num);
			
		setcolor(LIGHTBLUE);
		outtextxy(63 + 90 * 4, 297 + 20 * (t + 1), hold.sto_price);
		
		setcolor(DARKGRAY);
		outtextxy(63 + 90 * 5, 297 + 20 * (t + 1), hold.sto_day);
	}
	fseek(fp, 0, SEEK_END);
	if((len % MAXHOLD) != 0)
	{
		if (fclose(fp) != 0)
		{
			printf("cannot close 1.dat");
			delay(2000);
			exit(1);
		}
		return len / MAXHOLD + 1;           //返回有多少页
	}
	else      //恰好某一页
	{
		if (fclose(fp) != 0)
		{
			printf("cannot close 1.dat");
			delay(2000);
			exit(1);
		}
		return len / MAXHOLD ;           //返回有多少页
	}           
}

/*****************************
FUNCTION: get_asset
DESCRIPTION: 获取资产
INPUT: u
RETURN: 无
*****************************/
void get_asset(USER* u)
{
	settextjustify(LEFT_TEXT,TOP_TEXT);
	settextstyle(2,0,7);
	setcolor(DARKGRAY);
	outtextxy(145, 170, u->user);
	outtextxy(145, 210, u->bankcard);
	outtextxy(145, 250, u->balance);
}

/****************************************************
FUNCTION: get_buy
DESCRIPTION: 根据股票位置来获取数据库里的某只股
INPUT: stk_locat,stk_code,stk_name,stk_price
RETURN: 无
*****************************************************/
void get_buy(int stk_locat,char* stk_code,char* stk_name, char* stk_price)
{         
	FILE* fp;
	int i = 0, l = 0;
	int j = 0, k = 0;
	int m = 0;
	int item = 1;
	char ch = '0';
	if ((fp = fopen("Database\\1.dat", "rb")) == NULL)
	{
		printf("cannot open file stoname.dat");
		delay(2000);
		exit(1);
	}
	memset(stk_code,'\0',7*sizeof(char));
	memset(stk_name,'\0',10*sizeof(char));   	   
	memset(stk_price,'\0',sizeof(stk_price));
	for(i = 0; i < stk_locat - 1; i++)
	{
		l = countline(fp,k);
		k = k + l;
	}//获取第stk_locat行的位置
	l = countline(fp,k);
	for(j = 0,ch = '0',item = 1,m = 0;j < l; j++)
	{
		fseek(fp, j * sizeof(char) + k, SEEK_SET);
		fread(&ch, sizeof(char), 1, fp);
		if (ch != '\t' && ch != '\n' && ch != '\r' && ch != ' ')
		{
			if(item == 1)
				stk_code[m] = ch;
			else if(item == 2)
				stk_name[m] = ch;
			else if(item == 6)
				stk_price[m] = ch;
		}
		m++;
		if(ch == '\t')
		{
			m = 0;
			item++;
		}
		else if(ch == '\n' || ch == '\r') 
		{
			break;
		}
	}
	if(fclose(fp) != 0)
	{
		printf("cannot close 1.dat");
		delay(1000);
		exit(1);
	}
}

/****************************************
FUNCTION: update_hold_day
DESCRIPTION: 更新持仓天数
INPUT: tp,u
RETURN: 无
****************************************/
void update_hold_day(struct tm* tp, USER* u)
{
	FILE* fp;
	Hold hold;
	int len;
	int i = 0, l = 0;
	int j = 0, k = 0;
	int m = 0;
	int item = 1;
	int prime[3] = {0};
	int now[3] = {0};
	int day = 0;
	char ch = '0';
	char STK_ADR[50] = {'\0'};    		     //地址段
	memset(&hold,'\0',sizeof(Hold));  		 //初始化
	strcpy(STK_ADR,"database\\USER\\");      //遇到'\0'结束
	strcat(STK_ADR,u->user);
	if(access(STK_ADR, 0) == -1)             //不存在文件夹
	{
		if(mkdir(STK_ADR) != 0)              //未成功创建
		{
			printf("The file opens failued");
		}
	}//若存在则直接打开
	strcat(STK_ADR,"\\");
	strcat(STK_ADR,"hold.dat");
	if((fp = fopen(STK_ADR, "rb+")) == NULL)
	{
		return; //如果不存在持仓直接返回，无事发生
	}
	fseek(fp, 0, SEEK_END);
	len = ftell(fp) / sizeof(Hold);  //计算持股股数
	now[0] = tp->tm_year + 1900;
	now[1] = tp->tm_mon + 1;
	now[2] = tp->tm_mday;
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
				if(item == 1)
					hold.sto_date[m] = ch;
				else if(item == 2)
					hold.sto_code[m] = ch;
				else if(item == 3)
					hold.sto_name[m] = ch;
				else if(item == 4)
					hold.sto_num[m] = ch;
				else if(item == 5)
					hold.sto_price[m] = ch;
			/*	else if(item == 6)
					hold.sto_day[m] = ch;   //不接收天数*/
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
		k = k + l;
		turn_a2i(hold.sto_date, prime, prime + 1, prime + 2);
		day = get_hold_day(prime, now);
		itoa(day, hold.sto_day, 10);
		hold.sto_date[10] = '\t';
		hold.sto_code[6] = '\t';  
		hold.sto_name[9] = '\t';
		hold.sto_num[9] = '\t';
		hold.sto_price[6] = '\t';
		hold.sto_day[3] = '\n';
		fseek(fp, i * sizeof(Hold), SEEK_SET);
		fwrite(&hold, sizeof(Hold), 1, fp);
	}
	if (fclose(fp) != 0)
	{
		printf("\n cannot close Database");
		exit(1);
	}
}