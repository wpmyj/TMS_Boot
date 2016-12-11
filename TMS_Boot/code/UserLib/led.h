#ifndef __LED_H
#define __LED_H
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#define MTDCCU_LED_GPIO			GPIOF
#define MTDCCU_LED_GPIO_Pin	GPIO_Pin_11
#define MTDCCU_LED_GPIO_RCC_AHB1Periph_GPIO	RCC_AHB1Periph_GPIOF

#define CCULED_LED_GPIO			GPIOE
#define CCULED_LED_GPIO_Pin	GPIO_Pin_7
#define CCULED_LED_GPIO_RCC_AHB1Periph_GPIO	RCC_AHB1Periph_GPIOE

#define MTDMFD_LED_GPIO			GPIOA
#define MTDMFD_LED_GPIO_Pin	GPIO_Pin_7
#define MTDMFD_LED_GPIO_RCC_AHB1Periph_GPIO	RCC_AHB1Periph_GPIOE


#define __LED_GPIO(DEV)	DEV ## _LED_GPIO
#define LED_GPIO(DEV)	__LED_GPIO(DEV)
#define __LED_GPIO_Pin(DEV)	DEV ## _LED_GPIO_Pin
#define LED_GPIO_Pin(DEV)	__LED_GPIO_Pin(DEV)
#define __LED_GPIO_RCC_AHB1Periph_GPIO(DEV)	DEV ## _LED_GPIO_RCC_AHB1Periph_GPIO
#define LED_GPIO_RCC_AHB1Periph_GPIO(DEV)	__LED_GPIO_RCC_AHB1Periph_GPIO(DEV)

extern void ledInit(void);
extern void led2_on(void );
extern void led2_off(void);
extern uint8_t led2_get_state(void);
#endif //__LED_H
