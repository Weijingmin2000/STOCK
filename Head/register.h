#ifndef _REGISTER_H_
#define _REGISTER_H_


void user_register(int* func);
void random_str(char* str);
void draw_signin(char* str);
void lightbutton_register(int x, int y, int x1, int y1, int color1, int color2, int flag); //color1为边框色,color2为填充色
void recoverbutton_register(int status);

#endif