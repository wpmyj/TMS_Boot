#include "can_address.h"
#include "stm32f4xx.h"
#include "stdio.h"

void can_address_init()
{
		GPIO_InitTypeDef GPIO_InitStructure;
	/* 开启GPIO 的时钟  */
	RCC_AHB1PeriphClockCmd(CANADD0_RCC(Z_DEVICE) ,ENABLE);		//因为所有的引脚都使用的CPIOF
	RCC_AHB1PeriphClockCmd(CANADD1_RCC(Z_DEVICE) ,ENABLE);		//因为所有的引脚都使用的CPIOF
	RCC_AHB1PeriphClockCmd(CANADD2_RCC(Z_DEVICE) ,ENABLE);		//因为所有的引脚都使用的CPIOF
	RCC_AHB1PeriphClockCmd(CANADD3_RCC(Z_DEVICE) ,ENABLE);		//因为所有的引脚都使用的CPIOF
	RCC_AHB1PeriphClockCmd(CANADD4_RCC(Z_DEVICE) ,ENABLE);		//因为所有的引脚都使用的CPIOF
	RCC_AHB1PeriphClockCmd(CANADD5_RCC(Z_DEVICE) ,ENABLE);		//因为所有的引脚都使用的CPIOF
	RCC_AHB1PeriphClockCmd(CANADD6_RCC(Z_DEVICE) ,ENABLE);		//因为所有的引脚都使用的CPIOF
	RCC_AHB1PeriphClockCmd(CANADD7_RCC(Z_DEVICE) ,ENABLE);		//因为所有的引脚都使用的CPIOF
			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 	//GPIO_PuPd_UP;				//W H H 
 
	GPIO_InitStructure.GPIO_Pin= CANADD0_GPIO_PIN(Z_DEVICE);
	GPIO_Init(CANADD0_GPIO(Z_DEVICE), &GPIO_InitStructure);		//
	GPIO_InitStructure.GPIO_Pin= CANADD1_GPIO_PIN(Z_DEVICE);
	GPIO_Init(CANADD1_GPIO(Z_DEVICE), &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin= CANADD2_GPIO_PIN(Z_DEVICE);
	GPIO_Init(CANADD2_GPIO(Z_DEVICE), &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin= CANADD3_GPIO_PIN(Z_DEVICE);
	GPIO_Init(CANADD3_GPIO(Z_DEVICE), &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin= CANADD4_GPIO_PIN(Z_DEVICE);
	GPIO_Init(CANADD4_GPIO(Z_DEVICE), &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin= CANADD5_GPIO_PIN(Z_DEVICE);
	GPIO_Init(CANADD5_GPIO(Z_DEVICE), &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin= CANADD6_GPIO_PIN(Z_DEVICE);
	GPIO_Init(CANADD6_GPIO(Z_DEVICE), &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin= CANADD7_GPIO_PIN(Z_DEVICE);
	GPIO_Init(CANADD7_GPIO(Z_DEVICE), &GPIO_InitStructure);
}


/*电路图中 s2表示高位数据，s1表示低位数据*/
uint8_t can_get_address(void)
{
	uint8_t  value=0;
	value = (CANADD4_READ(Z_DEVICE) <<7) |(CANADD5_READ(Z_DEVICE) <<6) |(CANADD6_READ(Z_DEVICE) <<5) | (CANADD7_READ(Z_DEVICE) <<4)
				|(CANADD0_READ(Z_DEVICE) <<3) | (CANADD1_READ(Z_DEVICE) <<2)| (CANADD2_READ(Z_DEVICE) <<1)|(CANADD3_READ(Z_DEVICE) <<0)	  ;
	return ~value;		//将读到的数据按位取反
}
