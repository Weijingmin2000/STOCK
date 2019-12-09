#ifndef _MARKET_H_
#define _MARKET_H_


#define MAXMARKETSIZE 8
#include"public.h"
#include"control.h"
#include"database.h"


typedef struct
{
	int number;
	char code[7];//股票代码
	char name[10];//股票名称
}Stoname;


typedef struct				   //股票列表Kline队列结构
{
	Stoname base[MAXMARKETSIZE];
	int front;				   //front端为屏幕上端
	int rear;				   //rear端为屏幕下端
	int flag;
}MarketQueue;//用于股票查找的队列

void sto_market(Ctr* cinfo, USER* u, int sto_num, char* sto_name);//要读取的某只股票的标号,用来从沪深过来的搜索
void put_marketqueue(int* pos);
void draw_market(void);
void klinequeue(MarketQueue* Q);
void input_klinequeue(MarketQueue* Q, int k);
int en_klinequeue(MarketQueue* q, FILE* fp1, int line, int prelen);
void destroy_klinequeue(MarketQueue* q);
int count_klinequeue(int k);
int countline_kline(FILE* fp1, int prelen);
void lightbutton_market(int x, int y, int x1, int y1, int color, int flag);
void recoverbutton_market(int num);

#endif