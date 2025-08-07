/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include 	"stm32f10x_it.h"
//#include 	"GetCharacterFont.h"
#include 	"Config.h"
#include	"TPG.h"
#include 	"printf.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#define USART_PLAY 0
extern BitAction		PAPER_Key;
char R_n,R_Buff[600],R_Flag;
char P_Buff[800],P_n,P_c_n;
u8 P_Buff_Const[300][48];

// typedef enum {
//     RX_STATE_NORMAL,    // 接收普通数据
//     RX_STATE_HEADER,    // 接收长宽数据头
//     RX_STATE_DIMENSION  // 接收长宽数据
// } UART_RxState;

// typedef struct {
//     u16 width;
//     u16 height;
// } FontDimension;
volatile UART_RxState rx_state = RX_STATE_NORMAL;
volatile FontDimension font_dim = {0};
volatile uint8_t dim_buffer[4];  // 存储长宽数据(2字节width + 2字节height)
volatile uint8_t dim_index = 0;

void USART1_IRQHandler(void)
{
	uint8_t ucTemp;
	if (USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET) {
			ucTemp = USART_ReceiveData(USART1);
			// ucTemp = (uint8_t)(USART1->DR & 0xFF);
			R_Buff[R_n++] = ucTemp;
			P_Buff[P_n++] = ucTemp;
			
			#if USART_PLAY
			switch (rx_state)
			{
				case RX_STATE_NORMAL:
					if (R_n == 1 && ucTemp == '#') { // 如果接收到'#'，表示开始接收长宽数据
						rx_state = RX_STATE_HEADER; // 开始接收长宽数据头
						R_n = 0; // 重置索引
					} else { // 接收到图片数据
						P_Buff[P_n++] = ucTemp;
						// Usart_SendByte(USART1,ucTemp); // 回显接收到的数据
						if(ucTemp == '@' && P_n == 49) { // 如果接收到'@'，表示结束接收一行数据
							P_n--; // 去掉最后一个字符
							P_Buff[P_n] = '\0'; //字符串结束符
							memcpy(P_Buff_Const[P_c_n++], P_Buff, P_n);
							P_n = 0;
							Usart_SendString(USART1, "OK1\r\n");
						} else if(ucTemp == '\n') {// 如果接收到换行符或回车符
							P_n--; //去掉最后一个字符
							P_Buff[P_n] = '\0'; //字符串结束符
							memcpy(P_Buff_Const[P_c_n++], P_Buff, P_n);
							P_Buff[0] = '\0'; //清空P_Buff
							P_c_n = 0; //当接收完成则置零
							P_n = 0; //重置索引
							R_n = 0;
							Usart_SendString(USART1, "OK2\r\n");
							R_Flag = 1;
						}
					}
					break;

				case RX_STATE_HEADER:
					if (R_n < 3) { // 接收4个字节的长宽数据头
						// R_Buff[R_n] = '\0'; // 字符串结束符
					} else {
						rx_state = RX_STATE_DIMENSION; // 切换到接收长宽数据状态
					}
					break;

				case RX_STATE_DIMENSION:
					if (R_n >= 4) { // 接收到足够的字节后，处理长宽数据
						// FontDimension dim;
						font_dim.height = (uint16_t)(R_Buff[0] | (R_Buff[1] << 8));  // 小端序
						font_dim.width = (uint16_t)(R_Buff[2] | (R_Buff[3] << 8));
						R_n = 0; // 重置索引
						rx_state = RX_STATE_NORMAL; // 重置状态机
						
						Usart_SendByte(USART1, font_dim.width); // 发送确认字符
					}
					break;
			}
			#endif

			#if !USART_PLAY
			if(P_n == 49 && ucTemp == '@') 
			{
				P_n--; // 去掉最后一个字符
				P_Buff[P_n] = '\0'; //字符串结束符
				memcpy(P_Buff_Const[P_c_n++], P_Buff, P_n);
				P_n = 0;
				Usart_SendString(USART1, "OK\r\n");
			}
			else if(ucTemp == '\n') // 如果接收到换行符或回车符
			{
				P_n--; //去掉最后一个字符
				R_Buff[R_n-1] = '\0'; //字符串结束符
				R_n = 0; //重置索引

				P_Buff[P_n] = '\0'; //字符串结束符
				memcpy(P_Buff_Const[P_c_n++], P_Buff, P_n);
				P_Buff[0] = '\0'; //清空P_Buff
				P_c_n = 0; //当接收完成则置零
				P_n = 0; //重置索引
				Usart_SendString(USART1, "OK\r\n");
				R_Flag = 1;
				
			}
			else if(R_n >= sizeof(R_Buff) - 1) //防止缓冲区溢出
			{
				R_Buff[R_n] = '\0'; //确保字符串结束
				R_n = 0; //重置索引
			}
			else if(P_n >= sizeof(P_Buff_Const) - 1) //防止缓冲区溢出
			{
				P_Buff[P_n] = '\0'; //确保字符串结束
				P_n = 0; //重置索引
			}
			#endif
		} 
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
}

/*******************************************************************************
* Function Name  : USART3_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void USART3_IRQHandler(void)
{

	uint8_t ucTemp;
	if (USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET) 
		{
			ucTemp = USART_ReceiveData(USART3);
			USART_SendData(USART3,ucTemp);
		}
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);

}



extern	u16 			Detect_USART[6];
extern	BitAction	PAPER_Key;
extern	BitAction	PAPER_ConON;
extern 	u16 			UpStartUSART[14];
extern 	u8				Step[]; 				//电机走步顺序
extern 	u8				res;
extern 	u8				NHeatTime;														//加热间隔时间
extern	BitAction	STB_ON,STB_OFF;

FunctionalState MOTO_2Esc=ENABLE;
FunctionalState	MOTO_EN_It	= 	DISABLE;
FunctionalState	TPH_EN_It		=		DISABLE;
FunctionalState	TPH_EN;
FunctionalState	MOTO_EN;
u8	cis=0,STB_L_Num=0,STB_H_Num=0;
u16	It_Num;
u32	It_Loop=0;
u8 PAPER_ON=0,PAPER_OFF=0;
/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : This function handles TIM3 global interrupt request.200us
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
			//进纸判断*******************
			if(ADC_Num[1]< 550)			//ADC默认值 判断进纸	2200/4
			{
				PAPER_ON++;
				PAPER_OFF=0;
			}
			else
			{
				PAPER_ON=0;
				PAPER_OFF++;
			}
			PAPER_ON>100?(PAPER_Key=Bit_SET):(PAPER_Key);
			PAPER_OFF>100?(PAPER_Key=Bit_RESET):(PAPER_Key);
			PAPER_Key!=Bit_SET?MOTO_EN=DISABLE,TPH_EN=DISABLE:(PAPER_Key);
			//////
			//连接测试判断**************
			//TPH加热
			if(TPH_EN	==	ENABLE)																//总TPH加热使能判断
			{
				if(TPH_EN_It	==	ENABLE)													//中断执行TPH加热使能判断
				{
					if(STB_H_Num<=(UpStartUSART[1]))
					{
						TPH_STB1(STB_ON);
						STB_H_Num++;
					}
					else
					{
						TPH_STB1(STB_OFF);
						STB_L_Num++;
						if(STB_L_Num	>	10-UpStartUSART[1])
						{
							STB_H_Num=0;
							STB_L_Num=0;
						}
					}
				}
				else 
					{
						TPH_STB1(Bit_RESET);
						TPH_EN_It		=	DISABLE;
					}
			}
			else 
			{
						TPH_STB1(Bit_RESET);	
						TPH_EN_It		=	DISABLE;
			}
			/////////////
			//电机走步
			////////////
			/**********************
			电机走步
			**********************/
			if(MOTO_EN	==	ENABLE)															//总步进电机使能判断
			{
				if(MOTO_EN_It == ENABLE)													//中断任务执行走步判断
				{		
					if(It_Num>=UpStartUSART[2])									//步进电机步进频率，直接工作第一步
					{
			//			TPH_STB(Bit_RESET);	
						res &= 0x07;	
						MotoStep(Step[res]);						
						It_Num=0;		
						res++;	
						cis++;
					}						
				}			
			}
			if(cis == 4)		
			{
				MOTO_EN_It 	= 	DISABLE;
				cis = 0;
			}	
			///////////////
			///////////////
			//0.1ms时间计数
			It_Num++;
			It_Loop++;
	}
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}


/*******************************************************************************
* Function Name  : ADC1_2_IRQHandler
* Description    : This function handles ADC1 and ADC2 global interrupts requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*
extern uint16_t	ADC_num;
extern FlagStatus	Flag;
void ADC1_2_IRQHandler(void)
{
	if((ADC_GetITStatus(ADC1,ADC_IT_EOC))	!=	RESET)
	{
		ADC_num	=	ADC_GetConversionValue(ADC1);
		Flag	=	SET;
	}
	ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
}*/

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
