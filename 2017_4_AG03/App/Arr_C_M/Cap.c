/***************软件版本2R_V4_0**************************
*@Cap.C
*@使能Cap_SW端口，光耦旁路电容
*@光耦旁路电容测试：
*							检测UpStartUSART[5]	旁路电容检测使能
*							使能Cap_SW，20*10ms读取谐振频率;
*							判断是否正在校准，
*							校准频率： 				Cap_Hz[0]
*							测试频率:					Cap_HZ[1]
*							计算Cap_USART[]并转换测试结果Cap_USART[2]
*							与UpStartUSART[4]，+-10%对比返回测试结果
*							并记录Cap_USART[2]:
*										计算Cap:						Cap_USART[0]				
*								 		结果:								Cap_USART[1]					
*										正确Correct  		 		0x11
*										错误Error		 				0xAA
*										未收录Failure				0x55	
*******************************************************/

/******************************************************
*@UpStartUSART[13]
				UpStartUSART[4]			光耦旁路电容值,单位：pF
				UpStartUSART[5]			旁路电容检测使能
*@It中断：
			Cap_It_Num						中断计数
*******************************************************/
/******************************************************
*@输出外部调用
					Cap_USART[2]
					Cap_It_Num
*******************************************************/


#include	"C_M.h"
extern u16 UpStartUSART[14];
extern	u16	It_Num;

void Cap_HzGet(FunctionalState Test);
u8 Cap_Reckon(void);

u16 Cap_USART[2];
u16	Cap_Hz[2];



/************************************
* 函数：u8 	Cap_Test(FunctionalState Test)
* 功能：光耦旁路电容测试
* 入口参数：ENABLE:测试电容值；DISABLE:进行校准
* 出口参数：	正确Correct  		0x11
*						错误Error		 		0xAA
*						未收录Failure		0x55
*************************************/
u8 	Cap_Test(FunctionalState Test)
{
	u8 Zero;
	if(UpStartUSART[5]!=ENABLE)
	{
		Cap_USART[0]	= 0;
		Cap_USART[1]	= 0x00;	
		MT_SW(Bit_RESET);	
		return 0x00;
	}
	Cap_HzGet(Test);
	MT_SW(Bit_RESET);	
	Zero=Cap_Reckon();
	Cap_USART[1]	=	Zero;
	return Zero;
}


/************************************
* 函数：void Cap_HzGet(void)
* 功能：谐振频率取次
* 出口参数：无
*************************************/
void Cap_HzGet(FunctionalState Test)
{
	u32 Hz_Num=0;
	Test==ENABLE?(__nop()):(MT_SW(Bit_RESET));
	Delay1ms(10);
	TIM3 -> CNT	=	0;							//定时器重新计数
	It_Num=0;											//检测计数时间ti重新计数，1ms每轮
	while(It_Num<500)
		{
			if(FCHzing()	==	Bit_SET)
			{
				Hz_Num++;
				while(FCHzing()	==	Bit_SET)
				{
					if(It_Num>500)break;
				}
			}
		}
	if(Test == ENABLE)
	{
		Cap_Hz[1]	= Hz_Num;
	}
	else if(Test	==	DISABLE)
	{
		Cap_Hz[1]	=	0;
		Cap_Hz[0]	= Hz_Num;	
	}
}


/************************************
* 函数：u8 Cap_Reckon(void)
* 功能：电容值计算并判断
* 入口参数：无
* 出口参数：	正确Correct  		0x11
*						错误Error		 		0xAA
*						未收录Failure		0x55
*************************************/
u8 Cap_Reckon(void)
{
	float Zero;
	u32 C;
	//Cap_Hz[0]+=5;
	if(Cap_Hz[1]>40)
		{
			Zero  =   (float)Cap_Hz[0]/(float)Cap_Hz[1];
			Zero  *=  Zero;
			if(Zero<1)
				{Zero  =  1-Zero;}
			else
				{Zero  =  Zero-1;}
			C   =   100*Zero*Cprice;			//精确到pF,四舍五入
			if((C%100)>=5)
			{
				C = C/100+1;
			}
			else
			{
				C = C/100;
			}
		}
	Cap_USART[0]	=	C;					//做不精确相对误差	
	Zero	=	UpStartUSART[4];
	if(Cap_USART[0]	<	(Zero-(Zero/Accuracy)))
	{
		return	0x55;
	}
	else if(Cap_USART[0]	>	(Zero+Zero))
	{
		return	0xAA;
	}
	else 
	{
		return 0x11;
	}
}






