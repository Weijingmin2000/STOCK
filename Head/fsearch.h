#ifndef _FSEARCH_H_
#define _FSEARCH_H_

#include"database.h"

void F_search(int* count, int opt[][2], int* sto_list);//读取文件
int fundsearch(int* count, int opt[][2], FUND* s);//基本面查找
void refresh_progress2(int progress, int color);//更新进度条

#endif