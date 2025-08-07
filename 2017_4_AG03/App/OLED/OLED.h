#ifndef __OLED_H_
#define __OLED_H_

#include	"Config.h"


//0,不支持ucos
//1,支持ucos
#define SYSTEM_SUPPORT_UCOS		0		//定义系统文件夹是否支持UCOS
																	    
	 
//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入






#define USE_HORIZONTAL 2  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 320

#else
#define LCD_W 320
#define LCD_H 240
#endif


//-----------------测试LED端口定义---------------- 

//#define LED_ON GPIO_ResetBits(GPIOA,GPIO_Pin_15)
//#define LED_OFF GPIO_SetBits(GPIOA,GPIO_Pin_15)

//-----------------LCD端口定义---------------- 

#define	LCD_SCLK_Clr() 	GPIO_ResetBits(GPIO_Port_LCD_SCLK,GPIO_Pin_LCD_SCLK)	//SCL=SCLK
#define LCD_SCLK_Set() 	GPIO_SetBits(GPIO_Port_LCD_SCLK,GPIO_Pin_LCD_SCLK)

#define LCD_MOSI_Clr() 	GPIO_ResetBits(GPIO_Port_LCD,GPIO_Pin_LCD_MOSI)	//SDA=MOSI
#define LCD_MOSI_Set() 	GPIO_SetBits(GPIO_Port_LCD,GPIO_Pin_LCD_MOSI)
	
#define LCD_RES_Clr() 	GPIO_ResetBits(GPIO_Port_LCD,GPIO_Pin_LCD_RES)//RES
#define LCD_RES_Set() 	GPIO_SetBits(GPIO_Port_LCD,GPIO_Pin_LCD_RES)

#define LCD_DC_Clr() 		GPIO_ResetBits(GPIO_Port_LCD,GPIO_Pin_LCD_DC)//DC
#define LCD_DC_Set() 		GPIO_SetBits(GPIO_Port_LCD,GPIO_Pin_LCD_DC)
 		     
#define LCD_BLK_Clr()  	GPIO_ResetBits(GPIO_Port_LCD,GPIO_Pin_LCD_BLK)//BLK
#define LCD_BLK_Set()  	GPIO_SetBits(GPIO_Port_LCD,GPIO_Pin_LCD_BLK)

#define ZK_MISO       	GPIO_ReadInputDataBit(GPIO_Port_LCD,GPIO_Pin_ZK_MISO)//MISO  读取字库数据引脚

#define LCD_CS_Clr() 	 	GPIO_ResetBits(GPIO_Port_LCD,GPIO_Pin_LCD_CS)//FR1
#define LCD_CS_Set() 		GPIO_SetBits(GPIO_Port_LCD,GPIO_Pin_LCD_CS)

#define ZK_CS_Clr()  		GPIO_ResetBits(GPIO_Port_LCD,GPIO_Pin_ZK_CS)//FR2
#define ZK_CS_Set()   	GPIO_SetBits(GPIO_Port_LCD,GPIO_Pin_ZK_CS)


void LCD_GPIO_Init(void);//初始化GPIO
void LCD_Writ_Bus(u8 dat);//模拟SPI时序
void LCD_WR_DATA8(u8 dat);//写入一个字节
void LCD_WR_DATA(u16 dat);//写入两个字节
void LCD_WR_REG(u8 dat);//写入一个指令
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//设置坐标函数
void LCD_Init(void);//LCD初始化



void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);//指定区域填充颜色
void LCD_DrawPoint(u16 x,u16 y,u16 color);//在指定位置画一个点
void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);//在指定位置画一条线
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);//在指定位置画一个矩形
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color);//在指定位置画一个圆

void LCD_ShowChinese(u16 x,u16 y,char *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示汉字串
void LCD_ShowChinese12x12(u16 x,u16 y,char *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示单个12x12汉字
void LCD_ShowChinese16x16(u16 x,u16 y,char *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示单个16x16汉字
void LCD_ShowChinese24x24(u16 x,u16 y,char *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示单个24x24汉字
void LCD_ShowChinese32x32(u16 x,u16 y,char *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示单个32x32汉字

void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode);//显示一个字符
void LCD_ShowString(u16 x,u16 y,const char *p,u16 fc,u16 bc,u8 sizey,u8 mode);//显示字符串
u32 mypow(u8 m,u8 n);//求幂
void LCD_ShowIntNum(u16 x,u16 y,u16 num,u8 len,u16 fc,u16 bc,u8 sizey);//显示整数变量
void LCD_ShowFloatNum1(u16 x,u16 y,float num,u8 len,u16 fc,u16 bc,u8 sizey);//显示两位小数变量

void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width);//显示图片

//字库函数
void ZK_command(u8 dat);
u8 get_data_from_ROM(void);
void get_n_bytes_data_from_ROM(u8 AddrHigh,u8 AddrMid,u8 AddrLow,u8 *pBuff,u8 DataLen);
void Display_GB2312(u16 x,u16 y,u8 zk_num,u16 fc,u16 bc);
void Display_GB2312_String(u16 x,u16 y,u8 zk_num,char text[],u16 fc,u16 bc);
void Display_Asc(u16 x,u16 y,u8 zk_num,u16 fc,u16 bc);
void Display_Asc_String(u16 x,u16 y,u16 zk_num,char text[],u16 fc,u16 bc);
void Display_Arial_TimesNewRoman(u16 x,u16 y,u8 zk_num,u16 fc,u16 bc);
void Display_Arial_String(u16 x,u16 y,u16 zk_num,char text[],u16 fc,u16 bc);
void Display_TimesNewRoman_String(u16 x,u16 y,u16 zk_num,u8 text[],u16 fc,u16 bc);

/**************************************************************/
void	LCD_Layout(void); 		//LCD显示背景布局
void	LCD_DataClear(void);	//LCD检测数据清除
u8		LCD_Testout(void);			//OLED显示测试数据，并判断结果
u8		LCD_HS(void);						//胶辊检测并显示
void	LCD_Variable(void);

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE           	 0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 //棕色
#define BRRED 			     0XFC07 //棕红色
#define GRAY  			     0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			     0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

#endif


