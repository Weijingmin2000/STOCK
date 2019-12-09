#ifndef _FUNDAM_H
#define	_FUNDAM_H

void fundam_control(Ctr* cinfo,USER* u,int* page, int* sto_list);//基本面控制函数
void fundamental_draw(void);
void move_circle(int* MouseX,int* MouseY,int* press,int* min,int* max,int n);
void move_circle2(int *MouseX,int *MouseY,int *press,int *min,int *max,int n);
void fundam_judge(int *count,int n);
void lightbutton_fundamental(void);
void recoverbutton_fundamental(void);

#endif