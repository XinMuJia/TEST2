#ifndef __CONFIG_H_
#define __CONFIG_H_
#include	"stm32f10x.h"
#include  <string.h>
#include	<stdio.h>
#include	<math.h>


//步走电机测试引脚
#define GPIO_Port_Moto				GPIOB
#define	GPIO_Pin_MotoA_Z			GPIO_Pin_12
#define	GPIO_Pin_MotonA_Z			GPIO_Pin_13
#define	GPIO_Pin_MotoB_Z			GPIO_Pin_15
#define	GPIO_Pin_MotonB_Z			GPIO_Pin_14


#define	GPIO_Port_MT_SW				GPIOC
#define	GPIO_Pin_MT_SW				GPIO_Pin_8

//电机电感测试引脚
#define	GPIO_Port_FL				GPIOC
#define	GPIO_Pin_FL					GPIO_Pin_9
//电容测试引脚
#define	GPIO_Port_FC				GPIOA
#define	GPIO_Pin_FC					GPIO_Pin_8



//连接测试
#define	GPIO_Port_Link				GPIOA
#define	GPIO_Pin_Link_H_S			GPIO_Pin_3

//TPH扇区选通,STB
#define	GPIO_Port_STB_B				GPIOB
#define	GPIO_Pin_STB1					GPIO_Pin_0
#define	GPIO_Port_STB_C				GPIOC
#define	GPIO_Pin_STB2					GPIO_Pin_5
#define	GPIO_Pin_STB3					GPIO_Pin_4
#define	GPIO_Port_STB_A				GPIOA
#define	GPIO_Pin_STB4					GPIO_Pin_7
#define	GPIO_Pin_STB5					GPIO_Pin_6
//TPH锁存,LAT
#define	GPIO_Port_TPH_LAT			GPIOB
#define	GPIO_Pin_TPH_LAT			GPIO_Pin_1
//TPH数据时钟,DI,CLK
#define	GPIO_Port_TPH_DI			GPIOA
#define	GPIO_Pin_TPH_DI				GPIO_Pin_4
#define	GPIO_Port_TPH_CLK			GPIOA
#define	GPIO_Pin_TPH_CLK			GPIO_Pin_5

//电平选通
#define	GPIO_Port_VH					GPIOC
#define	GPIO_Pin_VH_EN				GPIO_Pin_7
#define GPIO_Port_VCC					GPIOB
#define GPIO_Pin_VCC_EN				GPIO_Pin_10

//蜂鸣器
#define	GPIO_Port_BEEP				GPIOC
#define	GPIO_Pin_BEEP					GPIO_Pin_2
//LED指示灯
#define	GPIO_Port_LED_Link		GPIOC
#define	GPIO_Pin_LED_Link			GPIO_Pin_14
#define	GPIO_Port_LED_Error		GPIOC
#define	GPIO_Pin_LED_Error		GPIO_Pin_15
#define	GPIO_Port_LED_FRONT		GPIOC
#define	GPIO_Pin_LED_FRONT		GPIO_Pin_13

//OLED显示屏
#define	GPIO_Port_LCD_SCLK		GPIOD
#define	GPIO_Pin_LCD_SCLK			GPIO_Pin_2
#define	GPIO_Port_LCD					GPIOB
#define	GPIO_Pin_LCD_MOSI			GPIO_Pin_3
#define	GPIO_Pin_LCD_RES			GPIO_Pin_4
#define	GPIO_Pin_LCD_DC				GPIO_Pin_5
#define	GPIO_Pin_LCD_BLK  		GPIO_Pin_6
#define	GPIO_Pin_ZK_MISO			GPIO_Pin_7
#define	GPIO_Pin_LCD_CS				GPIO_Pin_8
#define	GPIO_Pin_ZK_CS				GPIO_Pin_9

//按钮开关,开始打印
#define	GPIO_Port_Start				GPIOC
#define	GPIO_Pin_Start				GPIO_Pin_0

//ADC功能引脚
#define	GPIO_Port_ADC_A				GPIOA
#define	GPIO_Pin_ADC_PAPER		GPIO_Pin_1
#define	GPIO_Pin_ADC_TM				GPIO_Pin_2
#define	GPIO_Pin_ADC_VDC			GPIO_Pin_0





//胶辊测试
#define	H_S()								GPIO_ReadInputDataBit(GPIO_Port_Link,GPIO_Pin_Link_H_S)
//VH开关
#define	VH_EN(Temp)					GPIO_WriteBit(GPIO_Port_VH,GPIO_Pin_VH_EN,Temp)
#define	nVCC_EN(Temp)				GPIO_WriteBit(GPIO_Port_VCC,GPIO_Pin_VCC_EN,Temp)
//轻触开关Start，Calibration
#define	Start_Key()					GPIO_ReadInputDataBit(GPIO_Port_Start,GPIO_Pin_Start)
//LED灯设置
#define nLED_ERROR(Temp) 		GPIO_WriteBit(GPIO_Port_LED_Error,GPIO_Pin_LED_Error,Temp)
#define nLED_Link(Temp) 		GPIO_WriteBit(GPIO_Port_LED_Link,GPIO_Pin_LED_Link,Temp)
#define nLED_FRONT(Temp) 		GPIO_WriteBit(GPIO_Port_LED_FRONT,GPIO_Pin_LED_FRONT,Temp)
#define BEEP(Temp) 					GPIO_WriteBit(GPIO_Port_BEEP,GPIO_Pin_BEEP,Temp)

//电机设置
#define	MT_SW(Temp)					GPIO_WriteBit(GPIO_Port_MT_SW,GPIO_Pin_MT_SW,Temp)
#define	FCHzing()						GPIO_ReadInputDataBit(GPIO_Port_FC,GPIO_Pin_FC)
#define	FLHzing()						GPIO_ReadInputDataBit(GPIO_Port_FL,GPIO_Pin_FL)

#define MotoStep(Temp)(		\
GPIO_WriteBit(GPIO_Port_Moto,GPIO_Pin_MotoA_Z,(BitAction)(Temp & 0x01)),					\
GPIO_WriteBit(GPIO_Port_Moto,GPIO_Pin_MotonA_Z,(BitAction)((Temp & 0x02) >> 1)),		\
GPIO_WriteBit(GPIO_Port_Moto,GPIO_Pin_MotoB_Z,(BitAction)((Temp & 0x04) >> 2)),		\
GPIO_WriteBit(GPIO_Port_Moto,GPIO_Pin_MotonB_Z,(BitAction)((Temp & 0x08) >> 3))		\
)

extern u16	ADC_Num[3];	

void	Delay1us(u16 us);
void  Delay1ms(u16 ms);
void SysTick_Delay_1Ms( __IO uint32_t ms);
void SysTick_Delay_5Us( __IO uint32_t us);
#endif /* __CONFIG_H_ */
