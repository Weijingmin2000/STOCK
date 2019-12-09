#include"main.h"
#include"public.h"

/*****************************************
main.c
COPYRIGHT:	weijingmin
FUNCTION:	main
ABSTRACT:
		A.skip among interface
DATE:2019/9/9
******************************************/


/*********************************************
FUNCTION:main
DESCRIPTION：主函数
INPUT:void
RETURN:无
***********************************************/
void main(void)
{
	int func = 0;
	int gd = VGA;
	int gm = VGAHI;
	USER u;
	memset(&u,'\0',sizeof(USER));
	initgraph(&gd, &gm, "C:\\STOCK\\BGI");
	welcome_page();	//欢迎界面

	mouseinit();
	//freopen("Database//Debug.txt", "w", stdout); //debug
	while (1)
	{
		switch (func)
		{
			case 0:		//初始化选择界面
			{
				initmenu_set(&func);    //会改变func的值进入
				memset(&u,'\0',sizeof(USER));
				break;                  //也即回到界面就成为无登录状态
			}
			case 1:		//退出按钮
				delay(1000);
				closegraph();
				exit(1);
			case 2:		//用户登录界面
				user_login(&func,&u);
				break;
			case 3:		//用户注册界面
				user_register(&func);
				break;
			case 4:		//忘记密码界面
				user_forgetpass(&func);
				break;
			case 5:		//管理员登录界面
				admin_login(&func);
				break;
			case 6:		//主界面主控函数
				control_main(&func,&u);
				break;
		}
	}
}