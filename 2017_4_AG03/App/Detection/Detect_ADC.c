/*******************************************************
*************VH电压,热敏值,红外集电极电压取样***********
***************软件版本2R_V4_0**************************
*@u8	VH_Test(void);
*@读取ADC取样值，并计算
*@VH电压：
*				读取ADC_Num[1]的值并进行电压换算
*				储存在Detect_USART[0]与UpStartUSART[11]进行判断
*							 			返回结果:									
*										正确Correct  		 		0x11
*										大于				 				0xAA
*										小于								0x55	
*******************************************************/
/******************************************************
*@u8	TM_Test(void);
*@读取ADC取样值，并计算
*@TM,TPH3950热敏电压：
*				读取ADC_Num[5]的值并进行电压换算
*				储存在Detect_USART[1]与正常温度进行判断5~50
*							 			返回结果:									
*										正确		  		 		0x11
*										错误							0xAA
*										温度警报					0x55
*******************************************************/
/******************************************************
*@u8	RAY_Test(void);
*@读取红外集电极取样值
*@CE电压：
*				读取ADC_Num[2]的值并进行电压换算
*				储存在Detect_USART[2]与设定值进行判断小于0.8V
*							 			返回结果:									
*										正确Correct  		 		0x11
*										错误Error		 				0xAA
*******************************************************/

/****************************************
u16	ADC_Num[3]:									//ADC缓存
*							ADC_Num[0]				//VH电压取样ADC
*							ADC_Num[1]				//红外光耦电压取样ADC
*							ADC_Num[2]				//TPH热敏电压取样ADC
****************************************/


#include	"Detect_ADC.h"


extern u16	ADC_Num[3];								//ADC缓存
extern u16 	UpStartUSART[14];						//选择型号参数设置

u16	Detect_USART[6];
/****************************
*	Detect_USART[0]		VH电压值
*	Detect_USART[1]		VH测试结果	
*	Detect_USART[2]		TM热敏值温度
*	Detect_USART[3]		TM测试结果
*	Detect_USART[4]		光耦集电极电压
*	Detect_USART[5]		光耦运行结果
****************************/
u16 	PAPER_SETD=500;					//默认光耦下限启动参数	
u16 	PAPER_SETU=2500;					//默认光耦上限启动参数
/************************************
* 函数：u8	VH_Test(void)
* 功能：VH电压计算
* 入口参数：无
* 出口参数：正确Correct  		0x11
*								大于				0xAA
*								小于				0x55	
*************************************/
u8	VH_Test(void)
{
	u16	Zero;
	u16	u=8000;
	u = 9000;
	VH_EN(Bit_SET);
	Delay1ms(100);
	Zero 	=	ADC_Num[0]*4;
	Zero	=	((((float)Zero/4096)*3.3)*1000);				//*1000,单位 mV	
	Detect_USART[0]=Zero;
	if((Zero	<	(u-1000)))							
	{
		Detect_USART[1]=0x55;
		return	0x55;
	}
	else if (Zero	>	(u+1000))
	{
		Detect_USART[1]=0xAA;
		return	0xAA;
	}
	else 
	{
		Detect_USART[1]=0x11;
		return 0x11;
	}
}


/************************************
* 函数：u8	TM_Test(void)
* 功能：热敏电压计算
* 入口参数：无
* 出口参数：		正确		  		 		0x11
*								错误							0xAA
*								温度警报					0x55
*公式：T1 =ln(Rt/R)/B+1/T2
*************************************/
//u16 Rp	=		UpStartUSART[8];       	  //320;						//常温下ADC取样
const float T2 =  (273.15+25.0);	//T2
const float Bx =  3950.0;					//B
const float Ka =  273.15;
u8	TM_Test(void)
{
	float Rp	=		UpStartUSART[8];      		//常温下ADC取样
	float Rt;
	float temp;
	Delay1ms(20);
	Rt = ADC_Num[2];
	//like this R=5000, T2=273.15+25,B=3470, RT=5000*EXP(3470*(1/T1-1/(273.15+25)),  
	temp = Rt/Rp;
	temp = log(temp);				//ln(Rt/Rp)
	temp/= Bx;								//ln(Rt/Rp)/B
	temp+= (1/T2);
	temp = 1/(temp);
	temp-= Ka;
	
	Detect_USART[2]	=	temp;
	if(temp	<	15 || temp	>	35)
	{
		Detect_USART[3]	=	0x55;
		return	0x55;
	}
	else 
	{
		Detect_USART[3]	= 0x11;
		return 0x11;
	}
}




/************************************
* 函数：u8	RAY_Test(void)
* 功能：红外集电极计算
* 入口参数：无
* 出口参数：默认启动 				0x11
*						默认待机				0x55
*						错误Error				0xAA
*************************************/
u8	RAY_Test(void)
{
	u16	Zero;
	Delay1ms(20);
	Zero	=	ADC_Num[1]*4;
	Zero	=	(((float)Zero/4096)*3.3)*1000;				//*1000,单位 mV
	Detect_USART[4]=Zero;
	if(Zero	> PAPER_SETU &&	Zero	< 3300)
	{
		Detect_USART[5]=0x55;
		return 0x55;
	}
	else if(Zero	<	PAPER_SETD	&&	Zero	>	32)
	{
		Detect_USART[5]=0x11;
		return	0x11;
	}
	else
	{
		Detect_USART[5]=0xAA;
		return	0xAA;
	}
}





