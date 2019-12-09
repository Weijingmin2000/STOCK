#include"public.h"
#include"queue.h"
#include"database.h"

/*****************************************
queue.c
COPYRIGHT:	weijingmin
FUNCTION:	extract info to hushenqueue
ABSTRACT:
		A.extract stock info from hushen
		B.create circle line
DATE:2019/9/23
******************************************/

/*********************************************
FUNCTION:initqueue
DESCRIPTION：构造存放股票信息的队列
INPUT:Q
RETURN:无
***********************************************/
void initqueue(StoQueue* Q)
{
	Q->base = (Stodata*)malloc(MAXQSIZE * sizeof(Stodata));
	if (!Q->base)
	{
		printf("memory allocation runs wrong in queue.c");
		delay(2000);
		closegraph();
		exit(1);
	}
	Q->front = Q->rear = 0;
	inputqueue(Q, 0);
	return;
}

/*********************************************
FUNCTION:inputqueue
DESCRIPTION：队列赋值
INPUT:Q,k
RETURN:无
***********************************************/
void inputqueue(StoQueue* Q,int k)
{
	FILE* fp = NULL;
	int i, j;
	int m = 0;
	Q->rear = 0;
	if ((fp = fopen("Database\\1.dat", "rb")) == NULL)
	{
		printf("cannot open file 1.dat");
		delay(2000);
		exit(1);
	}
	for (i = 0; i < MAXQSIZE; i++)          //一次二十只
	{
		memset(Q->base+Q->rear, '\0', sizeof(Stodata));
		j = enqueue(Q, fp, m, k);//j为一行的长度+1('\n')
		k =k + j;//k为文件指针内部位置
		m += 20;  //y+20
		Q->rear = (Q->rear + 1) % (MAXQSIZE);
	}

	if (fclose(fp) != 0)
	{
		printf("cannot close 1.dat");
		delay(2000);
		exit(1);
	}
	return;
}

/*********************************************
FUNCTION:enqueue
DESCRIPTION：队头或队尾添加元素，
	向下翻页队头指针赋值，向上翻页队尾指针赋值
INPUT:q,fp1,line,prelen
RETURN:len
***********************************************/
int enqueue(StoQueue* q, FILE* fp1, int line, int prelen)
{   //求每行的
	int i = 0;
	int item = 1;
	int m = 0;
	int len = 0;
	char ch = '0';
	setcolor(LIGHTGRAY);
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(0, HORIZ_DIR, 1);
	while (ch != '\n')
	{
		fseek(fp1, i * sizeof(char)+prelen, SEEK_SET);
		fread(&ch, sizeof(char), 1, fp1);
		len++;
		i++;//求这一行的长度
	}
	for (i = 0; i < len; i++)
	{
		fseek(fp1, i * sizeof(char) + prelen, SEEK_SET);
		fread(&ch, sizeof(char), 1, fp1);
		//代码	名称	涨幅%	现价	涨跌	买价	卖价	总量	现量	涨速%	换手%	今开
		if (ch != '\t' && ch != '\n' && ch != '\r' && ch != ' ')
		{
			/*if (item == 0)//代码
			{
				q->base[q->rear].number[m] = ch;
			}*/
			if (item == 1)
			{
				q->base[q->rear].code[m] = ch;
			}
			else if (item == 2)
			{
				q->base[q->rear].name[m] = ch;
			}
			else if (item == 3)
			{
				q->base[q->rear].dat1[m] = ch;
			}
			else if (item == 4)
			{
				q->base[q->rear].dat2[m] = ch;
			}
			else if (item == 5)
			{
				q->base[q->rear].dat3[m] = ch;
			}
			else if (item == 6)
			{
				q->base[q->rear].dat4[m] = ch;
			}
			else if (item == 7)
			{
				q->base[q->rear].dat5[m] = ch;
			}
			else if (item == 8)
			{
				q->base[q->rear].dat6[m] = ch;
			}
			else if (item == 9)
			{
				q->base[q->rear].dat7[m] = ch;
			}
			else if (item == 10)
			{
				q->base[q->rear].dat8[m] = ch;
			}
			else if (item == 11)
			{
				q->base[q->rear].dat9[m] = ch;
			}
			else if (item == 12)
			{
				q->base[q->rear].dat10[m] = ch;
			}
		}
		m++;
		if (ch == '\t')
		{
			m = 0;
			item++;
		}

		else if ((ch == '\n' || ch == '\r' || i == len) && item == 12)
		{
			break;
		}
	}
	setcolor(YELLOW);
	outtextxy(0, 55 + line, q->base[q->rear].code);

	if (q->base[q->rear].name[0] >= '!' && q->base[q->rear].name[0] <= 'z')
	{
		outtextxy(47, 50 + line + 5, "*ST");
		puthz(47+24, 50 + line, q->base[q->rear].name+3, 16, 17, YELLOW);
	}
	else
	{
		puthz(47, 50 + line, q->base[q->rear].name, 16, 17, YELLOW);
	}

	if (q->base[q->rear].dat1[0] == '-')
	{
		setcolor(GREEN);
	}
	else
	{
		setcolor(RED);
	}
	outtextxy(114, 55 + line, q->base[q->rear].dat1);//涨幅
	setcolor(LIGHTGRAY);
	outtextxy(164, 55 + line, q->base[q->rear].dat2);//现价

	if (q->base[q->rear].dat3[0] == '-')
	{
		setcolor(GREEN);
	}
	else
	{
		setcolor(RED);
	}
	outtextxy(214, 55 + line, q->base[q->rear].dat3);//涨跌

	setcolor(LIGHTGRAY);
	outtextxy(264, 55 + line, q->base[q->rear].dat4);//买价
	outtextxy(314, 55 + line, q->base[q->rear].dat5);//卖价
	
	setcolor(YELLOW);
	outtextxy(364, 55 + line, q->base[q->rear].dat6);//总量
	
	setcolor(MAGENTA);
	outtextxy(429, 55 + line, q->base[q->rear].dat7);//现量
	
	if (q->base[q->rear].dat8[0] == '-')
	{
		setcolor(GREEN);
	}
	else
	{
		setcolor(RED);
	}
	outtextxy(487, 55 + line, q->base[q->rear].dat8);//涨速

	setcolor(LIGHTGRAY);
	outtextxy(541, 55 + line, q->base[q->rear].dat9);//换手
	
	outtextxy(591, 55 + line, q->base[q->rear].dat10);//今开
	return len;
}


/*int dequeue(StoQueue* q, int flag)//删除队列
{
	if (q->front == q->rear)
	{
		printf("queue full!");
		delay(2000);
		exit(1);
	}
	switch(flag)
	{
		case 1:
			q->front = (q->front + 1) % MAXQSIZE;//向下翻页时队尾指针+1
			break;
		case 2:
			q->rear = (q->rear - 1);//向上翻页时队头指针-1
			if (q->rear == -1)
			{
				q->rear = 20;
			}
			break;
	}
	return 1;
}*/

/*********************************************
FUNCTION:destroyqueue
DESCRIPTION：销毁队列
INPUT:q
RETURN:无
***********************************************/
void destroyqueue(StoQueue* q)
{
	if ((q->base) != NULL)
	{
		free(q->base);
		q->base = NULL;
	}
	if (q != NULL)
	{
		free(q);
		q = NULL;
	}
}

/*********************************************
FUNCTION:countqueue
DESCRIPTION：数每一页有多长
INPUT:k
RETURN:k(一页文件的位置)
***********************************************/
int countqueue(int k)
{
	FILE* fp = NULL;
	int i, j;
	if ((fp = fopen("Database\\1.dat", "rb")) == NULL)
	{
		printf("cannot open file 1.dat");
		delay(2000);
		exit(1);
	}
	fseek(fp, k, SEEK_SET);
	for (i = 0; i < MAXQSIZE; i++)
	{
		j = countline(fp, k);//j为一行的长度+1('\n')
		k = k + j;//k为文件指针内部位置
	}
	if (fclose(fp) != 0)
	{
		printf("cannot close 1.dat");
		delay(2000);
		exit(1);
	}
	return k;
}

/*********************************************
FUNCTION:countline
DESCRIPTION：数每排有多长
INPUT:fp1,prelen
RETURN:len
***********************************************/
int countline(FILE* fp1, int prelen)
{
	int len = 0;
	int i = 0;
	char ch = '0';
	while (ch != '\n')
	{
		fseek(fp1, i * sizeof(char) + prelen, SEEK_SET);
		fread(&ch, sizeof(char), 1, fp1);
		len++;
		i++;//求这一行的长度
	}
	return len;
}