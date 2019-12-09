#ifndef _LGFUNC_H_
#define _LGFUNC_H_
#include"database.h"

//检查用户转账输入格式是否正确
void transfer_judge(char* m, char* p, USER* u);

//检查用户是否能转账，若能则转账
int judge_samecardpass(char* m, char* p, USER* u);

//管理员身份是否匹配
int admin_judge(char* admin, char* pass);

void inputadmin(char* id, int x1, int y1, int charnum, int color);
//x1,y1分别是账号，验证码等输入信息的左端点和上端点

void inputpassword(char* id, int x1, int y1, int charnum, int color);
//x1,y1分别是密码等隐藏信息输入的左端点和上端点

int login_complete(char* u, char* p);
//用来判断登录信息是否输入完全和正确

int forgetpass_complete(char* u, char* id, char* np, char* cp);
//检查是否可以更改新密码

int judge_rightpassword(char* name, char* pass);
//用户名和密码是否匹配

int name_id(char* name, char* id, char* pass, char* againpass);
//判断账号与身份证是否匹配，并判断两次输入的密码是否相同，若匹配且相同则写入新密码。

int register_complete(char* u, char* p, char* cp, char* id, char* vc, char* rdvc,char* bank,char* bankcode);
//用来判断注册信息是否输入完全和正确

void judge(char* str, int* state, int x, int y); 
//判断注册的状态

void recoverhz(int x, int y, int color);
//覆盖之前输出的汉字

int judge_sameuser(char* new_user,int flag);
//判断账户是否出现重名,以及找回密码时判断用户名是否存在     1为

int checkright_user(char* str, int x, int y);
//检查用户名是否大于6位

int checkright_password(char* str, int x, int y);
//检查用户名是否大于6位

int checkright_confirmedpassword(char* str1, char* str2, int x, int y);
//检查确认密码是否一致

int checkright_ID(char* str, int x, int y, int flag);
//检查身份证位数与格式

int checkright_verificationcode(char* str1, char* str2, int x, int y);
//检查验证码是否一致

void input_database(char* name, char* id, char* code,char* bank,char* bankcode);
//把用户注册信息写入文件

int input_uinfo(USER* us);
//登陆后把用户信息读入u

#endif