/***************软件版本2R_V2_0**************************
*TPH热敏打印
********************************************************/

#ifndef __TPG_H_
#define __TPG_H_



#include	"Config.h"
#include	"TPG_Font.h"
#include    "TPG_Pic.h"




#define TPH_HLAT(Temp)		(GPIO_WriteBit(GPIO_Port_TPH_LAT,GPIO_Pin_TPH_LAT,Temp))			//数据锁存
#define TPH_SCLK(Temp)		(GPIO_WriteBit(GPIO_Port_TPH_CLK,GPIO_Pin_TPH_CLK,Temp))			//时钟
#define TPH_HDI(Temp)			(GPIO_WriteBit(GPIO_Port_TPH_DI,GPIO_Pin_TPH_DI,Temp))				//数据

#define TPH_STB1(Temp) 		(GPIO_WriteBit(GPIO_Port_STB_B,GPIO_Pin_STB1,Temp))		//1加热控制
#define TPH_STB2(Temp) 		(GPIO_WriteBit(GPIO_Port_STB_C,GPIO_Pin_STB2,Temp))		//2加热控制
#define TPH_STB3(Temp)		(GPIO_WriteBit(GPIO_Port_STB_C,GPIO_Pin_STB3,Temp))		//3加热控制
#define TPH_STB4(Temp)		(GPIO_WriteBit(GPIO_Port_STB_A,GPIO_Pin_STB4,Temp))		//4加热控制
#define TPH_STB5(Temp) 		(GPIO_WriteBit(GPIO_Port_STB_A,GPIO_Pin_STB5,Temp))		//5加热控制

#define TPH_STB(Temp) 		(GPIO_WriteBit(GPIO_Port_STB_B,GPIO_Pin_STB1,Temp),	\
													 GPIO_WriteBit(GPIO_Port_STB_C,GPIO_Pin_STB2	|	GPIO_Pin_STB3,Temp),	\
													 GPIO_WriteBit(GPIO_Port_STB_A,GPIO_Pin_STB4	|	GPIO_Pin_STB5,Temp))		//加热控制


void 	TPH_WR_Byte(u8 dat);
void 	TPH_PrintString(u16 x,char *dp,u8 size);
void	TPH_PrintChar(u16 x,u8 o,u8 chr,u8 size);

void 	TPH_Space(u16	x);
void	TPH_PrintfArr(u16	x);

void	MotoOneStep(void);
void	FeedNDotLine(u8	zh);
void	TPH_Printinit(void);
void 	TPH_Esc(void);
void	TPH_PrintNum(u16 x);
FunctionalState TPH_Column(u16 x,u16 b);
void TPH_ShowPicture(void);
//void TPH_128Loop(u16 kNum);
void 	TPH_Loop0(void);
void 	TPH_Loop1(void);
void 	TPH_Loop2(void);
void 	TPH_Loop3(void);
void 	TPH_Loop4(void);
void	TPH_STB_Init(u8 l);
void TPH_PrintImage_m2(const unsigned char *image, u16 rows, u16 cols);

#endif /* __TPG_H_ */




