#include <stdarg.h>
#include "stm32f4xx_usart.h"
#include "user_stdio.h"
#include "canfestival.h"
#include <stdbool.h>
#include <stdio.h>
#include "user_string.h"

int fputc(int ch,FILE* f)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
	USART_ClearFlag(USART1,USART_FLAG_TC);  
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	USART_SendData(USART1, (uint8_t) ch);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	/* Loop until the end of transmission */
  return ch;
}

