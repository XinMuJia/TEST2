/*******************************************************
*****************固件初始化设置*************************
***************固件版本TestPCBV2.0**********************
*@设置内容
*		1.RCCInit(void)										外部8M时钟启用	
*		2.void	GPIOInit(void)						引脚端口初始化启用
*******************************************************/
#include	"Hardware_Init.h"
#include	"Config.h"
#include	"BEEP_LED.h"

ErrorStatus HSEStartUpStatus;



static void RCCInit(void)
{
	//重置RCC时钟
	RCC_DeInit();
	//打开外部高速时钟
	RCC_HSEConfig(RCC_HSE_ON);
	//等待HSE开启
	HSEStartUpStatus	=	RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus	==	SUCCESS)
	{
		//总线时钟，系统时钟1分频
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		//外设2时钟，总线时钟1分频
		RCC_PCLK2Config(RCC_HCLK_Div1);
		//外设1时钟，总线时钟2分频
		RCC_PCLK1Config(RCC_HCLK_Div2);
		//	PLLCLK = 8MHz *		7	  = 56 MHz	
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_7);	//RCC_PLLMul_4
		//打开PLL时钟
		RCC_PLLCmd(ENABLE);
		//等待PLL时钟开启
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){}
		//设置系统时钟为PLL时钟
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		//确认系统时钟变化
		while(RCC_GetSYSCLKSource()	!=	0x08){}
	}
	RCC_AHBPeriphClockCmd	(RCC_AHBPeriph_DMA1,ENABLE);
		
	RCC_APB2PeriphClockCmd(	\
													RCC_APB2Periph_GPIOA	|	\
													RCC_APB2Periph_GPIOB	|	\
													RCC_APB2Periph_GPIOC	|	\
													RCC_APB2Periph_GPIOD	|	\
													RCC_APB2Periph_GPIOE	|	\
													RCC_APB2Periph_USART1	|	\
													RCC_APB2Periph_ADC1		|	\
													RCC_APB2Periph_AFIO	\
													,ENABLE);
	
		
	RCC_APB1PeriphClockCmd(	\
													RCC_APB1Periph_TIM3		|	\
													RCC_APB1Periph_USART3		\
													,ENABLE);
	
}





void	GPIOInit(void)
{
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  
	GPIO_InitTypeDef	GPIO_InitStructure;
	//步进电机引脚设置
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_MotoA_Z	|	GPIO_Pin_MotonA_Z;
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_Out_PP;
	GPIO_Init(GPIO_Port_Moto,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_MotoB_Z	|	GPIO_Pin_MotonB_Z;
	GPIO_Init(GPIO_Port_Moto,&GPIO_InitStructure);
	//测试启动引脚设置
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_MT_SW;
	GPIO_Init(GPIO_Port_MT_SW,&GPIO_InitStructure);
	//电平选通引脚设置
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_VH_EN;
	GPIO_Init(GPIO_Port_VH,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_VCC_EN;
	GPIO_Init(GPIO_Port_VCC,&GPIO_InitStructure);
	
	
	//TPH功能引脚设置
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_STB1;
	GPIO_Init(GPIO_Port_STB_B,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_STB2	|	GPIO_Pin_STB3;
	GPIO_Init(GPIO_Port_STB_C,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_STB4	|	GPIO_Pin_STB5;
	GPIO_Init(GPIO_Port_STB_A,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_TPH_LAT;
	GPIO_Init(GPIO_Port_TPH_LAT,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_TPH_DI;
	GPIO_Init(GPIO_Port_TPH_DI,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_TPH_CLK;
	GPIO_Init(GPIO_Port_TPH_CLK,&GPIO_InitStructure);
	
	//蜂鸣器引脚设置
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_BEEP;
	GPIO_Init(GPIO_Port_BEEP,&GPIO_InitStructure);
	//LED指示灯引脚设置
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_LED_Error;
	GPIO_Init(GPIO_Port_LED_Error,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_LED_Link;
	GPIO_Init(GPIO_Port_LED_Link,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_LED_FRONT;
	GPIO_Init(GPIO_Port_LED_FRONT,&GPIO_InitStructure);
	//OLED显示屏引脚设置
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_ZK_MISO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
	GPIO_Init(GPIO_Port_LCD,&GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_LCD_SCLK;
	GPIO_Init(GPIO_Port_LCD_SCLK,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_LCD_MOSI	|	GPIO_Pin_LCD_RES	|	GPIO_Pin_LCD_DC;
	GPIO_Init(GPIO_Port_LCD,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_LCD_BLK	|	GPIO_Pin_LCD_CS		|	GPIO_Pin_ZK_CS;
	GPIO_Init(GPIO_Port_LCD,&GPIO_InitStructure);
	

	//输入设置
	//电容电感频率检测输入
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_FC;
	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_IPU;
	GPIO_Init(GPIO_Port_FC,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_FL;
	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_IPU;
	GPIO_Init(GPIO_Port_FL,&GPIO_InitStructure);
	
	//胶辊/接地测试输入
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_Link_H_S;
	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_IPU;
	GPIO_Init(GPIO_Port_Link,&GPIO_InitStructure);

	//按钮开关引脚设置
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_Start;
	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_IPD;
	GPIO_Init(GPIO_Port_Start,&GPIO_InitStructure);
	
	
	//ADC功能引脚设置
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_ADC_PAPER	|	GPIO_Pin_ADC_TM	|	GPIO_Pin_ADC_VDC;
	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_AIN;
	GPIO_Init(GPIO_Port_ADC_A,&GPIO_InitStructure);
	
//USART1引脚定义
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;						
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
	//复用输出PA9，做TX
	GPIO_Init(GPIOA,&GPIO_InitStructure);								
	GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_10;			
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	//浮空输入PA10，做RX
	GPIO_Init(GPIOA,&GPIO_InitStructure);													
}



void	NVICInit()
{
	/***************
	Tim3中断配置
	*****************/
	NVIC_InitTypeDef	NVIC_InitStructure;
	//NVIC优先级组配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	//中断源：TIM3_IRQn
	NVIC_InitStructure.NVIC_IRQChannel	=	TIM3_IRQn;
	//抢占优先级：0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	0;
	//子优先级：0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority	=	0;
	//使能中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd	=	ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/*************
	USART1中断配置
	*************/
	NVIC_InitStructure.NVIC_IRQChannel	=	USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	1;
	NVIC_InitStructure.NVIC_IRQChannelCmd	=	ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
//定时器3
//中断计时100Um

void	Timer3Init()
{
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	// 自动重装载寄存器周的值(计数值)
	TIM_TimeBaseStructure.TIM_Period	=	100;
	// 时钟预分频数为56，则驱动计数器的时钟CK_CNT = CK_INT(56) / (55+1)=	1M
	TIM_TimeBaseStructure.TIM_Prescaler	=	55;
	// 时钟分频因子
	TIM_TimeBaseStructure.TIM_ClockDivision	=	TIM_CKD_DIV1;
	// 计数器计数模式
	TIM_TimeBaseStructure.TIM_CounterMode	=	TIM_CounterMode_Down;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	//清除溢出标志
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	//使能TIM3_UP中断
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	//使能TIM3定时器
	TIM_Cmd(TIM3,ENABLE);
}



void USART1Init()
{							
	//复位USART1	
	USART_DeInit(USART1);			
	//配置USART1
	USART_InitTypeDef USART_InitStructure;			
	
  	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//使能USART1	
	//配置波特率	
	USART_InitStructure.USART_BaudRate = 115200;																		
	//无硬件数据流控制	
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	
	//收发模式
	USART_InitStructure.USART_Mode	=	USART_Mode_Rx	| USART_Mode_Tx	;					
	//无奇偶检验位
	USART_InitStructure.USART_Parity=USART_Parity_No;			
	//1位停止位	
	USART_InitStructure.USART_StopBits=USART_StopBits_1;														
	//8位数据
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;				
	//初始化配置USART1
	USART_Init(USART1,&USART_InitStructure);	
	// 使能串口接收中断
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);	
	//使能USART1串口
	USART_Cmd(USART1,ENABLE);	
}


/********************
例码上抄的
3-ADC-多通道-DMA读取
********************/
static void ADC1Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32) ( & ( ADC1->DR ) );
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_Num;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 3;    // 改
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  // 改
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1 , ENABLE);
/*----------------------------------------------------------------------*/
	
	ADC_InitTypeDef ADC_InitStruct;	
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;        // 改
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;	
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_NbrOfChannel = 3;	
	ADC_Init(ADC1,&ADC_InitStruct);
	RCC_ADCCLKConfig(RCC_PCLK2_Div4);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0,  1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1,  2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2,  3, ADC_SampleTime_55Cycles5);
 	// 使能ADC DMA 请求
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	// 初始化ADC 校准寄存器
	ADC_ResetCalibration(ADC1);
	// 等待校准寄存器初始化完成
	while(ADC_GetResetCalibrationStatus(ADC1)){}
  // ADC开始校准
	ADC_StartCalibration(ADC1);
	// 等待校准完成
	while(ADC_GetCalibrationStatus(ADC1));
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}



/************************************
* 函数：void	IO_Init(void)
* 功能：IO端口引脚初始化
* 入口参数：无
* 出口参数：无
*************************************/
void	IO_Init(void)
{
	GPIO_ResetBits(GPIO_Port_Moto,GPIO_Pin_MotoA_Z);		
	GPIO_ResetBits(GPIO_Port_Moto,GPIO_Pin_MotonA_Z);
	GPIO_ResetBits(GPIO_Port_Moto,GPIO_Pin_MotoB_Z);
	GPIO_ResetBits(GPIO_Port_Moto,GPIO_Pin_MotonB_Z);
	
	VH_EN(Bit_RESET);			
	nVCC_EN(Bit_SET);	
	MT_SW(Bit_RESET);
	
	nLED_ERROR(Bit_SET);
	nLED_Link(Bit_SET);
	nLED_FRONT(Bit_SET);	
	
	BEEP_Di();													//提示音
}


//以下是SPI模块的初始化代码，配置成主机模式 						  
//SPI口初始化
//这里针是对SPI1的初始化
// void SPI1_Init(void)
// {
// 	GPIO_InitTypeDef  GPIO_InitStructure;
// 	SPI_InitTypeDef  SPI_InitStructure;
	
// 	/* SPI的IO口和SPI外设打开时钟 */
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
// 	/* 启用SPI1引脚重映射，使用备用引脚 */
// 	GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);
	
// 	/* SPI的IO口设置 - 使用重映射后的引脚 */
// 	/* PA5重映射到PB3，PA6重映射到PB4，PA7重映射到PB5 */
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;  //PB3-SPI1_SCK PB4-SPI1_MISO PB5-SPI1_MOSI
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
// 	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
// 	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
// 	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
// 	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
// 	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
// 	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
// 	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
// 	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
// 	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
// 	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	
// 	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
	
// 	SPI1_ReadWriteByte(0xff);//启动传输	
// }

void InitHardWare(void)
{
	RCCInit();
	GPIOInit();
	ADC1Init();
	Timer3Init();
	USART1Init();
	NVICInit();
	IO_Init();
}






