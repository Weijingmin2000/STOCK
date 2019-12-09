#ifndef _QUEUE_H_
#define _QUEUE_H_

#define MAXQSIZE 20
#include"control.h"
#include"database.h"

typedef struct				   //股票列表队结构
{
	Stodata* base;
	int front;				   //front端为屏幕上端
	int rear;				   //rear端为屏幕下端
	int flag;
}StoQueue;//股票信息的队列  

void draw_stoqueue(void);//画股票队列

void stock_queue(Ctr* cinfo,USER* u);//调用股票队列

void initqueue(StoQueue* Q);//构造存放股票信息的队列

int enqueue(StoQueue* q, FILE* fp1, int line, int prelen);//队头或队尾添加元素，向下翻页队头指针赋值，向上翻页队尾指针赋值

void destroyqueue(StoQueue* q);//销毁队列

void inputqueue(StoQueue* Q,int k);//队列赋值，k为文件开始位置

int countqueue(int k);//数每一页有多长

int countline(FILE* fp1, int prelen);//数每排有多长

#endif
