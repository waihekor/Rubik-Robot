#ifndef __USART__H__
#define __USART__H__

#include "stm32f10x.h"
#include "stdio.h"


extern u8 rece_flag;
extern u8 uart_rdata;           /*通过串口发来的字符*/
extern u8 rece_string[100];      /*存放接收到的字符串*/




u8 asc_to_num(u8 asc);
u8 num_to_asc(u8 num);				
void USART_SendChar(u8 siglechar);				
void USART1_Config(void);
void USART_SendString (unsigned char *str);
void USART_SendChar8(u8 num);
void USART_SendInt16(u16 num);
void NVIC_USART1_Configuration(void);


#endif
