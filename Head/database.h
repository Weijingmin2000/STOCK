#ifndef _DATA_H_
#define _DATA_H_
typedef struct userinfo
{
	char user[15];           //6-12位，用户名
	char ID[20];             //18位，身份证
	char password[20];       //8-16位，密码
	char bankcard[21];       //19位，银行卡
	char bankpassword[8];    //6位,银行卡密码
	char balance[15];        //银行卡余额
}USER;	//封装用户信息

typedef struct admininfo
{
	char user[15];//6-12位，管理员账号
	char password[10];//8位密码
}ADMIN;


typedef struct				  //每只股票的基本信息
{
	char code[7];	//股票代码
	char name[10];	//股票名称
	char dat1[6];	//涨幅
	char dat2[6];	//现价
	char dat3[6];	//涨跌
	char dat4[6];	//买价
	char dat5[6];	//卖价
	char dat6[8];	//总量
	char dat7[6];	//现量
	char dat8[6];	//涨速
	char dat9[6];	//换手
	char dat10[6];	//今开
}Stodata;

typedef struct stockday_char
{
	//日期	    开盘	    最高	    最低	    收盘	    成交量	    成交额
	char date[12];//日期
	char open[6];//开盘
	char high[6];//最高
	char low[6];//最低
	char close[6];//收盘
	char VOL[10];//成交量 volume
	char VOT[15];//成交额 volume of transaction
}STODAYCHAR;//某一天股票的字符串信息


typedef struct stockday_float
{
	//日期	    开盘	    最高	    最低	    收盘	    成交量	    成交额
	int Year;//年
	int Month;//月
	int Day;//日
	float Open;//开盘
	float High;//最高
	float Low;//最低
	float Close;//收盘
	float VOL;//成交量
	float VOT;//成交额
}STODAYFLOAT;//某一天股票转化后的数字信息


typedef struct techline_major
{
	float first[90];
	float second[90];
	float third[90];
	float forth[90];
}UPLINE;//行情主图的数据


typedef struct techline_vice
{
	float first[90];
	float second[90];
	float third[90];
}DOWNLINE;//行情副图的数据


typedef struct priceinfo
{
	float Open[90];
	float Close[90];
	float High[90];
	float Low[90];
	float VOL[90];
}PRICEINFO;//存放某一支90天价格信息的结构体，用于搜索

typedef struct fundamental
{
	char PE[7];//市盈率 price earning ratio
	char LEV[7];//资产负债率 asset-liability ratio
	char PB[7];//市净率 price/book value ratio
	char NP[7];//净利润率 net profit rate
	char DE[7];//权益比 debt-equity ratio
}FUND;//存放某一支股票的基本面信息

typedef struct
{
	char sto_date[12];    //成交日期
	char sto_code[7];     //证券代码
	char sto_name[10];    //证券名称
	char sto_flag[2];  	  //操作标记
	char sto_num[10];     //成交数量
	char sto_price[7];    //成交均价
	char sto_amount[20];  //成交金额
}Hstdata;//历史成交


typedef struct
{
	char sto_date[11];       //持仓日期
	char sto_code[7];        //证券代码
	char sto_name[10];       //证券名称
	char sto_num[10];        //持股数
	char sto_price[7];       //成本价num
	char sto_day[4];         //持仓天数
}Hold;//持仓
	

#endif