#ifndef __PRINTF_H_
#define __PRINTF_H_
#include "stm32f10x.h"
#include 	<stdio.h>

/*****************  发送一个字节 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);


/****************** 发送8位的数组 ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num);



/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);



/*****************  发送一个16位数 **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);


int fgetc(FILE *f);

int fputc(int ch, FILE *f);

#endif /* __PRINTF_H_ */
