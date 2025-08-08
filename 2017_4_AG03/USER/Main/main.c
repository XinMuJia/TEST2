/**********************************************
*ÿ����¼�޸ĳ�����
*ProgID: ��λ��� ��2001
*2�ǹ��ܱ��,001Ϊ��Ӧ���ܲ�Ʒ���
*˵����
			1 �������Գ���
			2 ���������������
			3	����TPH���Գ���
			4 Ʒ�ܳ����������
			5 Ʒ��TPH���Գ���
			6	����ά�޲��Գ���
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

extern	u32					Cap_USART[2];								//������·����
extern	u16					Detect_USART[];								//VH,TM,RAY����
extern	u16					MT_USART[];									//������Խ��
extern 	u16 				UpStartUSART[];								//ѡ���ͺŲ�������
extern	BitAction		STB_ON,STB_OFF;									//��ֽ�ź�
extern 	char				*Type_ID;
extern	u16					It_Num;
extern	u8					Link_G_en;									//�Ƿ����ͷƬ�ӵ�
extern	u8					Print_Minloopnum;
extern	u8					TPH_STB_EN;
extern FunctionalState  MOTO_H_Set;
extern char R_Buff[30], R_n, R_Flag,P_n;
extern u8 P_Buff_Const[300][48]; // ��ӡͼ��ĳ�������
extern volatile FontDimension font_dim;

u16					ADC_Num[3];										//ADC����
BitAction		PAPER_Key;
char 				TPH_Cache[10][24];
u8 					TPH_LAN_TEST;
#define RUN 1

int main(void)
{
	u8	ls;
	u8	lps;
	InitHardWare();													//MCU�����ʼ��
	if(Key1()	!=	Bit_RESET)
	{
		MOTO_H_Set=ENABLE;
	}	
	MOTO_HzGet();
	UpStart();														//���û�о��ʼ������
	//LCD_Init();													//LCD��ʼ��
	//LCD_Layout();													//LCD��ʾ��������
	Cap_Test(DISABLE);	
	nLED_FRONT(Bit_RESET);											//��ʼ�����ָʾ��
	while(1)
	{
Retest:		
		// if(Key1()	!=	Bit_RESET)							//��������
		if(1)
		{
//			Display_Arial_String(232,118,16, "ON ",DARKBLUE,LGRAYBLUE);
//			Display_GB2312_String(46,170,24, "���ڲ��Բ�Ʒ������",YELLOW,LGRAYBLUE);
			nLED_Link(Bit_RESET);	
			nVCC_EN(Bit_RESET);
			//��ӡǰ��������		
  			// TM_Test();															//����Ƭ�¶Ȳ���
			// RAY_Test();														//����������
			// VH_Test();															//VH����
			// MT_Test();															//�����в��� 
			// Cap_Test(ENABLE);													//���ݲ���
			//if(LCD_Testout()!=0x11){goto Retest;}								//���ԵĽ���жϲ�LCD���
			//�������***
			#if !RUN
			u8 step_sequence[8] = {0x05,0x01,0x09,0x08,0x0a,0x02,0x06,0x04}; 
			u8 i;
			VH_EN(Bit_SET);
			MotoStep(0x00);
			for(i = 0; i < 100; i++)   // ת��100��
			{
				MotoStep(step_sequence[i % 8]);  // ѭ��ִ��8������
				// FeedNDotLine(2);
				Delay1ms(1);  // ����ת��
			}
			MotoStep(0x00);
			VH_EN(Bit_RESET);
			#endif
			// TPH_Printinit();						//��ӡǰ TPH��ʼ��

			// while (PAPER_Key == Bit_SET && R_Flag == 1)		//�ȴ�����
			// {
			// 	for(lps=64;lps>0;lps--)
			// 	{
			// 		TPH_Loop1();
			// 	}	
			// 	TPH_PrintString(0,"                      ",24);
			// 	TPH_PrintString(0, R_Buff, 24); //��ӡ���յ����ַ���
			// 	TPH_PrintString(0,"                      ",24);
			// 	TPH_PrintString(0," \\\\\\\\\\| ",24);
			// 	TPH_PrintString(0,"                      ",24);
			// 	R_Flag = 0; // ���ñ�־λ
			// 	R_Buff[0] = '\0';
			// } TPH_Esc(); //������ӡ
			// nVCC_EN(Bit_SET);
			// for (int i = 0; i < 100; i++)
			// MotoOneStep();
			// nVCC_EN(Bit_SET);

			#if RUN
			ls=0;
			while(PAPER_Key	!=	Bit_SET		||	ls	==	0 )
			{
				if(It_Num>5000)													//ÿX00ms ���һ��HPT�¶Ⱥ͹���ȡֵ
				{
					It_Num=0;
					if(TM_Test()!=0x11) //�ж��¶ȷ�Χ
						{
							nVCC_EN(Bit_SET);
							Display_Arial_String(232,118,16, "OFF",RED,LGRAYBLUE);  
							LCD_DataClear();
							goto Retest;
						}
					RAY_Test();															//����������
					LCD_Variable();	
					if(Detect_USART[5]	==	0x11)				//�����ֵ���
						{
								ls	= 1;
						}	
					}
				}
			
			/*******************************
			��ӡ������
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
			TPH_Printinit();						//��ӡǰ TPH��ʼ��

			while (PAPER_Key == Bit_SET && R_Flag == 1)		//�ȴ�����
			{
				TPH_PrintString(0,"hello",24);
				for(lps=64;lps>0;lps--)
				{
					TPH_Loop1();
				}	
				// TPH_PrintString(0,"                      ",24);
				// TPH_PrintString(0, R_Buff, 24); //��ӡ���յ����ַ���
				// TPH_PrintString(0,"                      ",24);
				// TPH_PrintString(0," |||||||||| ",24);
				// TPH_PrintString(0,"                      ",24);
				
				// Load_Font_Config(); // ����ͼƬ����
				TPH_PrintImage_m2((const unsigned char *)P_Buff_Const,font_dim.height,font_dim.width); // ��ӡͼ��,�Ӵ��ڻ�ȡ��ģ
				// TPH_PrintImage_m2((const unsigned char *)P_Buff_Const,MY_FONT_ROWS,MY_FONT_COLS); // ��ӡͼ��,���ļ���ȡ��ģ
				// TPH_PrintImage_m2((const unsigned char *)my_font,MY_FONT_ROWS,MY_FONT_COLS); // ��ӡͼ��,���ļ���ȡ��ģ
				
				R_Flag = 0; // ���ñ�־λ
				memset(P_Buff_Const[0], '\0', sizeof(P_Buff_Const[0]));
			} 
			// MT_SW(Bit_RESET);	
			

			

			// do
			// 	{	
			// 		//STB�ڿ��ӡ
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
			// 		//���ݴ�ӡ
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
			Display_GB2312_String(92,160,24, "��ӡ���",DARKBLUE,LGRAYBLUE);
			while(Key1()	==	Bit_SET)		//��ⰴ�����߲��Բ�Ʒ�뿪������
			{
				if(It_Num>2500)						//ÿ500ms ���һ��HPT�¶Ⱥ͹���ȡֵ
				{
					It_Num=0;
					TM_Test();															//����Ƭ�¶Ȳ���
					RAY_Test();															//����������
//					LCD_Variable();	
				}
			}
			nVCC_EN(Bit_SET);
//			Display_Arial_String(232,118,16, "OFF",RED,LGRAYBLUE);   		//ASC 12x16����(Arial����)
//			LCD_DataClear();
			#endif
		}
	}
}




/******************************************************************************
*����˵����TPH����ͬһ�б���
*������ݣ�d��ͷ������ 
					 num�����ӵı���
					 len������������λ��
					 n,c�����ӵĺ�׺
*����ֵ��  ��
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
*����˵����m^n
*������ݣ�m:���� n:ָ��
*����ֵ��  result
******************************************************************************/
// u32 num_pow(u8 m,u8 n)
// {
// 	u32 result=1;
// 	while(n--)result*=m;    
// 	return result;
// }

//������
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
*����˵����TPH��ӡͼƬ����x'y
*������ݣ���
*����ֵ��  ��
******************************************************************************/
// void Load_Font_Config() {
//     FIL file;
//    f_open(&file, "font_config.bin", FA_READ);
//    f_read(&file, &MY_FONT_ROWS, 2, NULL);
//    f_read(&file, &MY_FONT_COLS, 2, NULL);
//    f_close(&file);
// }










