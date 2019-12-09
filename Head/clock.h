#ifndef _CLOCK_H_
#define _CLOCK_H_

void clock_draw(void); //画钟表
void clock_change(int a, int b, int c);//a为半径，b为时分秒，c为换算机制
void clock_show(void);//显示时钟
void clock_get(unsigned char* h, unsigned char* m);//获得时间并更新

#endif
