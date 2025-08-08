/**********************************************
*每次烧录修改程序编号
*ProgID: 四位编号 如2001
*2是功能编号,001为对应功能产品编号
*说明：
			1 技术测试程序
			2 生产出厂检验程序
			3	生产TPH测试程序
			4 品管出厂检验程序
			5 品管TPH测试程序
			6	生产维修测试程序
**********************************************/


#include	"Config.h"
#include	"Printf.h"
#include	"Start.h"
#include	"Hardware_Init.h"
#include	"Detect_ADC.h"
#include	"OLED.h"
#include	"TPG.h"
#include	"BEEP_LED.h"
#include	"C_M.h"
#include 	"my_font.h"
#include    "stdio.h"
#include    "stm32f10x_it.h"
// #include 	"ff.h"

void 	ShowNum(u8 d,u8 s,u16 num,u8 len,u8 n,u8 c);
void Load_Font_Config();
u32 	num_pow(u8 m,u8 n);

extern	u32					Cap_USART[2];								//光耦旁路电容
extern	u16					Detect_USART[];								//VH,TM,RAY测试
extern	u16					MT_USART[];									//电机测试结果
extern 	u16 				UpStartUSART[];								//选择型号参数设置
extern	BitAction		STB_ON,STB_OFF;									//有纸信号
extern 	char				*Type_ID;
extern	u16					It_Num;
extern	u8					Link_G_en;									//是否测试头片接地
extern	u8					Print_Minloopnum;
extern	u8					TPH_STB_EN;
extern FunctionalState  MOTO_H_Set;
extern char R_Buff[30], R_n, R_Flag,P_n;
extern u8 P_Buff_Const[300][48]; // 打印图像的常量数组
extern volatile FontDimension font_dim;

u16					ADC_Num[3];										//ADC缓存
BitAction		PAPER_Key;
char 				TPH_Cache[10][24];
u8 					TPH_LAN_TEST;
#define RUN 1

int main(void)
{
	u8	ls;
	u8	lps;
	InitHardWare();													//MCU外设初始化
	if(Key1()	!=	Bit_RESET)
	{
		MOTO_H_Set=ENABLE;
	}	
	MOTO_HzGet();
	UpStart();														//设置机芯初始化参数
	//LCD_Init();													//LCD初始化
	//LCD_Layout();													//LCD显示背景布局
	Cap_Test(DISABLE);	
	nLED_FRONT(Bit_RESET);											//初始化完成指示灯
	while(1)
	{
Retest:		
		// if(Key1()	!=	Bit_RESET)							//按键测试
		if(1)
		{
//			Display_Arial_String(232,118,16, "ON ",DARKBLUE,LGRAYBLUE);
//			Display_GB2312_String(46,170,24, "正在测试产品．．．",YELLOW,LGRAYBLUE);
			nLED_Link(Bit_RESET);	
			nVCC_EN(Bit_RESET);
			//打印前测试内容		
  			// TM_Test();															//加热片温度测试
			// RAY_Test();														//红外光耦测试
			// VH_Test();															//VH测试
			// MT_Test();															//电机电感测试 
			// Cap_Test(ENABLE);													//电容测试
			//if(LCD_Testout()!=0x11){goto Retest;}								//测试的结果判断并LCD输出
			//测试完毕***
			#if !RUN
			u8 step_sequence[8] = {0x05,0x01,0x09,0x08,0x0a,0x02,0x06,0x04}; 
			u8 i;
			VH_EN(Bit_SET);
			MotoStep(0x00);
			for(i = 0; i < 100; i++)   // 转动100步
			{
				MotoStep(step_sequence[i % 8]);  // 循环执行8步序列
				// FeedNDotLine(2);
				Delay1ms(1);  // 控制转速
			}
			MotoStep(0x00);
			VH_EN(Bit_RESET);
			#endif
			// TPH_Printinit();						//打印前 TPH初始化

			// while (PAPER_Key == Bit_SET && R_Flag == 1)		//等待按键
			// {
			// 	for(lps=64;lps>0;lps--)
			// 	{
			// 		TPH_Loop1();
			// 	}	
			// 	TPH_PrintString(0,"                      ",24);
			// 	TPH_PrintString(0, R_Buff, 24); //打印接收到的字符串
			// 	TPH_PrintString(0,"                      ",24);
			// 	TPH_PrintString(0," \\\\\\\\\\| ",24);
			// 	TPH_PrintString(0,"                      ",24);
			// 	R_Flag = 0; // 重置标志位
			// 	R_Buff[0] = '\0';
			// } TPH_Esc(); //结束打印
			// nVCC_EN(Bit_SET);
			// for (int i = 0; i < 100; i++)
			// MotoOneStep();
			// nVCC_EN(Bit_SET);

			#if RUN
			ls=0;
			while(PAPER_Key	!=	Bit_SET		||	ls	==	0 )
			{
				if(It_Num>5000)													//每X00ms 检测一次HPT温度和光耦取值
				{
					It_Num=0;
					if(TM_Test()!=0x11) //判断温度范围
						{
							nVCC_EN(Bit_SET);
							Display_Arial_String(232,118,16, "OFF",RED,LGRAYBLUE);  
							LCD_DataClear();
							goto Retest;
						}
					RAY_Test();															//红外光耦测试
					LCD_Variable();	
					if(Detect_USART[5]	==	0x11)				//光耦极限值检测
						{
								ls	= 1;
						}	
					}
				}
			
			/*******************************
			打印的内容
			********************************/
			memset(TPH_Cache[0],'\0', sizeof(TPH_Cache[0]));
			strcpy(TPH_Cache[0],"VH_Test:  ");
			ShowNum(strlen(TPH_Cache[0]),0,Detect_USART[0],4,'m','V');
				
			memset(TPH_Cache[1],'\0', sizeof(TPH_Cache[1]));
			strcpy(TPH_Cache[1],"Cap_Test: ");
			ShowNum(strlen(TPH_Cache[1]),1,Cap_USART[0],4,'p','F');
				
			memset(TPH_Cache[2],'\0', sizeof(TPH_Cache[2]));
			strcpy(TPH_Cache[2],"MOTO_Test:");
			ShowNum(strlen(TPH_Cache[2]),2,MT_USART[0],4,'u','H');
			
			
			// memset(TPH_Cache[6],'\0', sizeof(TPH_Cache[6]));
			TPH_Cache[6][0] = '\0';
			strcpy(TPH_Cache[6],"RAY_Test: ");
			ShowNum(strlen(TPH_Cache[6]),6,(Detect_USART[4]),4,'m','V');
			memset(TPH_Cache[7],'\0', sizeof(TPH_Cache[7]));
			strcpy(TPH_Cache[7],"TM_Test:  ");
			ShowNum(strlen(TPH_Cache[7]),7,(Detect_USART[2]),3,'~','C');	
			// strcpy(TPH_Cache[8],"this is a test");
			TPH_Printinit();						//打印前 TPH初始化

			while (PAPER_Key == Bit_SET && R_Flag == 1)		//等待按键
			{
				TPH_PrintString(0,"hello",24);
				for(lps=64;lps>0;lps--)
				{
					TPH_Loop1();
				}	
				TPH_PrintString(0,"                      ",24);
				TPH_PrintString(0, R_Buff, 24); //打印接收到的字符串
				TPH_PrintString(0,"                      ",24);
				TPH_PrintString(0," |||||||||| ",24);
				TPH_PrintString(0,"                      ",24);
				
				// Load_Font_Config(); // 加载图片配置
				TPH_PrintImage_m2((const unsigned char *)P_Buff_Const,font_dim.height,font_dim.width); // 打印图像,从串口获取字模
				// TPH_PrintImage_m2((const unsigned char *)P_Buff_Const,MY_FONT_ROWS,MY_FONT_COLS); // 打印图像,从文件获取字模
				// TPH_PrintImage_m2((const unsigned char *)my_font,MY_FONT_ROWS,MY_FONT_COLS); // 打印图像,从文件获取字模
				
				R_Flag = 0; // 重置标志位
				memset(P_Buff_Const[0], '\0', sizeof(P_Buff_Const[0]));
			} 
			// MT_SW(Bit_RESET);	
			

			

			// do
			// 	{	
			// 		//STB黑块打印
			// /*		for(lps=48;lps>0;lps--)
			// 		{
			// 			TPH_Loop0();
			// 		}	*/
					
			// 		TPH_PrintString(0,"hello",24);
			// 		for(lps=64;lps>0;lps--)
			// 		{
			// 			TPH_Loop1();
			// 		}	
			// 	//	TPH_PrintString(0,"                        ",16);
			// 		for(lps=64;lps>0;lps--)
			// 		{
			// 			TPH_Loop2();
			// 		}
			// 		TPH_Space(192);
			// 	//	TPH_PrintString(0,"                        ",16);
			// 		for(lps=64;lps>0;lps--)
			// 		{
			// 			TPH_Loop3();
			// 		}	
			// 		TPH_Space(192);
			// 	//	TPH_PrintString(0,"                        ",16);			

			// 		TPH_ShowPicture();
			// 		//内容打印
			// 		TPH_PrintString(0,"                      ",24);
			// 		TPH_PrintString(16,TPH_Cache[0],24);
			// 		TPH_PrintString(16,TPH_Cache[1],24);
			// 		TPH_PrintString(16,TPH_Cache[2],24);
			// 		TPH_PrintString(16,TPH_Cache[6],24);
			// 		TPH_PrintString(16,TPH_Cache[7],24);
			// 		// TPH_PrintString(16,TPH_Cache[8],24);
			// 		TPH_PrintString(0,"                      ",24);
			// 		TPH_PrintString(0," ! # $ % & ' ( ) * + , - . / 0 1 2 3 4 5 6 7 8 9 : ; < = > ? @ A B C D E F G H I J K L M N O P Q R S T U V W X Y Z [ ] ^ _ ` a b c d e f g h i j k l m n o p q r s t u v w x y z { | } ~",24);
			// /*		TPH_PrintString(0,"! # $ % & ' ( ) * + , - . / 0 1 2 3 4 5 6 7 8 9",24);
			// 		if(PAPER_Key!=Bit_SET)break;
			// 		TPH_PrintString(0,": ; < = > ? @ A B C D E F G H I J K L M N O P Q",24);
			// 		if(PAPER_Key!=Bit_SET)break;
			// 		TPH_PrintString(0,"R S T U V W X Y Z [ ] ^ _ ` a b c d e f g h i j",24);
			// 		if(PAPER_Key!=Bit_SET)break;
			// 		TPH_PrintString(0,"k l m n o p q r s t u v w x y z { } ~ | ",24);					*/
			// 		TPH_PrintString(0,"                      ",24);
			// 		TPH_PrintString(110,"Print Test Notes",24);
			// 		TPH_PrintString(128,Type_ID,24);
			// 		TPH_PrintString(0,"Speed: 62mm/s",24);
			// 		TPH_PrintString(0,"Versions: 2R_V4_3",24);
			// 		TPH_PrintString(0,"                                        ",24);
			// 		TPH_LAN_TEST=1;
			// 		TPH_PrintString(0,"||||  LAN_Error",24);
			// 		TPH_LAN_TEST=0;
			// }while(PAPER_Key==Bit_SET);
			TPH_Esc();
			Display_GB2312_String(92,160,24, "打印完毕",DARKBLUE,LGRAYBLUE);
			while(Key1()	==	Bit_SET)		//检测按键或者测试产品离开后跳出
			{
				if(It_Num>2500)						//每500ms 检测一次HPT温度和光耦取值
				{
					It_Num=0;
					TM_Test();															//热敏片温度测试
					RAY_Test();															//红外光耦测试
//					LCD_Variable();	
				}
			}
			nVCC_EN(Bit_SET);
//			Display_Arial_String(232,118,16, "OFF",RED,LGRAYBLUE);   		//ASC 12x16点阵(Arial类型)
//			LCD_DataClear();
			#endif
		}
	}
}




/******************************************************************************
*函数说明：TPH添加同一行变量
*入口数据：d：头部数据 
					 num：添加的变量
					 len：保留变量的位数
					 n,c：添加的后缀
*返回值：  无
*******************************************************************************/
void ShowNum(u8 d,u8 s,u16 num,u8 len,u8 n,u8 c)
{         	
	u8 t,temp;
	u8 enshow=0;	
	for(t=d;t<(len+d);t++)
	{
		temp=(num/num_pow(10,len-(t-d)-1))%10;
		if(enshow==0&&t<(len-1+d))
		{
			if(temp==0)
			{
				TPH_Cache[s][t]=' ';
				continue;
			}else enshow=1; 
		}
		TPH_Cache[s][t]=temp+0x30;
	 //	OLED_ShowChar(x+(sizey/2)*t,y,temp+'0',sizey); 
	}
	TPH_Cache[s][t]=' ';
	TPH_Cache[s][t+1]=n;
	TPH_Cache[s][t+2]=c;
}






/******************************************************************************
*函数说明：m^n
*入口数据：m:底数 n:指数
*返回值：  result
******************************************************************************/
// u32 num_pow(u8 m,u8 n)
// {
// 	u32 result=1;
// 	while(n--)result*=m;    
// 	return result;
// }

//快速幂
u32 num_pow(u8 m,u8 n)
{
    u32 result = 1;
    u32 base = m;
    while(n > 0) {
        if(n & 1) {
            result *= base;
        }
        base *= base;
        n >>= 1;
    }
    return result;
}



/******************************************************************************
*函数说明：TPH打印图片更新x'y
*入口数据：无
*返回值：  无
******************************************************************************/
// void Load_Font_Config() {
//     FIL file;
//    f_open(&file, "font_config.bin", FA_READ);
//    f_read(&file, &MY_FONT_ROWS, 2, NULL);
//    f_read(&file, &MY_FONT_COLS, 2, NULL);
//    f_close(&file);
// }










