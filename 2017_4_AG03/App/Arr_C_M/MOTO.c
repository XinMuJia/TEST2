#include	"C_M.h"

void MT_HzGet(void);
u8 MT_Reckon(void);

extern	u16	It_Num;
extern 	u16 UpStartUSART[14];						//选择型号参数设置
u8 	MT_Reckon(void);
u16 MT_USART[2];
u16	MT_Hz[2];
FunctionalState  MOTO_H_Set=DISABLE;

/************************************
* 函数：u8	MT_Test(void)
* 功能：电机测量
* 入口参数：无
* 出口参数：正确Correct  			0x11
*						错误Error		 			0xAA
*************************************/
u8	MT_Test(void)
{
	u8	Zero;
	//MT_Hz[0]=11800;
	MotoStep(0x00);
	MT_SW(Bit_SET);													//电机测试打开
	Delay1ms(100);
	MT_HzGet();															//电机电感频率取样
	VH_EN(Bit_RESET);
	Zero=MT_Reckon();
	MT_USART[1]=Zero;
	MotoStep(0x06);
	Delay1ms(20);
	MotoStep(0x00);
	return Zero;
}


/************************************
* 函数：void Cap_HzGet(void)
* 功能：谐振频率取次
* 出口参数：无
*************************************/
void MOTO_HzGet(void)
{
	u32 Hz_Num=0;
	Delay1ms(10);
	TIM3 -> CNT	=	0;							//定时器重新计数
	It_Num=0;											//检测计数时间ti重新计数，1ms每轮
	while(It_Num<1030)
		{
			if(FLHzing()	==	Bit_SET)
			{
				Hz_Num++;
				while(FLHzing()	==	Bit_SET)
				{
					if(It_Num>1000)break;
				}
			}
		}
	if(Hz_Num > 1000)
	{
		MT_Hz[0]	= Hz_Num+150;
	}
	else
	{
		MT_Hz[0]	= 0;	
	}
}


/************************************
* 函数：u8 MT_Reckon(void)
* 功能：电感值计算并判断
* 入口参数：无
* 出口参数：	正确Correct  		0x11
*						错误Error		 		0xAA
*						未收录Failure		0x55
*************************************/
u8 MT_Reckon(void)
{
	float Zero;
	u32 	H;
	if(MT_Hz[1]>40)
	{
			Zero  =   (float)MT_Hz[0]/(float)MT_Hz[1];
			Zero  *=  Zero;
			if(Zero<1)
				{Zero  =  1-Zero;}
			else
				{Zero  =  Zero-1;}
			H   =   100*Zero*Hprice;			//精确到uH,四舍五入
			if((H%100)>=5)
			{
				H = H/100+1;
			}
			else
			{
				H = H/100;
			}
			MT_USART[0]	=	H;					//做不精确相对误差		
	}
	else
	{
		MT_USART[0]=0;
	}
	if(MOTO_H_Set==ENABLE)
		{
			if(MT_USART[0]	<	2200)
			{
				return	0x55;
			}
			else if(MT_USART[0]	>	3100)
			{
				return	0xAA;
			}
			else 
			{
				return 0x11;
			}
		}
	else
		{
	
			if(MT_USART[0]	<	3100)
			{
				return	0x55;
			}
			else if(MT_USART[0]	>	3800)
			{
				return	0xAA;
			}
			else 
			{
				return 0x11;
			}
		}
	
}



/************************************
* 函数：void MT_HzGet()
* 功能：谐振频率取次
* 入口参数：无
* 出口参数：无
*************************************/
void MT_HzGet(void)
{
	u32 Hz_Num=0;
	TIM3 -> CNT	=	0;							//定时器重新计数
	It_Num=0;											//检测计数时间ti重新计数，1ms每轮
	while(It_Num<1000)
	{
		if(FLHzing()	==	Bit_SET)
		{
			Hz_Num++;
			while(FLHzing()	==	Bit_SET)
			{
				if(It_Num>1000)break;
			}
		}
	}
	MT_Hz[1]	= Hz_Num;
}











