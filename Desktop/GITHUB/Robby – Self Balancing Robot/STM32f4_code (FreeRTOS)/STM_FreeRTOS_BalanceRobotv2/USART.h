#ifndef USART_H
#define USART_H

#include "stm32f4xx.h"

#define MAX_R_STRLEN 45 // this is the maximum string length of our string in characters
#define MAX_S_STRLEN 12

void init_USART1(uint32_t baudrate);
void USART_puts(USART_TypeDef* USARTx, volatile char *s);
 
void USART_Config(void);
void InterruptUSART(void);

#endif
