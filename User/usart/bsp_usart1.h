#ifndef _BSP_USART1_H
#define	_BSP_USART1_H

#include "stm32f10x.h"
#include <stdio.h>
#define uchar unsigned char 
#define uint unsigned char

void SendStrings(uchar * temp);
void SendChar(uchar temp);
void USART1_Config(void);
static void NVIC_Configuration(void);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
void USART2_Config(void);
void USART3_Config(void);
void USART_printf(USART_TypeDef* USARTx, uint8_t *Data,...);

#endif /* __USART1_H */
