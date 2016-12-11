#include "led.h"

void ledInit(void )
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(LED_GPIO_RCC_AHB1Periph_GPIO(Z_DEVICE), ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED_GPIO_Pin(Z_DEVICE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(LED_GPIO(Z_DEVICE), &GPIO_InitStructure);
}
void led2_on()
{
	GPIO_ResetBits(LED_GPIO(Z_DEVICE), LED_GPIO_Pin(Z_DEVICE));
}
void led2_off()
{
	GPIO_SetBits(LED_GPIO(Z_DEVICE), LED_GPIO_Pin(Z_DEVICE));
}
uint8_t led2_get_state()
{
	return GPIO_ReadOutputDataBit(LED_GPIO(Z_DEVICE), LED_GPIO_Pin(Z_DEVICE)) ?0:1;
}

