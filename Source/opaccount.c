#include"public.h"
#include"open.h"
#include"lgfunc.h"
#include"control.h"


/***********************************************
opaccount.c
COPYRIGHT: 	dengshumin
FUNCTION: open a new account
ABSTRACT:
		A.open account for user
		B.button lighting and recovering
DATE: 2019/9/22
************************************************/


/*******************************************************************
FUNCTION: opaccount
DESCRIPRION: 开户函数,若已开户则会给与弹窗，未开户则进行开户操作
INPUT: cinfo,u
RETURN: 无
********************************************************************/
void opaccount(Ctr* cinfo,USER* u)
{
	int num = 0;              //按钮号
	char B[21] = {0};   //Bankcard
	char BP[8] = {0};
	char BCP[8] = {0};
	short int state1 = 0;
	short int state2 = 0;
	short int state3 = 0;
	memset(cinfo,0,sizeof(Ctr));   //将控制信息初始化
	cinfo->func = 1;          //用1标记开户函数的界面
	clrmous(MouseX,MouseY);
	delay(10);
	if(strlen(u->bankcard) == 0)    //未开户
	{
		cleardevice();
		mainmenu_draw();
		control_judge(u);
		draw_opaccount();
		while (1)
		{
			if(!bioskey(1))
			{
				newmouse(&MouseX, &MouseY, &press);
				control_mouse(cinfo,u);
			}
			else if(bioskey(1))
				cinfo->key = bioskey(0);
			control_key(cinfo);
			if(control_menu(cinfo))
			{
				draw_opaccount();
				submenu_draw(cinfo->itemnum);
			}
			if(cinfo->func != 1 && cinfo->func != 12)
			{
				clrmous(MouseX,MouseY);
				delay(10);                    //防止留痕
				return;  //如果按了其他功能键，就返回，如果是沪深，原函数里便有判断
			}
			if(MouseX>100&&MouseX<540&&MouseY>120&&MouseY<145)
			{ //银行卡号框
				if(mouse_press(100,120,540,145) == 2)
				{
					MouseS = 2;
					if(num == 0 && state1 == 0)
					{
						lightbutton_opa( 100, 120, 540, 145, RED, LIGHTGRAY,1);
						num = 1;
					}
					continue;
				}
				else if(mouse_press(100,120,540,145) == 1)
				{
					MouseS = 0;//输入
					lightbutton_opa( 100, 120, 540, 145, RED, LIGHTGRAY,1);
					B[0] = '\0';
					inputadmin(B,100,120,19,LIGHTGRAY);
					if(strlen(B) != 0 )
						state1 = 1;
					else	
						state1 = 0;
					continue;
				}
			}
			if(MouseX>100&&MouseX<540&&MouseY>190&&MouseY<215)
			{ //银行卡密码框
				if(mouse_press(100,190,540,215) == 2)
				{
					MouseS = 2;
					if(num == 0 && state2 == 0)
					{
						lightbutton_opa(100,190,540,215,RED,LIGHTGRAY,2);
						num = 2;
					}
					continue;
				}
				else if(mouse_press(100,190,540,215) == 1)
				{
					MouseS = 0;
					lightbutton_opa(100,190,540,215,RED,LIGHTGRAY,2);
					BP[0] = '\0';
					inputpassword(BP,100,190,6,LIGHTGRAY);
					if(strlen(BP) != 0)
						state2 = 1;
					else 
						state2 = 0;
					continue;   //输入
				}
			}
			if(MouseX > 100 && MouseX < 540 && MouseY > 260 && MouseY<285)
			{ //确认密码框
				if(mouse_press(100,260,540,285) == 2)
				{
					MouseS = 2;
					if(num == 0 && state3 == 0)
					{
						lightbutton_opa(100,260,540,285,RED,LIGHTGRAY,3);
						num = 3;
					}
					continue;
				}
				else if(mouse_press(100,260,540,285) == 1)
				{
					MouseS = 0;
					lightbutton_opa(100,260,540,285,RED,LIGHTGRAY,3);
					BCP[0] = '\0';
					inputpassword(BCP,100,260,6,LIGHTGRAY);
					if(strlen(BCP) != 0)
						state3 = 1;
					else 
						state3 = 0;
					continue ; //输入
				}
			}
			if(MouseX > 143 && MouseX <237 && MouseY > 350 && MouseY <380)
			{   //重置
				if(mouse_press(143,350,237,380) == 2)
				{
					if(num == 0)
					{
						MouseS = 1;
						lightbutton_opa(143,350,237,380,WHITE,GREEN,4);
						num = 4;
					}
					continue;
				}
				else if(mouse_press(143,350,237,380) == 1)
				{
					MouseS = 0;
					clrmous(MouseX,MouseY);
					cleardevice();
					delay(10);
					mainmenu_draw();
					control_judge(u);
					draw_opaccount();
					num = 0;
					B[0] = '\0';
					BP[0] = '\0';
					BCP[0] = '\0';
					state1 = 0;
					state2 = 0;
					state3 = 0;
					continue;
				}
			}
			if(MouseX>273 && MouseX < 367 && MouseY > 350 && MouseY <380)
			{   //开户
				if(mouse_press(273,350,367,380) == 2)
				{
					if(num == 0)
					{
						MouseS = 1;
						lightbutton_opa(273,350,367,380,WHITE,RED,5);
						num = 5;
					}
					continue;
				}
				else if(mouse_press(273,350,367,380) == 1)
				{
					MouseS = 0;
					if(opaccount_complete(B,BP,BCP,u))
					{
						cinfo->func = 0;
						return ;
					}
					else
					{
						continue;
					}
				}
			}
			if(MouseX > 403 && MouseX < 497 && MouseY > 350 && MouseY <380)
			{   //返回
				if(mouse_press(403,350,497,380) == 2)
				{
					if(num == 0)
					{
						MouseS = 1;
						lightbutton_opa(403,350,497,380,WHITE,BLUE,6);
						num = 6;
					}
					continue;
				}
				else if(mouse_press(403,350,497,380) == 1)
				{
					MouseS = 0;
					cinfo->func = 0;
					return ;
				}
			}	
			if(num != 0)
			{
				if (state1 == 0)
					recoverbutton_opa(1);
				if (state2 == 0)
					recoverbutton_opa(2);
				if (state3 == 0)
					recoverbutton_opa(3);
				if (num == 4 || num == 5 || num == 6)
					recoverbutton_opa(num);
				num = 0;
			}
			if(MouseS != 0)
			{
				MouseS = 0;
			}
		}
	}
	else          //已开户
	{
		draw_exist();
		while (1)
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
				draw_exist();
			}
			if(cinfo->func != 1)
			{
				clrmous(MouseX,MouseY);     //防止留痕 
				return;  //如果按了功能键，就返回，如果是沪深，原函数里便有判断
			}
			if(mouse_press(280,240,360,280) == 2)    //未点击确认框
			{
				if(num == 0)
				{
					MouseS = 1;
					lightbutton_opa(280,240,360,280,WHITE,RED,7);
					num = 7;
				}
				continue;
			}
			else if(mouse_press(280,240,360,280) == 1)
			{
				MouseS = 0;
				cinfo->func = 0;
				break;
			}
			if(num != 0)
			{
				recoverbutton_opa(num);
				num = 0;
			}
			if(MouseS != 0)
			{
				MouseS = 0;
			}
		}
	}
	return;
}

/**********************************
FUNCTION: draw_opaccount
DESCRIPRION: 绘制开户界面
INPUT: void
RETURN: 无
**********************************/
void draw_opaccount(void)
{
	setbkcolor(BLACK);
	setcolor(WHITE);
	setfillstyle(1,WHITE);
	settextjustify(LEFT_TEXT,TOP_TEXT);
	settextstyle(1,HORIZ_DIR,2);
	
	puthz(100,100,"银行卡号（应为",16,17,WHITE);
	outtextxy(219,94,"19");
	puthz(248,100,"位）",16,17,WHITE);
	bar(100,120,540,145);
	
	puthz(100,170,"密码（应为",16,17,WHITE);
	outtextxy(188,164,"6");
	puthz(202,170,"位）",16,17,WHITE);
	bar(100,190,540,215);
	
	puthz(100,240,"确认密码",16,17,WHITE);
	bar(100,260,540,285);
	
	setfillstyle(1, GREEN);
	bar(143, 350, 237, 380);
	puthz(161, 353, "重置", 24, 34, WHITE);
	
	setfillstyle(1, RED);
	bar(273, 350, 367, 380);
	puthz(291, 353, "开户", 24, 34, WHITE);
	
	setfillstyle(1, BLUE);
	bar(403, 350, 497, 380);
	puthz(421, 353, "返回", 24, 34, WHITE);
	
	setfillstyle(1,LIGHTGRAY);
	bar(0,480,640,456);
}

/**************************************
FUNCTION: draw_exist
DESCRIPRION: 绘制弹窗退出提示
INPUT: void
RETURN: 无
**************************************/
void draw_exist(void)
{
	setfillstyle(1,LIGHTGRAY);
	bar(165,155,475,315);
	setfillstyle(1,RED);
	bar(170,160,470,310);
	puthz(193,190,"您已开户，请点击确认按钮退出",16,18,YELLOW);
	setfillstyle(1,LIGHTRED);
	bar(280,240,360,280);
	puthz(295,249,"确认",24,26,BLUE);
}

/******************************************
FUNCTION: lightbutton_opa
DESCRIPRION: 开户界面的点亮
INPUT: x,y,x1,y1,color1,color2,flag
RETURN: 无
*******************************************/
void lightbutton_opa(int x, int y, int x1, int y1, int color1, int color2, int flag)
{
	clrmous(MouseX, MouseY);
	delay(10);
	setcolor(color1);
	setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
	setfillstyle(1, color2);
	bar(x, y, x1, y1);
	rectangle(x, y, x1, y1);	//画边框

	if (flag == 4)
	{
		puthz(161, 353, "重置", 24, 34, LIGHTGREEN);
	}
	else if (flag == 5)
	{
		puthz(291, 353, "开户", 24, 34, LIGHTRED);
	}
	else if (flag == 6)
	{
		puthz(421, 353, "返回", 24, 34, LIGHTBLUE);
	}
	else if (flag == 7)
	{
		puthz(295, 249, "确认", 24, 26, LIGHTRED);
	}
}

/**************************************************
FUNCTION: recoverbutton_opa
DESCRIPRION: 开户界面恢复 
INPUT: status
RETURN: 无
**************************************************/
void recoverbutton_opa(int status)
{
	delay(10);
	clrmous(MouseX, MouseY);
	setfillstyle(1, WHITE);
	switch (status)
	{
	case 1:
		bar(100, 120, 540, 145);
		break;
	case 2:
		bar(100, 190, 540, 215);
		break;
	case 3:
		bar(100, 260, 540, 285);
		break;
	case 4:
		setfillstyle(1, GREEN);
		bar(143, 350, 237, 380);
		puthz(161, 353, "重置", 24, 34, WHITE);
		break;
	case 5:
		setfillstyle(1, RED);
		bar(273, 350, 367, 380);
		puthz(291, 353, "开户", 24, 34, WHITE);
		break;
	case 6:
		setfillstyle(1, BLUE);
		bar(403, 350, 497, 380);
		puthz(421, 353, "返回", 24, 34, WHITE);
		break;
	case 7:
		setfillstyle(1,LIGHTRED);
		bar(280,240,360,280);
		puthz(295,249,"确认",24,26,BLUE);
		break;
	}
}

/******************************
FUNCTION: opaccount_complete
DESCRIPRION: 开户完成以及判定
INPUT: b,bp,bcp,u
RETURN: 无
******************************/
int opaccount_complete(char* b,char* bp,char* bcp,USER* u)
{
	int i;
	int state1 = 0;            //银行卡号输入格式是否正确的变量
	int state2 = 0;            //银行卡密码输入格式是否正确的变量
	int state3 = 0;            //确认密码输入格式是否正确的变量
	while(1)
	{
		setfillstyle(1,BLACK);
		bar(396,100,540,116);
		bar(443,170,540,186);
		bar(455,240,540,256);
		judge(b, &state1,490,100);
		judge(bp,&state2,490,170);
		judge(bcp, &state3, 490, 240);
		if(state1 == 1 || state2 == 1 || state3 == 1)
			break;
		else if(state1 == 0&&state2 == 0&&state3 == 0)  //全都有输入
		{
			if(judge_samecredit(b,1))          //已开户返回0，未开户返回1
			{
				state1 = checkright_bankcard(b);
				state2 = checkright_bankpassword(bp);
				state3 = checkright_bankconfirmedpassword(bp,bcp);
				if(state1 != 0 || state2 != 0 || state3 != 0)
				{
					break;
				}
				else if(state1 == 0 && state2 == 0 && state3 == 0)
				{
					if(input_database_bk(b,bp,u))
					{
						puthz(280,390,"开户成功！",16,17,RED);
						delay(2000);
						return 1;
					}
					else
					{
						printf("Something Wrong");
						delay(2000);
						exit(1);
					}
				}
			}
			else if(judge_samecredit(b,1))
			{
				state1 = checkright_bankcard(b);
				state2 = checkright_bankpassword(bp);
				state3 = checkright_bankconfirmedpassword(bp,bcp);
				break;
			}
		}
	}
	return 0;
}

/**********************************************
FUNCTION: checkright_bankcard
DESCRIPRION: 检查银行卡号是否规范,并给予提示
INPUT: b
RETURN: 规范则返回1，否则返回0
**********************************************/
int checkright_bankcard(char* str)
{
	if(strlen(str) != 19)
	{
		setcolor(RED);
		settextjustify(LEFT_TEXT,TOP_TEXT);
		settextstyle(1,HORIZ_DIR,2);
		puthz(396,100,"银行卡号应为",16,17,RED);
		outtextxy(497,94,"19");
		puthz(524,100,"位",16,17,RED);
		return 1;
	}  
	else
	{
		setcolor(BLUE);
		setlinestyle(SOLID_LINE,0,3);
		line(545,128,555,138);
		line(555,138,565,118);
		return 0;
	}
}

/*************************************************************
FUNCTION: checkright_bankpassword
DESCRIPRION: 检查银行卡密码是否规范，并给予提示
INPUT: bp
RETURN: 规范则返回1，否则返回0
*************************************************************/
int checkright_bankpassword(char* str)
{
	if(strlen(str) != 6)
	{
		setcolor(RED);
		settextjustify(LEFT_TEXT,TOP_TEXT);
		settextstyle(1,HORIZ_DIR,2);
		puthz(443,170,"密码应为",16,17,RED);
		outtextxy(512,164,"6");
		puthz(524,170,"位",16,17,RED);
		return 1;
	}
	else
	{
		setcolor(BLUE);
		setlinestyle(SOLID_LINE,0,3);
		line(545,198,555,208);
		line(555,208,565,188);
		return 0;
	}
}

/***************************************************
FUNCTION: checkright_bankconfirmedpassword
DESCRIPRION: 检查两次密码是否一致，并给予提示
INPUT: bp,bcp
RETURN: 规范则返回1，否则返回0
***************************************************/
int checkright_bankconfirmedpassword(char* str1,char* str2)
{
	if(strcmp(str1, str2) != 0)
	{
		puthz(455 ,240,"两次不一致",16,17,RED);
		return 1;
	}
	else 
	{
		setcolor(BLUE);
		setlinestyle(SOLID_LINE,0,3);
		line(545,268,555,278);
		line(555,278,565,258);
		return 0;
	}
}

/******************************************************
FUNCTION: judge_samecredit
DESCRIPRION: 检查该卡是否已被开户
INPUT: new_credit,flag
RETURN: 若已开户则返回0，否则返回1
*******************************************************/
int judge_samecredit(char* new_credit,int flag)
{
	int len;
	int i;
	FILE* fp;
	USER* u = NULL;
	if ((fp = fopen("database\\UserData.dat", "rb+")) == NULL)//建立数据库
	{
		printf("cannot open file UserData.dat");
		delay(3000);
		exit(1);
	}
	fseek(fp, 0, SEEK_END);
	len = ftell(fp) / sizeof(USER);
	for (i = 0; i < len; i++)
	{
		if ((u = (USER*)malloc(sizeof(USER))) == NULL)
		{
			printf("memoryallocation runs wrong in opaccount.c");
			delay(3000);
			exit(1);
		}
		fseek(fp, i * sizeof(USER), SEEK_SET);
		fread(u, sizeof(USER), 1, fp);
		if(strcmp(u->bankcard,new_credit) == 0)
		{
			switch(flag)
			{
			case 1:
				puthz(455,100,"该卡已开户",16,17,RED);
				break;
			case 2:
				break;
			default:
				//printf("something wrong");
				delay(3000);
				exit(1);
			}
			if (u != NULL)
			{
				free(u);
				u = NULL;
			}
			if(fclose(fp) != 0)
			{
				printf("\n cannot close Database.");
				delay(3000);
				exit(1);
			}
			return 0;
		}
		if(u != NULL)
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
		printf("\n cannot close Database.");
		delay(3000);
		exit(1);
	}
	return 1;
}

/**************************************************
FUNCTION: input_database_bk
DESCRIPRION: 将开户信息存入数据库中
INPUT: bankcard,bankcode,u
RETURN: 存入返回1，否则返回0
**************************************************/
int input_database_bk(char* bankcard,char* bankcode,USER* us)
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
			printf("memoryallocation runs wrong in opaccount.c");
			delay(3000);
			exit(1);
		}
		fseek(fp, i * sizeof(USER), SEEK_SET);//每次往后移一个用户
		fread(u, sizeof(USER), 1, fp);//将信息读入u
		if (strcmp(us->user, u->user) == 0)
		{
			strcpy(us->bankcard,bankcard);
			strcpy(us->bankpassword,bankcode);
			strcpy(u->bankcard,bankcard);
			strcpy(u->bankpassword,bankcode);
			fseek(fp, i * sizeof(USER), SEEK_SET);
			fwrite(u, sizeof(USER), 1, fp); //修改完后一定要记得写入数据 余额默认为0
			if (u != NULL)
			{
				free(u);
				u = NULL;
			}
			if (fclose(fp) != 0)
			{
				printf("\n cannot close Database");
				exit(1);
			}
			delay(2000);
			return 1;
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
	}
	return 0;
}