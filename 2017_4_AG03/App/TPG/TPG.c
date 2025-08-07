/***************软件版本2R_V4_0**************************
*@TPG.C
*@使用引脚
*              说明: 
*              ----------------------------------------
*              GND  					电源地							
*              VCC  					3.3v电源						
*							 VH						 	4.2V-8V电源					
*              PAPER_PRINT  	PE5						
*              TPH_DI 			  PE10						
*              TPH_CLK			  		PE11
*              STB_5				  PE12
*              STB_4/6					PE13
*              STB_3				  PE14
*              STB_2				  PE15
*              STB_1				  PB10
*							 TPH_LAT				PB11
*******************************************************/


#include	"TPG.h"


extern	u16	It_Num;
extern	u16	UpStartUSART[14];
extern	FunctionalState	TPH_EN;
extern	FunctionalState	MOTO_EN;
extern	BitAction	STB_ON,STB_OFF;
extern 	FunctionalState  MOTO_EN_It;
extern 	FunctionalState  TPH_EN_It;
extern 	FunctionalState 	MOTO_2Esc;
//u8	Step[4] = {0x05,0x09,0x0a,0x06}; 				//电机走步顺序
u8	Step[8] = {0x05,0x01,0x09,0x08,0x0a,0x02,0x06,0x04}; 				//电机走步顺序
u8	res;
u16	kos;
void	TPH_Print1(u16 x);

/******************************************************************************
*	函数说明：检查行打印字符是否溢出
*	入口数据：x  当前已需打印点行数
*	          b  行尾预留白
*	返回值：  ENABLE		有预留
*						DISABLE		已满，另起一行
******************************************************************************/
FunctionalState TPH_Column(u16 x,u16 b)
{
	if((x+b)	<	UpStartUSART[6])
	{
		return	ENABLE;
	}
	else
	{
		return	DISABLE;
	}
	
}


/******************************************************************************
*	函数说明：TPH写入一个位数据
*	入口数据：dat    数据
*	返回值：  无
******************************************************************************/
void TPH_WR_Byte(u8 dat)
{
		if(dat&0x88)							//dat高位写入
		{
			//printf("1");
			TPH_HDI(Bit_SET);
	//		Delay1ns(2);
    }
		
		else
		{
			//printf("0");
			TPH_HDI(Bit_RESET);
		//	Delay1ns(2);
    }
		
		TPH_SCLK(Bit_SET);;				//时钟信号跳转
	//	Delay1ns(2);
		TPH_SCLK(Bit_RESET);			
		//Delay1ns(3);
}



/******************************************************************************
*	函数说明：TPH打印字符函数
*	入口数据：x		 起始坐标
*						o		 打印高度
*           chr  写入的字符
*           size 字符的大小
* 返回值：  无
******************************************************************************/
void	TPH_PrintChar(u16 x,u8 o,u8 chr,u8 size)
{
	u8 c,j,k;
	u8 data1,DATA1;
	c=chr-' ';																				//得到偏移后的值
	o*=size/16;																				//计算当前打印的行数
	for(j=0;j	<	size/16;j++)													//当前字符打印对需要的字节数
	{	
		if(size==16)data1=TPH_F8X16[c][o];							//查找字库
		else if(size==32)data1=TPH_F12X24[c][o+j];
		else return;																		//未找到相应字符大小直接返回
	//	j==1?(k=4):(k=0);
		for(k=0;k<8;k++)																//输入一个字节数据
		{
			DATA1=0x00;
			if(data1	&	0x80)															//高位在前
			{
				kos++;
				DATA1=0x88;
			}
			TPH_WR_Byte(DATA1);
			data1 <<= 1;
		}
	}
}

extern 	u8 TPH_LAN_TEST;
/******************************************************************************
*	函数说明：TPH打印字符串
*	入口数据：x  		起始坐标
            *dp 	写入的字符串
            size	字符的大小
*	返回值：  无
******************************************************************************/
void 	TPH_PrintString(u16 x,char *dp,u8 size)
{
	u8	o,k,Ta;
	u8	Wide,Tall,Column;
	u16	z;
	u16	Header;
	FunctionalState	loop;
	kos=0;
	Ta=size;
	Ta>16?(Ta=32):(Ta=16);
	Wide=16;																	//ASCII 字符宽														
	Tall=size;																		//ASCII 字符高
	Header=x;																			//前留白
	Column=0;																			//字符串所打印行数
	TPH_EN	=	ENABLE;
	MOTO_EN	=	ENABLE;	
	do
	{
		loop=	DISABLE;															//字符串打印循环初始化
		for(o=0;o<Tall;o++)													//打印字符高度循环
		{
			k	=	Column;																//现在打印的字符寻址位
			z	=	Header;																//行打印点数初始							
			TPH_Space(Header);
			while(dp[k]!='\0')												
			{
				TPH_PrintChar(Header,o,dp[k],Ta);			
				k++;																		//字符寻址位计数
				z+=Wide;																//已打印行点阵数计数
				if(kos>=200)														//当打印有效位等于96点 或者 数据传输完毕时 进行一次加热
				{
						TPH_Print1(384-z);
						TPH_Space(z);
						kos=0;
				}
				if(TPH_Column(z,0) !=	ENABLE)						
				{
					loop	=	ENABLE;												//继续循环
					break;
				}
			}
				if(TPH_LAN_TEST	==	1)
				{
					TPH_Space(384-z);		
					TPH_HLAT(Bit_SET); 											//LAN测试
				}
				else
				{
					TPH_Space(384-z);		
					TPH_HLAT(Bit_RESET);																	//数据锁存
					Delay1us(1);
					TPH_HLAT(Bit_SET); 
				}
			while(MOTO_EN_It	==	ENABLE && MOTO_EN==ENABLE);
				
			TPH_PrintNum(0);
			kos=0;
		}
		Column	=	k;																//寻址位缓存
	}while(loop	==	ENABLE && dp[k]!='\0');				//字符打印完毕，结束循环
	
}

//单字符打印
// void TPH_PrintImage_Byte(u8 P_Byte,u16 rows, u16 cols)
// {
//     u16 row;
//     u8 dataByte;
//     u8 DATA1;
//     u8 k, m;
// 	u16 index;

// 	cols = cols / 8;  // 每行384点，每字节8点，所以每行需要48字节 
// 	TPH_EN = ENABLE;
//     MOTO_EN = ENABLE;




//     u8 center = (384 - cols * 8) / 2;  // 每行384点，图像宽
// }

void TPH_PrintImage_m2(const unsigned char *image,u16 rows, u16 cols)
{
    u16 row;
    u8 dataByte;
    u8 DATA1;
    u8 k, m;
	u16 index;

	cols = cols / 8;  // 每行384点，每字节8点，所以每行需要48字节 

    u8 center = (384 - cols * 8) / 2;  // 每行384点，图像宽度为12字节，每字节8点，所以中心位置为384 - 12 * 8
    TPH_EN = ENABLE;
    MOTO_EN = ENABLE;
    
    // 图像数据大小为9600字节
    // 每行384点，每字节8点，所以每行需要48字节
    
    for(row = 0; row < rows; row++) {
        // 等待电机中断准备好
        do {
			TPH_Space(center);  // 每行打印完后留白
            // 打印一行数据（384点）
            for(m = 0; m < cols; m++) {  // 48字节 = 384点

				// 计算一维数组中的索引
                index = row * cols + m;
                // 从图像数组中获取一个字节数据
                dataByte = image[index];
                
                // 将一个字节分解为8位并发送到TPH
                for(k = 0; k < 8; k++) {
                    DATA1 = 0x00;
                    if(dataByte & 0x80) {  // 高位在前
                        DATA1 = 0x88;
                    }
                    TPH_WR_Byte(DATA1);
					dataByte <<= 1;
                }
            }

            TPH_Space(center);  // 每行打印完后留白
            // 数据锁存
            TPH_HLAT(Bit_RESET);
            Delay1us(1);
            TPH_HLAT(Bit_SET);
            
        } while(MOTO_EN_It == ENABLE && MOTO_EN == ENABLE);
        
        // 触发下一行打印
        TPH_PrintNum(0);
    }
}


u8	data3=0x0F;
//打印黑块
void TPH_Loop1(void)
{
	u8 DATA1;
	u8 data1;
	u8	k,m;
	TPH_EN	=	ENABLE;
	MOTO_EN	=	ENABLE;
	do
	{
	for(m=16;m>0;m--)
		{
			data1=0XFF;
			for(k=0;k<8;k++)																//输入一个字节数据
			{
				DATA1=0x00;
				if(data1	&	0x80)															//高位在前
				{
					DATA1=0x88;
				}
				TPH_WR_Byte(DATA1);
				data1 <<= 1;
			}
		}
		TPH_Space(256);
		TPH_HLAT(Bit_RESET);																	//数据锁存
		Delay1us(1);
		TPH_HLAT(Bit_SET); 
		
		/*
		for(m=17;m>0;m--)
		{
			data1=0XFF;
			for(k=0;k<8;k++)																//输入一个字节数据
			{
				DATA1=0x00;
				if(data1	&	0x80)															//高位在前
				{
					DATA1=0x88;
				}
				TPH_WR_Byte(DATA1);
				data1 <<= 1;
			}
		}
		TPH_Space(248);
		TPH_HLAT(Bit_RESET);																	//数据锁存
		Delay1us(1);
		TPH_HLAT(Bit_SET); 
		
		*/
		
	}while(MOTO_EN_It	==	ENABLE && MOTO_EN	==	ENABLE);

		TPH_PrintNum(0);
}


//打印黑块
void TPH_Loop2(void)
{
u8 DATA1;
	u8 data1;
	u8	k,m;
	TPH_EN	=	ENABLE;
	MOTO_EN	=	ENABLE;
	do
	{
	TPH_Space(128);
	for(m=16;m>0;m--)
		{
			data1=0XFF;
			for(k=0;k<8;k++)																//输入一个字节数据
			{
				DATA1=0x00;
				if(data1	&	0x80)															//高位在前
				{
					DATA1=0x88;
				}
				TPH_WR_Byte(DATA1);
				data1 <<= 1;
			}
		}
		TPH_Space(128);
		TPH_HLAT(Bit_RESET);																	//数据锁存
		Delay1us(1);
		TPH_HLAT(Bit_SET); 
	}while(MOTO_EN_It	==	ENABLE && MOTO_EN	==	ENABLE);

		TPH_PrintNum(0);
}


//打印黑块
void TPH_Loop3(void)
{
	u8 DATA1;
	u8 data1;
	u8	k,m;
	TPH_EN	=	ENABLE;
	MOTO_EN	=	ENABLE;
	do
	{
	TPH_Space(256);
	for(m=16;m>0;m--)
		{
			data1=0XFF;
			for(k=0;k<8;k++)																//输入一个字节数据
			{
				DATA1=0x00;
				if(data1	&	0x80)															//高位在前
				{
					DATA1=0x88;
				}
				TPH_WR_Byte(DATA1);
				data1 <<= 1;
			}
		}
		TPH_HLAT(Bit_RESET);																	//数据锁存
		Delay1us(1);
		TPH_HLAT(Bit_SET); 
		
	}while(MOTO_EN_It	==	ENABLE && MOTO_EN	==	ENABLE);

		TPH_PrintNum(0);
}


//打印黑块
void TPH_Loop4(void)
{
	u8 DATA1;
	u8 data1,data2=0xFF;
	u8	k,m,s;
	TPH_EN	=	ENABLE;
	MOTO_EN	=	ENABLE;	
	//TPH_Space(288);
	
	TPH_Space(192);
	for(m=12;m>0;m--)
		{
			data1=data3;
			for(k=0;k<8;k++)																//输入一个字节数据
			{
				DATA1=0x00;
				if(data1	&	0x80)															//高位在前
				{
					DATA1=0x88;
				}
				TPH_WR_Byte(DATA1);
				data1 <<= 1;
			}
		}
		s=data3&0x01;
		data3 >>= 1;	
		s==1?data3|=0x80:data3;
		
		TPH_Space(192);
	
		for(m=12;m>0;m--)
		{
			data1=data2;
			for(k=8;k>0;k--)																//输入一个字节数据
			{
				DATA1=0x00;
				if(data1	&	0x80)															//高位在前
				{
					DATA1=0x88;
				}
				TPH_WR_Byte(DATA1);
				data1 <<= 1;
			}
		}
		TPH_Space(0);
		TPH_HLAT(Bit_RESET);																	//数据锁存
		Delay1us(1);
		TPH_HLAT(Bit_SET); 
		TPH_PrintNum(0);
}





/*

void TPH_Loop0(void)
{
	u8 DATA1;
	u8 data1,data2=0xFF;
	u8	k,m,s;
	TPH_EN	=	ENABLE;
	MOTO_EN	=	ENABLE;
	do
	{
	for(m=18;m>0;m--)
		{
			data1=0X0F;
			for(k=0;k<8;k++)																//输入一个字节数据
			{
				DATA1=0x00;
				if(data1	&	0x80)															//高位在前
				{
					DATA1=0x88;
				}
				TPH_WR_Byte(DATA1);
				data1 <<= 1;
			}
		}
		TPH_Space(240);
		TPH_HLAT(Bit_RESET);																	//数据锁存
		Delay1us(1);
		TPH_HLAT(Bit_SET); 
		
		
		for(m=18;m>0;m--)
		{
			data1=0XF0;
			for(k=0;k<8;k++)																//输入一个字节数据
			{
				DATA1=0x00;
				if(data1	&	0x80)															//高位在前
				{
					DATA1=0x88;
				}
				TPH_WR_Byte(DATA1);
				data1 <<= 1;
			}
		}
		TPH_Space(240);
		TPH_HLAT(Bit_RESET);																	//数据锁存
		Delay1us(1);
		TPH_HLAT(Bit_SET); 
		
		
		
	}while(MOTO_EN_It	==	ENABLE && MOTO_EN	==	ENABLE);

		TPH_PrintNum(0);
}
*/


void	TPH_Print1(u16 x)
{
	TPH_Space(x);																	//补足剩下所对齐的点阵数
	while(TPH_EN_It != DISABLE);
	TPH_HLAT(Bit_RESET);																	//数据锁存
	Delay1us(1);
	TPH_HLAT(Bit_SET); 
	It_Num=0;
	TPH_EN_It	= ENABLE;
}








//?ò??????

//uint8_t TPH_Buffer[216];

void TPH_ShowPicture(void)
{
	u16 i,j,s,c,m;
	u8 DATA1,DATA2,data1,data2,ko;
	MOTO_EN_It	=	ENABLE;
	c=UpStartUSART[6]/2;
	m=UpStartUSART[1];
	UpStartUSART[1]=1;
	for(s=0;s<1;s++)
	{
//		SPI_FLASH_BufferRead(TPH_Buffer,0x00000+(s*216)+21,216);
		for(i=0;i<16;i++)
		{
			MOTO_EN	=	ENABLE;
			TPH_EN_It = DISABLE;
			ko=0;
			for(j=0;j<c;j++)
					{
							DATA1=0x00;
							data1	=	0xFF>>4;
							data1	=	data1&0x0F;
							data1	=	data1-i;
							if(data1 > 0x00 && data1 < 16)
							{
								ko=1;
								DATA1=0x88;
							}
							TPH_WR_Byte(DATA1);

							DATA2=0x00;
							data2	=	0xFF&0x0F;
							data2	=	data2-i;
							if(data2 > 0x00 && data2 < 16)
							{
								ko=1;
								DATA2=0x88;
							}
							TPH_WR_Byte(DATA2);
					}
					TPH_EN	=	ENABLE;
					ko==0?i=16:i;
					TPH_Print1(0);
					MOTO_EN	=	DISABLE;
					
			}
			// MOTO_EN	=		ENABLE;
			
			FeedNDotLine(2);
			// while(MOTO_EN_It == ENABLE);	
			TPH_PrintNum(0);	
	 }
	UpStartUSART[1]=m;
}










/******************************************************************************
* 函数说明：TPH空行打印
* 入口数据：x 	空打印点数
* 返回值：  无
******************************************************************************/
void TPH_Space(u16	x)
{
		u16	c;
		for(c=x;c>0;c--)
		{
			TPH_WR_Byte(0x00);
		}
}



/**********************************************************************
* 函数说明：TPH点电阻测试运行
* 入口数据：x 测试点阵数
* 返回值： 	无
**********************************************************************/
void	TPH_PrintfArr(u16	x)
{
	TPH_EN	=	DISABLE;
	MOTO_EN	=	DISABLE;	
	TPH_Space(x-1);
	TPH_WR_Byte(0x88);
	TPH_PrintNum(x);
}



/******************************************************************************
*	函数说明：TPH打印开始
*	入口数据：x		已输入行点阵数
*	返回值：  无
*	说明：
			补足行点数
			锁存
			加热
			走步
******************************************************************************/
extern	BitAction	PAPER_Key;
void	TPH_PrintNum(u16 x)
{
	
		TPH_EN_It	= 	ENABLE;
		MOTO_EN_It = 	ENABLE;														//关闭中断执行中MOTO的使能
/*	TPH_EN_It	= 	ENABLE;
	MOTO_EN_It = 	ENABLE;														//关闭中断执行中MOTO的使能
	while(MOTO_EN_It == ENABLE)
	{
		if(PAPER_Key	==	Bit_RESET)
		{
			break;
		}
	}*/
}



/******************************************************************************
*	函数说明：电机走步
*	入口数据：zh		走步距
*	返回值：  无
******************************************************************************/
void FeedNDotLine(u8	zh)
{
	for(;zh>0;zh--)
	{
		MotoOneStep();
	}
	// MotoStep(0x00);
	// TPH_EN_It	=	DISABLE;													//中断执行TPH加热使能
}


/******************************************************************************
*	函数说明：电机走1步
*	入口数据：无
*	返回值：  无
******************************************************************************/
void MotoOneStep(void)
{	
	if(MOTO_EN	==	ENABLE)									//电机是否工作
	{
		MotoStep(Step[res]); 
		It_Num=0;															//中断计数清零
		while(It_Num<(UpStartUSART[2]))
		{
			;
		}
		res ++;
		res &= 0x03;
	}
}


/******************************************************************************
*	函数说明：TPH端口引脚初始化
*	入口数据：无
*	返回值：  无
******************************************************************************/
void	TPH_Printinit(void)
{
	TPH_STB(Bit_RESET);
	GPIO_ResetBits(GPIO_Port_TPH_DI,GPIO_Pin_TPH_DI);
	GPIO_ResetBits(GPIO_Port_TPH_CLK,GPIO_Pin_TPH_CLK);
	GPIO_SetBits(GPIO_Port_TPH_LAT,GPIO_Pin_TPH_LAT);
	VH_EN(Bit_SET);
	TPH_EN	=	ENABLE;
	MOTO_EN	=	ENABLE;	
	res	=	0;
	Delay1ms(50);
}

void TPH_Esc(void)
{
	VH_EN(Bit_RESET);
	TPH_EN	=	DISABLE;
	MOTO_EN	=	DISABLE;
	MotoStep(0x06);	
	Delay1ms(20);
	nLED_Link(Bit_SET);
}



