#include"public.h"
#include"welcome.h"


/*****************************************
welcome.c
COPYRIGHT: 	dengshumin
FUNCTION:   draw welcome interface
DATE:       2019/8/31
******************************************/

void welcome_page(void)	//欢迎界面
{
	int i;
	setbkcolor(RED);
	setcolor(LIGHTGRAY);
	setlinestyle(SOLID_LINE,0,1);
	for(i=0;i<2;i++)
	{
		line(0,160*(i+1),640,160*(i+1));
	}
	for(i=0;i<3;i++)
	{
		line(160*(i+1),0,160*(i+1),480);
	}
	puthz(106,40,"欢迎进入证券模拟投资系统",32,36,WHITE);
	puthz(106-1,40-1,"欢迎进入证券模拟投资系统",32,36,LIGHTGRAY);
	setcolor(WHITE);
	settextjustify(LEFT_TEXT, TOP_TEXT);
	settextstyle(TRIPLEX_FONT, HORIZ_DIR,2);
	outtextxy(200,40+16+36,"Made by Automation 1801 WJM and DSM");
	settextjustify(LEFT_TEXT,BOTTOM_TEXT);
	settextstyle(SANS_SERIF_FONT, HORIZ_DIR,1);
	outtextxy(0,150,"8.12");
	outtextxy(0,310,"7.89");

	bar(25,480-30-15,55,480-30-5);

	bar(85,480-110,95,480-20-60);
	bar(75,480-20-60,105,480-40);
	bar(85,480-40,95,480-20);

	bar(125,480-20-40-100,155,480-110);

	line(175,480-95,205,480-95);
	line(190,480-165,190,480-70);

	bar(225,480-180,255,480-100);
	bar(235,480-200,245,480-80);

	bar(275,480-190,305,480-200);
	bar(285,480-200,295,480-250);

	bar(325,480-300,355,480-295);

	bar(375,480-180,405,480-130);
	bar(385,480-240,395,480-180);

	bar(435,480-140,445,480-145);
	bar(425,480-140,455,480-110);
	bar(435,480-110,445,480-80);

	bar(475,480-20-25,505,480-20);

	bar(535,480-20-65,545,480-105);
	bar(525,480-20-58,555,480-20-65);
	bar(535,480-20-58,545,480-20-45);

	bar(575,480-95,605,480-110);
	bar(585,480-110,595,480-160);

	setfillstyle(1,RED);
	bar(85,410,95,430);
	
	bar(235,315,245,365);
	
	bar(435,348,445,362);
	
	delay(2500);
}