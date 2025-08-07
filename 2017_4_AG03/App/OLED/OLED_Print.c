#include 	"OLED.h"
#include	"BEEP_LED.h"
#include	"Detect_ADC.h"

extern	u16					Detect_USART[];										//VH,TM,RAY测试
extern	u16 				MT_USART[2];
extern	u16 				Cap_USART[2];
u8			LCD_HS(void);

/******************************************
* 函数：u8	LCD_Testout(void)
* 功能：OLED显示测试数据，并判断结果
* 入口参数：无
* 出口参数：	
*		机芯测试结果   0x11  测试成功
									 0xAA	 测试错误
******************************************/
extern 	u16 				UpStartUSART[];
u8	LCD_Testout(void)
{
	u8	Error_Flag=0;
	
	//VH值
	LCD_ShowIntNum(72,67,Detect_USART[0],4,DARKBLUE,LGRAYBLUE,16);
	Display_Arial_String(112,67,16,"MV",	DARKBLUE,LGRAYBLUE);   		//ASC 12x16点阵(Arial类型)
	//旁路电容
	LCD_ShowIntNum(232,67,Cap_USART[0],4,DARKBLUE,LGRAYBLUE,16);
	Display_Arial_String(272,67,16,"PF",	DARKBLUE,LGRAYBLUE);   		//ASC 12x16点阵(Arial类型)
	//电机电感
	LCD_ShowIntNum(72,84,MT_USART[0],4,DARKBLUE,LGRAYBLUE,16);   		//ASC 12x16点阵(Arial类型)
	Display_Arial_String(112,84,16,"UH",	DARKBLUE,LGRAYBLUE);   		//ASC 12x16点阵(Arial类型)
	//电机速度
	Display_Arial_String(232,84,16, "62MM /S",DARKBLUE,LGRAYBLUE);   		//ASC 12x16点阵(Arial类型)
	//温度
	LCD_ShowIntNum(72,101,Detect_USART[2],4,DARKBLUE,LGRAYBLUE,16);   			//ASC 12x16点阵(Arial类型)
	Display_GB2312_String(112,101,16, "℃",DARKBLUE,LGRAYBLUE); 				//15x16汉字
	//光耦分压
	LCD_ShowIntNum(232,101,Detect_USART[4],4,DARKBLUE,LGRAYBLUE,16);   		//ASC 12x16点阵(Arial类型)
	Display_Arial_String(272,101,16,"MV",	DARKBLUE,LGRAYBLUE);   		//ASC 12x16点阵(Arial类型)
	
	//VH错误输出OLED
	if(Detect_USART[1]	!=	0x11)
	{
		Display_GB2312_String(46,160,24, "工作电压错误！！！",RED,LGRAYBLUE); 							//15x16汉字
		Error_Flag=1;
	}
	
	//旁路电容错误输出OLED
	if(UpStartUSART[5]	==	ENABLE)
	{
		if(Cap_USART[1]	!=	0x11)
		{
			Display_GB2312_String(46,160,24, "旁路电容错误！！！",RED,LGRAYBLUE); 							//15x16汉字
			Error_Flag=1;
		}
  }
	
	//MOTO错误输出OLED
	if(MT_USART[1]	!=	0x11)
	{
		Display_GB2312_String(46,160,24, "电机电感错误！！！",RED,LGRAYBLUE); 							//15x16汉字
		Error_Flag=1;
	}
	//TM错误输出OLED
	if(Detect_USART[3]		!=	0x11)
	{
		Display_GB2312_String(46,160,24, "头片温度错误！！！",RED,LGRAYBLUE); 							//15x16汉字
		Error_Flag=1;
	}
	
	while(LCD_HS()==0x00)																											//接地测试
	{
		Display_Arial_String(72,118,16, "OFF",RED,LGRAYBLUE);   		//ASC 12x16点阵(Arial类型)
		if(TM_Test()!=0x11	)
		{
			Display_GB2312_String(46,160,24, "机架未接地！！！",RED,LGRAYBLUE); 							//15x16汉字
			Error_Flag=1;
		}
	}
	Display_Arial_String(72,118,16, "ON ",DARKBLUE,LGRAYBLUE);   		//ASC 12x16点阵(Arial类型)
	
	
	if(Error_Flag)
	{
		Display_GB2312_String(46,185,24, "按键后重新进行检测",RED,LGRAYBLUE); 		//15x16汉字
		nVCC_EN(Bit_SET);
		Display_Arial_String(232,118,16, "OFF",RED,LGRAYBLUE);   									//ASC 12x16点阵(Arial类型)
		BEEP_Error();
		nLED_Link(Bit_SET);	
		while(Key1()==Bit_RESET);
		LCD_DataClear();	
		nLED_ERROR(Bit_SET);
		return 0xAA;
	}
		Display_GB2312_String(46,170,24, "测试成功打印测试纸",DARKBLUE,LGRAYBLUE); 		//15x16汉字
		return 0x11;
}




/******************************************
* 函数：void	LCD_Variable(void)
* 功能：显示实时数据（光耦，温度）
* 入口参数：无
* 出口参数：无	
******************************************/
void	LCD_Variable(void)
{
	LCD_ShowIntNum(72,101,Detect_USART[2],4,DARKBLUE,LGRAYBLUE,16);   			//ASC 12x16点阵(Arial类型)
	LCD_ShowIntNum(232,101,Detect_USART[4],4,DARKBLUE,LGRAYBLUE,16);   			//ASC 12x16点阵(Arial类型)
}
	



/******************************************
* 函数：void	LCD_DataClear(void)
* 功能：清除测试所得数据
* 入口参数：无
* 出口参数：无	
******************************************/
void	LCD_DataClear(void)
{
		Display_Arial_String(72,67,16, 		"       ",	DARKBLUE,LGRAYBLUE);   		//ASC 12x16点阵(Arial类型)
		Display_Arial_String(232,67,16, 		"       ",DARKBLUE,LGRAYBLUE);   		//ASC 12x16点阵(Arial类型)
		Display_Arial_String(72,84,16, 		"       ",	DARKBLUE,LGRAYBLUE);   		//ASC 12x16点阵(Arial类型)
		Display_Arial_String(232,84,16, 	"        ",	DARKBLUE,LGRAYBLUE);   		//ASC 12x16点阵(Arial类型)
		Display_Arial_String(72,101,16,		"   ",			DARKBLUE,LGRAYBLUE);   		//ASC 12x16点阵(Arial类型)
		Display_Arial_String(97,101,16,		"   ",			DARKBLUE,LGRAYBLUE); 			//15x16汉字
		Display_Arial_String(232,101,16,	"       ",	DARKBLUE,LGRAYBLUE);   	//ASC 12x16点阵(Arial类型)
		Display_Arial_String(72,118,16, 	"    ",			RED,LGRAYBLUE);   					//ASC 12x16点阵(Arial类型)
	//	Display_Arial_String(232,118,16, 	"    ",			RED,LGRAYBLUE);   					//ASC 12x16点阵(Arial类型)
		LCD_Fill(40,160,300,220,LGRAYBLUE);
		Display_GB2312_String(46,160,24, "欢迎使用本公司机芯",RED,LGRAYBLUE); 		//15x16汉字
		Display_GB2312_String(46,185,24, "使用中请注意防静电",RED,LGRAYBLUE); 		//15x16汉字
}

/******************************************
* 函数：void	LCD_Layout(void)	
* 功能：LCD显示背景布局
* 入口参数：无
* 出口参数：无	
******************************************/
void	LCD_Layout(void)	
{
		LCD_Fill(0,0,LCD_W,LCD_H,LGRAYBLUE);				//设置背景色
		LCD_ShowPicture(0,0,320,49);								//打印固定图片
		Display_GB2312_String(0,50,16, 		"测试治具：",BLACK,LGRAYBLUE); 		//15x16汉字
		Display_GB2312_String(0,67,16,		"工作电压：",BLACK,LGRAYBLUE); 		//15x16汉字
		Display_GB2312_String(160,67,16,	"旁路电容：",BLACK,LGRAYBLUE); 		//15x16汉字
		Display_GB2312_String(0,84,16, 		"电机电感：",BLACK,LGRAYBLUE); 		//15x16汉字
		Display_GB2312_String(160,84,16, 	"打印速度：",BLACK,LGRAYBLUE); 		//15x16汉字
		Display_GB2312_String(0,101,16, 	"头片温度：",BLACK,LGRAYBLUE); 		//15x16汉字
		Display_GB2312_String(160,101,16, "光耦检测：",BLACK,LGRAYBLUE); 		//15x16汉字
		Display_GB2312_String(0,118,16, 	"机架接地：",BLACK,LGRAYBLUE); 		//15x16汉字
		Display_GB2312_String(160,118,16, "电源开关：",BLACK,LGRAYBLUE); 		//15x16汉字
		Display_Arial_String(4,135,16, 		"************************************",DARKBLUE,LGRAYBLUE);   		//ASC 12x16点阵(Arial类型)
		Display_Arial_String(4,145,16, 		"************************************",DARKBLUE,LGRAYBLUE);   		//ASC 12x16点阵(Arial类型)
		Display_Arial_String(72,50,16, 		"AG_03",DARKBLUE,LGRAYBLUE);   		//ASC 12x16点阵(Arial类型)
		Display_GB2312_String(46,160,24, 	"欢迎使用本公司机芯",RED,LGRAYBLUE); 		//15x16汉字
		Display_GB2312_String(46,185,24, 	"使用中请注意防静电",RED,LGRAYBLUE); 		//15x16汉字
		Display_GB2312_String(0,50,16, 		"测",BLACK,LGRAYBLUE); 						//15x16汉字
}
	
	

/******************************************
* 函数：u8	LCD_HS(void)
* 功能：胶辊/接地连接检测并显示 
* 入口参数：无
* 出口参数：机架接地		0x11
						机架未接地	0x00
******************************************/
u8	LCD_HS(void)
{
	u8	i;
	if(H_S()		==	Bit_RESET)
	{
		Delay1ms(100);
		if(H_S()		==	Bit_RESET)
		{
//			printf("VDD Link OK!");
			i	= 0x11;
		}
		else 
		{
			i	= 0x00;
		}
	}
	return	i;
}	

	
	
	
	
