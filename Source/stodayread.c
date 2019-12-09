#include"storead.h"
#include"public.h"
#include"database.h"

/*****************************************
stodayread.c
COPYRIGHT:	weijingmin
FUNCTION:	sto daybyday information
ABSTRACT:
		A.get one stock for 150 days
		B.change string to float and int
DATE:2019/10/12
******************************************/



/*int check_database(int sto_number)//返回该数据库的最大值
{
	FILE* fp = NULL;
	long int len;
	long int i;
	int line = 0;
	char ch = '\0';
	char ctl[30] = "Database\\dayinfo\\", num[4] = { '\0' };//目录
	itoa(sto_number, num, 10);//股票序号类型转换，方便文件读取
	strcat(ctl, num);
	strcat(ctl, ".dat");


	if ((fp = fopen(ctl, "rb")) == NULL)
	{
		printf("cannot open dayinfo\\%d", sto_number);
		delay(3000);
		exit(1);
	}
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);

	for (i = 0; i < len; i++)
	{
		fseek(fp, i * sizeof(char), SEEK_SET);
		fread(&ch, sizeof(char), 1, fp);
		if (ch == '\n')
		{
			line++;//计算数据库的行数
		}
	}
	line -= 10;
	return line;
}*/

/*********************************************
FUNCTION:sto_daybyday2float
DESCRIPTION:将所得天数的信息转换为浮点数
INPUT:sto_num,No_date,read_days,info
RETURN:无
***********************************************/
void sto_daybyday2float(int sto_num, long int No_date, long int read_days, STODAYFLOAT info[])//要读取的天数
{
	int i = 0;
	STODAYCHAR s[150];
	//STODAYCHAR* s = NULL;//s为指向很多天股票信息的指针
	/*if ((info = (STODAYFLOAT*)malloc(read_days * sizeof(STODAYFLOAT))) == NULL)
	{
		printf("memoryallcation runs wrong");
		delay(3000);
		exit(1);
	}*/
	
	/*if ((s = (STODAYCHAR*)malloc(read_days * sizeof(STODAYCHAR))) == NULL)
	{
		printf("memoryallocation runs wrong");
		delay(3000);
		exit(1);
	}*/
	memset(s, '\0', 150 * sizeof(STODAYCHAR));

	sto_daybyday(sto_num, No_date, read_days, s);//调用sto_daybyday
	for (i = 0; i < read_days; i++)
	{
		turn_a2_f_t(&s[i], &info[i]);
		/*if(s[i].open=='\0')//待修改！！！
		{
			break;//读到数据库末端，停止读取
		}*/
	}
	/*if (s != NULL)
	{
		free(s);
		s = NULL;
	}*/
}

/*********************************************
FUNCTION:sto_daybyday
DESCRIPTION:获取从指定天数开始的股票信息
INPUT:sto_number,start_date,days,stk
RETURN:无
***********************************************/
void sto_daybyday(int sto_number, long int start_date, long int days, STODAYCHAR* stk)//股票序号，股票读取开始日期，读取天数
{
	FILE* fp;
	int line_num = 1, i = 0, j = 0;//i为文件指到指定位置的标号数，j为从i往后移的标号
	int m = 0, d = 0;//m为每个字符对应文件的标号，d为找股票日期的标号数
	int item = 0;
	long int len;
	char ch = '\0';
	char ctl[30] = "Database\\dayinfo\\", num[4] = { '\0' };//目录
	itoa(sto_number, num, 10);//股票序号类型转换，方便文件读取
	strcat(ctl, num);
	strcat(ctl, ".dat");

	
	if ((fp = fopen(ctl, "rb")) == NULL)
	{
		printf("cannot open dayinfo\\%d", sto_number);
		delay(3000);
		exit(1);
	}
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	for (i = 0; line_num < start_date && i < len; i++)
	{
		fseek(fp, i * sizeof(char), SEEK_SET);
		fread(&ch, sizeof(char), 1, fp);
		if (ch == '\n')
		{
			line_num++;
			i++;//换行读取
		}
	}
	if (i != 0)
	{
		i--;//回退一格，从某排的第0格开始读取 
	}

	//如果该日期编号不存在
	if (i == len)
	{
		if (fclose(fp) != 0)//关闭文件
		{
			printf("Error in closing file \"StkInfo\\%d\".\n", sto_number);
			delay(3000);
			exit(1);
		}
		return;
	}
	else
	{
		while (1)
		{
			fseek(fp, (i + j) * sizeof(char), SEEK_SET);//从要读取天数的位置往后读取,此时文件指针位置在待读取天数的位置
			fread(&ch, sizeof(char), 1, fp);
			if (ch != '\t' && ch != '\n' && ch != '\r' && ch != ' ')
			{

				if (item == 0)
				{
					stk[d].date[m] = ch;
				}
				else if (item == 1)
				{
					stk[d].open[m] = ch;
				}
				else if (item == 2)
				{
					stk[d].high[m] = ch;
				}
				else if (item == 3)
				{
					stk[d].low[m] = ch;
				}
				else if (item == 4)
				{
					stk[d].close[m] = ch;
				}
				else if (item == 5)
				{
					stk[d].VOL[m] = ch;
				}
				else if (item == 6)
				{
					stk[d].VOT[m] = ch;
				}
			}
			if (ch != '\n')
			{
				m++;//往后移
			}
			if (ch == '\t')
			{
				m = 0;
				item++;//读下一个数据
			}
			else if ((ch == '\n') && (item == 6))
			{
				d++;//天数标号后移
				if (d == days)
				{
					break;//读到要读的位置
				}
				else
				{
					item = 0;
					m = 0;//两个标号归零
				}
			}
			j++;//每次读一个字符并进行相应的操作，读完后重新进入循环
		}
	}
	if (fclose(fp) != 0)
	{
		printf("cannot close dayinfo\\%d after read", sto_number);
		delay(3000);
		exit(1);
	}
}

/*********************************************
FUNCTION:turn_a2_f_t
DESCRIPTION:字符串转为整形或浮点型控制函数
INPUT:sto_number,start_date,days,stk
RETURN:无
***********************************************/
void turn_a2_f_t(STODAYCHAR* str, STODAYFLOAT* f)
{
	turn_a2i(str->date, &(f->Year), &(f->Month), &(f->Day));
	turn_a2f(str->open, &(f->Open));
	turn_a2f(str->high, &(f->High));
	turn_a2f(str->low, &(f->Low));
	turn_a2f(str->close, &(f->Close));
	turn_a2f(str->VOL,&(f->VOL));//此处有大问题!
	//f->VOL = (atoi(str->VOL));
	turn_a2f(str->VOT, &(f->VOT));
}

/*********************************************
FUNCTION:turn_a2f
DESCRIPTION:将每日指标都转换为浮点数
INPUT:str,price
RETURN:无
***********************************************/
void turn_a2f(char* str, float* price)
{
	float price1 = 0.0, price3 = 0.0;
	float price2 = 1.0;
	int i, j;
	for (i = 0; str[i] != '.'&&str[i]!='\0'; i++)
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			price1 *= 10.0;
			price1 += (float)(str[i] - '0');//整数部分
		}
	}
	for (j = i; str[j] != '\0'; j++)
	{
		if (str[j] >= '0' && str[j] <= '9')
		{
			price2 /= 10.0;//计算位数
			price3 *= 10.0;
			price3 += (float)(str[j] - '0');//小数部分
		}
	}
	*price = price1 + price2 * price3;
}

/*********************************************
FUNCTION:turn_a2i
DESCRIPTION:date转换为int
INPUT:str,y,m,d
RETURN:无
***********************************************/
void turn_a2i(char* str, int* y, int* m, int* d)//date切换成int
{
	int k = 0;
	int i, j = 0;
	int num[3] = { 0 };
	for (i = 0; str[i] != '\0'; i++)
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			k *= 10;
			k += (int)(str[i] - '0');
		}
		else if (str[i] == '/')
		{
			num[j] = k;	//日期赋给数组
			j++;
			k = 0;		//算日期的重新归零 
		}
	}
	num[j] = k;
	*y = num[0];
	*m = num[1];
	*d = num[2];
}