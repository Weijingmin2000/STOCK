#include"public.h"
#include"market.h"


/*****************************************
marketqueue.c
COPYRIGHT:	weijingmin
FUNCTION:	extract info to marketqueue
ABSTRACT:
		A.extract stock info
		B.create circle line
DATE:2019/10/7
******************************************/


/*********************************************
FUNCTION:klinequeue
DESCRIPTION：构造存放股票信息的队列
INPUT:Q
RETURN:无
***********************************************/
void klinequeue(MarketQueue* Q)
{
	/*Q->base = (Stoname*)malloc(MAXMARKETSIZE * sizeof(Stoname));
	if (!Q->base)
	{
		printf("memory allocation runs wrong");
		delay(2000);
		closegraph();
		exit(1);
	}*/
	Q->base->number=0;
	memset(Q->base->code,'\0',sizeof(Q->base->code));
	memset(Q->base->name,'\0',sizeof(Q->base->name));
	Q->front = Q->rear = 0;
	input_klinequeue(Q, 0);
	return;
}

/*********************************************
FUNCTION:input_klinequeue
DESCRIPTION：队列赋值
INPUT:Q,k
RETURN:无
***********************************************/
void input_klinequeue(MarketQueue* Q, int k)
{
	FILE* fp;
	int i, j;
	int m = 0;
	Q->rear = 0;
	if ((fp = fopen("Database\\name.dat", "rb")) == NULL)
	{
		printf("cannot open file stoname.dat");
		delay(2000);
		exit(1);
	}
	for (i = 0; i < MAXMARKETSIZE; i++)
	{
		Q->base[Q->rear].number = i;//计算该股票位于这一页的第几个
		//memset(Q->base + Q->rear, '\0', sizeof(Stodata));
		memset(Q->base[Q->rear].code, '\0', 7 * sizeof(char));
		memset(Q->base[Q->rear].name, '\0', 10 * sizeof(char));
		j = en_klinequeue(Q, fp, m, k);//j为一行的长度+1('\n')
		k = k + j;//k为文件指针内部位置
		m += 36;
		Q->rear = (Q->rear + 1) % (MAXMARKETSIZE);
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
FUNCTION:input_klinequeue
DESCRIPTION：队头或队尾添加元素，
	向下翻页队头指针赋值，向上翻页队尾指针赋值
INPUT:q,fp1,line,prelen
RETURN:len
***********************************************/
int en_klinequeue(MarketQueue* q, FILE* fp1, int line, int prelen)
{
	int i = 0;
	int item = 0;
	int m = 0;
	int len = 0;
	char ch = '0';
	setcolor(LIGHTGRAY);
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(0, HORIZ_DIR, 1);
	while (ch != '\n')
	{
		fseek(fp1, i * sizeof(char) + prelen, SEEK_SET);
		fread(&ch, sizeof(char), 1, fp1);
		len++;
		i++;//求这一行的长度
	}
	for (i = 0; i < len; i++)
	{
		fseek(fp1, i * sizeof(char) + prelen, SEEK_SET);
		fread(&ch, sizeof(char), 1, fp1);
		if (ch != '\t' && ch != '\n' && ch != '\r' && ch != ' ')
		{
			if (item == 0)//代码
			{
				q->base[q->rear].code[m] = ch;
			}
			if (item == 1)//名称
			{
				q->base[q->rear].name[m] = ch;
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
	outtextxy(2, 148 + line, q->base[q->rear].code);

	if (q->base[q->rear].name[0] >= '!' && q->base[q->rear].name[0] <= 'z')
	{
		outtextxy(2, 158 + line + 5, "*ST");
		puthz(2 + 24, 158 + line, q->base[q->rear].name + 3, 16, 17, YELLOW);
	}
	else
	{
		puthz(2, 158 + line, q->base[q->rear].name, 16, 17, YELLOW);
	}
	return len;
}

/*void destroy_klinequeue(MarketQueue* q)//销毁队列
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
}*/

/*********************************************
FUNCTION:count_klinequeue
DESCRIPTION：数每一页有多长
INPUT:k
RETURN:k(一页文件的位置)
***********************************************/
int count_klinequeue(int k)
{
	FILE* fp;
	int i, j;
	if ((fp = fopen("Database\\name.dat", "rb")) == NULL)
	{
		printf("cannot open file stoname1.dat");
		delay(2000);
		exit(1);
	}
	fseek(fp, k, SEEK_SET);
	for (i = 0; i < MAXMARKETSIZE; i++)
	{
		j = countline_kline(fp, k);//j为一行的长度+1('\n')
		k = k + j;//k为文件指针内部位置
	}
	if (fclose(fp) != 0)
	{
		printf("cannot close 1.dat");
		delay(2000);
		exit(1);
	}
	return k;//一页文件的位置
}

/*********************************************
FUNCTION:countline_kline
DESCRIPTION：数每排有多长
INPUT:fp1,prelen
RETURN:len
***********************************************/
int countline_kline(FILE* fp1, int prelen)
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