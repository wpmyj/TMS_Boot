/**
  ******************************************************************************
  * @file    can_stm32.c
  * @author  Zhenglin R&D Driver Software Team
  * @version V1.0.0
  * @date    26/04/2015
  * @brief   This file is can_stm32 file.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdio.h>
#include "canfestival.h"
#include "stm32f4xx.h"
#include "led.h"
#include "user_stdio.h"
#include "timer_STM32.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
// Store the last timer value to calculate the elapsed time
static TIMEVAL last_time_set = TIMEVAL_MAX;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  setTimer
  * @param  value:Set time value 0x0000-0xffff
  * @retval NONE
  */
void setTimer(TIMEVAL value)
{
	//printf("led : %d\n",(int)value);
	TIM_SetAutoreload(TIM4, value);
}
/**
  * @brief  getElapsedTime
  * @param  NONE
	* @retval TIMEVAL:Return current timer value
  */
TIMEVAL getElapsedTime(void)
{
	uint16_t timer = TIM_GetCounter(TIM4);
	return timer > last_time_set ? timer - last_time_set : last_time_set - timer; 	
}

/**
  * @brief  TIM4_Configuration
  * @param  NONE
  * @retval NONE
  */
static void TIM4_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		/* Time Base configuration */
		TIM_TimeBaseStructure.TIM_Prescaler = 84- 1;//1us
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseStructure.TIM_Period = 0xffff;//65.535ms
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	  TIM_PrescalerConfig(TIM4, 84- 1, TIM_PSCReloadMode_Immediate);
    /* TIM IT enable */
		TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);
		/* TIM enable counter */
		TIM_Cmd(TIM4, ENABLE);
}
/**
  * @brief  NVIC_Configuration
  * @param  NONE
  * @retval NONE
  */
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the TIM5 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
  * @brief  RCC_Configuration
  * @param  NONE
  * @retval NONE
  */
static void RCC_Configuration(void)
{
		/* TIM4 clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
}
/**
  * @brief  TIM4_start
  * @param  NONE
  * @retval NONE
  */

void TIM4_start(void)
{
		RCC_Configuration();

		/* configure TIM4 for remote and encoder */
		NVIC_Configuration();
		TIM4_Configuration();
}
void TIM4_stop(void)	
{
	TIM_Cmd(TIM4, DISABLE);			
}
/**
  * @brief  initTimer
  * @param  NONE
  * @retval NONE
  */
void initTimer(void)
{
		TIM4_start();
}
/**
  * @brief  TIM4_IRQHandler
  * @param  NONE
  * @retval NONE
  */
#include "wdg.h"
void TIM4_IRQHandler(void)
{
	IWDG_Feed();
	if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
	{
		  //my_printf("led : 0x%x \r\n",led2_get_state());
			if(!led2_get_state())
			{
				led2_on();
			}
			else
			{
				led2_off();
			}
			last_time_set = TIM_GetCounter(TIM4);
			
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
			TimeDispatch();
		
	}
}
/******************* (C) COPYRIGHT 2015 Personal Electronics *****END OF FILE****/
