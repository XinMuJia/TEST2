/*******************************************************
*********************电机1，光耦旁路电容测试***********/

/***************软件版本2R_V4_0**************************
*@MOTO.C
*@使能MT_SW端口，开始测试电机1
*@电机电感测试：
							使能MT_SW，读取FLHzing()谐振频率;
							计算转换测试结果L
							与UpStartUSART[9]，+-对比返回测试结果
							并记录MT_USART[2];
*									 	MOTO_L： 				MT_USART[0]
*								 		结果:						MT_USART[1]					
*										正确Correct  		 		0x11
*										错误Error		 				0xAA
*										未收录Failure				0x55
*******************************************************/


/*****************************************************
*@Cap.C
*@使能Cap_SW端口，读取CLHzing()光耦旁路电容谐振频率;
*@光耦旁路电容测试：
*							使能MT_SW，读取谐振频率;
*							判断是否正在校准，
*							校准频率： 				Cap_Hz[0]
*							测试频率:					Cap_HZ[1]
*							计算Cap_Num[]并转换测试结果Cap_Num[2]
*							与UpStartUSART[4]，+-10%对比返回测试结果
*							并记录Cap_USART[2]:
*										计算Cap:					Cap_USART[0]				
*								 		结果:							Cap_USART[1]					
*										正确Correct  		 		0x11
*										错误Error		 				0xAA
*										未收录Failure				0x55	
*******************************************************/


#ifndef __C_M_H_
#define __C_M_H_
#include	"Config.h"

#define Cprice	2000			//基准电容的值pF
#define	Accuracy	10			//1-99  数字越大报错精准越高
#define Hprice	1000			//基准电感的值uH

u8	MT_Test(void);
u8	MT_Test(void);
u8 	Cap_Test(FunctionalState Test);
void MOTO_HzGet(void);
#endif /* __C_M_H_ */




