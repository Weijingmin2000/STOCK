#ifndef _TSEARCH_H_
#define _TSEARCH_H_

//技术面选股搜索函数
void T_search(int opt[][6], int* sto_list, int progress_color);

//读取一只股票90天的Open,Close,High,Low,VOL信息
void sto_90days2float(PRICEINFO* pinfo, int sto_num);

//技术面搜索
int techsearch(int opt[][6], PRICEINFO* pinfo);

//更新进度条
void refresh_progress(int progress, int color);

#endif
