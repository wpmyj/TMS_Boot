#ifndef __CAN_ADDRES_H__
#define __CAN_ADDRES_H__

#include "stm32f4xx.h"
/*MTDCCU定义输入GPIO端口*/
#define  MTDCCU_CANADD0_RCC      	 RCC_AHB1Periph_GPIOF
#define  MTDCCU_CANADD0_GPIO 			 GPIOF
#define  MTDCCU_CANADD0_GPIO_PIN 	 GPIO_Pin_0
#define  MTDCCU_CANADD0_READ()		 GPIO_ReadInputDataBit(MTDCCU_CANADD0_GPIO,MTDCCU_CANADD0_GPIO_PIN)

#define  MTDCCU_CANADD1_RCC      	 RCC_AHB1Periph_GPIOF
#define  MTDCCU_CANADD1_GPIO 			 GPIOF
#define  MTDCCU_CANADD1_GPIO_PIN 	 GPIO_Pin_1
#define  MTDCCU_CANADD1_READ()		 GPIO_ReadInputDataBit(MTDCCU_CANADD1_GPIO,MTDCCU_CANADD1_GPIO_PIN)

#define  MTDCCU_CANADD2_RCC      	 RCC_AHB1Periph_GPIOF
#define  MTDCCU_CANADD2_GPIO 			 GPIOF
#define  MTDCCU_CANADD2_GPIO_PIN 	 GPIO_Pin_2
#define  MTDCCU_CANADD2_READ()		 GPIO_ReadInputDataBit(MTDCCU_CANADD2_GPIO,MTDCCU_CANADD2_GPIO_PIN)

#define  MTDCCU_CANADD3_RCC      	 RCC_AHB1Periph_GPIOF
#define  MTDCCU_CANADD3_GPIO 			 GPIOF
#define  MTDCCU_CANADD3_GPIO_PIN 	 GPIO_Pin_3
#define  MTDCCU_CANADD3_READ()		 GPIO_ReadInputDataBit(MTDCCU_CANADD3_GPIO,MTDCCU_CANADD3_GPIO_PIN)

#define  MTDCCU_CANADD4_RCC      	 RCC_AHB1Periph_GPIOF
#define  MTDCCU_CANADD4_GPIO 			 GPIOF
#define  MTDCCU_CANADD4_GPIO_PIN 	 GPIO_Pin_4
#define  MTDCCU_CANADD4_READ()		 GPIO_ReadInputDataBit(MTDCCU_CANADD4_GPIO,MTDCCU_CANADD4_GPIO_PIN)

#define  MTDCCU_CANADD5_RCC      	 RCC_AHB1Periph_GPIOF
#define  MTDCCU_CANADD5_GPIO 			 GPIOF
#define  MTDCCU_CANADD5_GPIO_PIN 	 GPIO_Pin_5
#define  MTDCCU_CANADD5_READ()		 GPIO_ReadInputDataBit(MTDCCU_CANADD5_GPIO,MTDCCU_CANADD5_GPIO_PIN)

#define  MTDCCU_CANADD6_RCC      	 RCC_AHB1Periph_GPIOF
#define  MTDCCU_CANADD6_GPIO 			 GPIOF
#define  MTDCCU_CANADD6_GPIO_PIN 	 GPIO_Pin_12
#define  MTDCCU_CANADD6_READ()		 GPIO_ReadInputDataBit(MTDCCU_CANADD6_GPIO,MTDCCU_CANADD6_GPIO_PIN)

#define  MTDCCU_CANADD7_RCC      	 RCC_AHB1Periph_GPIOF
#define  MTDCCU_CANADD7_GPIO 			 GPIOF
#define  MTDCCU_CANADD7_GPIO_PIN 	 GPIO_Pin_13
#define  MTDCCU_CANADD7_READ()		 GPIO_ReadInputDataBit(MTDCCU_CANADD7_GPIO,MTDCCU_CANADD7_GPIO_PIN)

/*CCULED定义输入GPIO端口*/
#define  CCULED_CANADD0_RCC      	 RCC_AHB1Periph_GPIOF
#define  CCULED_CANADD0_GPIO 			 GPIOF
#define  CCULED_CANADD0_GPIO_PIN 	 GPIO_Pin_6
#define  CCULED_CANADD0_READ()		 GPIO_ReadInputDataBit(CCULED_CANADD0_GPIO,CCULED_CANADD0_GPIO_PIN)

#define  CCULED_CANADD1_RCC      	 RCC_AHB1Periph_GPIOF
#define  CCULED_CANADD1_GPIO 			 GPIOF
#define  CCULED_CANADD1_GPIO_PIN 	 GPIO_Pin_7
#define  CCULED_CANADD1_READ()		 GPIO_ReadInputDataBit(CCULED_CANADD1_GPIO,CCULED_CANADD1_GPIO_PIN)

#define  CCULED_CANADD2_RCC      	 RCC_AHB1Periph_GPIOF
#define  CCULED_CANADD2_GPIO 			 GPIOF
#define  CCULED_CANADD2_GPIO_PIN 	 GPIO_Pin_8
#define  CCULED_CANADD2_READ()		 GPIO_ReadInputDataBit(CCULED_CANADD2_GPIO,CCULED_CANADD2_GPIO_PIN)

#define  CCULED_CANADD3_RCC      	 RCC_AHB1Periph_GPIOF
#define  CCULED_CANADD3_GPIO 			 GPIOF
#define  CCULED_CANADD3_GPIO_PIN 	 GPIO_Pin_9
#define  CCULED_CANADD3_READ()		 GPIO_ReadInputDataBit(CCULED_CANADD3_GPIO,CCULED_CANADD3_GPIO_PIN)

#define  CCULED_CANADD4_RCC      	 RCC_AHB1Periph_GPIOF
#define  CCULED_CANADD4_GPIO 			 GPIOF
#define  CCULED_CANADD4_GPIO_PIN 	 GPIO_Pin_15
#define  CCULED_CANADD4_READ()		 GPIO_ReadInputDataBit(CCULED_CANADD4_GPIO,CCULED_CANADD4_GPIO_PIN)

#define  CCULED_CANADD5_RCC      	 RCC_AHB1Periph_GPIOF
#define  CCULED_CANADD5_GPIO 			 GPIOF
#define  CCULED_CANADD5_GPIO_PIN 	 GPIO_Pin_14
#define  CCULED_CANADD5_READ()		 GPIO_ReadInputDataBit(CCULED_CANADD5_GPIO,CCULED_CANADD5_GPIO_PIN)

#define  CCULED_CANADD6_RCC      	 RCC_AHB1Periph_GPIOF
#define  CCULED_CANADD6_GPIO 			 GPIOF
#define  CCULED_CANADD6_GPIO_PIN 	 GPIO_Pin_13
#define  CCULED_CANADD6_READ()		 GPIO_ReadInputDataBit(CCULED_CANADD6_GPIO,CCULED_CANADD6_GPIO_PIN)

#define  CCULED_CANADD7_RCC      	 RCC_AHB1Periph_GPIOF
#define  CCULED_CANADD7_GPIO 			 GPIOF
#define  CCULED_CANADD7_GPIO_PIN 	 GPIO_Pin_12
#define  CCULED_CANADD7_READ()		 GPIO_ReadInputDataBit(CCULED_CANADD7_GPIO,CCULED_CANADD7_GPIO_PIN)

/*MTDMFD定义输入GPIO端口*/
#define  MTDMFD_CANADD0_RCC      	 RCC_AHB1Periph_GPIOC
#define  MTDMFD_CANADD0_GPIO 			 GPIOC
#define  MTDMFD_CANADD0_GPIO_PIN 	 GPIO_Pin_8
#define  MTDMFD_CANADD0_READ()		 GPIO_ReadInputDataBit(MTDMFD_CANADD0_GPIO,MTDMFD_CANADD0_GPIO_PIN)

#define  MTDMFD_CANADD1_RCC      	 RCC_AHB1Periph_GPIOC
#define  MTDMFD_CANADD1_GPIO 			 GPIOC
#define  MTDMFD_CANADD1_GPIO_PIN 	 GPIO_Pin_7
#define  MTDMFD_CANADD1_READ()		 GPIO_ReadInputDataBit(MTDMFD_CANADD1_GPIO,MTDMFD_CANADD1_GPIO_PIN)

#define  MTDMFD_CANADD2_RCC      	 RCC_AHB1Periph_GPIOC
#define  MTDMFD_CANADD2_GPIO 			 GPIOC
#define  MTDMFD_CANADD2_GPIO_PIN 	 GPIO_Pin_6
#define  MTDMFD_CANADD2_READ()		 GPIO_ReadInputDataBit(MTDMFD_CANADD2_GPIO,MTDMFD_CANADD2_GPIO_PIN)

#define  MTDMFD_CANADD3_RCC      	 RCC_AHB1Periph_GPIOG
#define  MTDMFD_CANADD3_GPIO 			 GPIOG
#define  MTDMFD_CANADD3_GPIO_PIN 	 GPIO_Pin_8
#define  MTDMFD_CANADD3_READ()		 GPIO_ReadInputDataBit(MTDMFD_CANADD3_GPIO,MTDMFD_CANADD3_GPIO_PIN)

#define  MTDMFD_CANADD4_RCC      	 RCC_AHB1Periph_GPIOA
#define  MTDMFD_CANADD4_GPIO 			 GPIOA
#define  MTDMFD_CANADD4_GPIO_PIN 	 GPIO_Pin_12
#define  MTDMFD_CANADD4_READ()		 GPIO_ReadInputDataBit(MTDMFD_CANADD4_GPIO,MTDMFD_CANADD4_GPIO_PIN)

#define  MTDMFD_CANADD5_RCC      	 RCC_AHB1Periph_GPIOA
#define  MTDMFD_CANADD5_GPIO 			 GPIOA
#define  MTDMFD_CANADD5_GPIO_PIN 	 GPIO_Pin_11
#define  MTDMFD_CANADD5_READ()		 GPIO_ReadInputDataBit(MTDMFD_CANADD5_GPIO,MTDMFD_CANADD5_GPIO_PIN)

#define  MTDMFD_CANADD6_RCC      	 RCC_AHB1Periph_GPIOA
#define  MTDMFD_CANADD6_GPIO 			 GPIOA
#define  MTDMFD_CANADD6_GPIO_PIN 	 GPIO_Pin_8
#define  MTDMFD_CANADD6_READ()		 GPIO_ReadInputDataBit(MTDMFD_CANADD6_GPIO,MTDMFD_CANADD6_GPIO_PIN)

#define  MTDMFD_CANADD7_RCC      	 RCC_AHB1Periph_GPIOC
#define  MTDMFD_CANADD7_GPIO 			 GPIOC
#define  MTDMFD_CANADD7_GPIO_PIN 	 GPIO_Pin_9
#define  MTDMFD_CANADD7_READ()		 GPIO_ReadInputDataBit(MTDMFD_CANADD7_GPIO,MTDMFD_CANADD7_GPIO_PIN)

//宏参数化
#define __CANADD0_RCC(DEV)					DEV##_CANADD0_RCC
#define __CANADD0_GPIO(DEV)		 			DEV##_CANADD0_GPIO
#define __CANADD0_GPIO_PIN(DEV) 	 	DEV ##_CANADD0_GPIO_PIN
#define __CANADD0_READ(DEV)		 			DEV##_CANADD0_READ()

#define CANADD0_RCC(DEV)						__CANADD0_RCC(DEV)
#define CANADD0_GPIO(DEV)		 				__CANADD0_GPIO(DEV)	
#define CANADD0_GPIO_PIN(DEV) 	 		__CANADD0_GPIO_PIN(DEV)
#define CANADD0_READ(DEV)		 				__CANADD0_READ(DEV)		

#define __CANADD1_RCC(DEV)					DEV##_CANADD1_RCC
#define __CANADD1_GPIO(DEV)		 				DEV##_CANADD1_GPIO
#define __CANADD1_GPIO_PIN(DEV) 	 		DEV ## _CANADD1_GPIO_PIN
#define __CANADD1_READ(DEV)		 				DEV##_CANADD1_READ()

#define CANADD1_RCC(DEV)					__CANADD1_RCC(DEV)
#define CANADD1_GPIO(DEV)		 			__CANADD1_GPIO(DEV)	
#define CANADD1_GPIO_PIN(DEV) 	 	__CANADD1_GPIO_PIN(DEV)
#define CANADD1_READ(DEV)		 			__CANADD1_READ(DEV)		

#define __CANADD2_RCC(DEV)					DEV##_CANADD2_RCC
#define __CANADD2_GPIO(DEV)		 				DEV##_CANADD2_GPIO
#define __CANADD2_GPIO_PIN(DEV) 	 		DEV ## _CANADD2_GPIO_PIN
#define __CANADD2_READ(DEV)		 				DEV##_CANADD2_READ()

#define CANADD2_RCC(DEV)					__CANADD2_RCC(DEV)
#define CANADD2_GPIO(DEV)		 			__CANADD2_GPIO(DEV)	
#define CANADD2_GPIO_PIN(DEV) 	 	__CANADD2_GPIO_PIN(DEV)
#define CANADD2_READ(DEV)		 			__CANADD2_READ(DEV)		

#define __CANADD3_RCC(DEV)					DEV##_CANADD3_RCC
#define __CANADD3_GPIO(DEV)		 				DEV##_CANADD3_GPIO
#define __CANADD3_GPIO_PIN(DEV) 	 		DEV ## _CANADD3_GPIO_PIN
#define __CANADD3_READ(DEV)		 				DEV##_CANADD3_READ()

#define CANADD3_RCC(DEV)					__CANADD3_RCC(DEV)
#define CANADD3_GPIO(DEV)		 			__CANADD3_GPIO(DEV)	
#define CANADD3_GPIO_PIN(DEV) 	 	__CANADD3_GPIO_PIN(DEV)
#define CANADD3_READ(DEV)		 			__CANADD3_READ(DEV)		

#define __CANADD4_RCC(DEV)					DEV##_CANADD4_RCC
#define __CANADD4_GPIO(DEV)		 				DEV##_CANADD4_GPIO
#define __CANADD4_GPIO_PIN(DEV) 	 		DEV ## _CANADD4_GPIO_PIN
#define __CANADD4_READ(DEV)		 				DEV##_CANADD4_READ()

#define CANADD4_RCC(DEV)					__CANADD4_RCC(DEV)
#define CANADD4_GPIO(DEV)		 			__CANADD4_GPIO(DEV)	
#define CANADD4_GPIO_PIN(DEV) 	 	__CANADD4_GPIO_PIN(DEV)
#define CANADD4_READ(DEV)		 			__CANADD4_READ(DEV)		

#define __CANADD5_RCC(DEV)					DEV##_CANADD5_RCC
#define __CANADD5_GPIO(DEV)		 				DEV##_CANADD5_GPIO
#define __CANADD5_GPIO_PIN(DEV) 	 		DEV ## _CANADD5_GPIO_PIN
#define __CANADD5_READ(DEV)		 				DEV##_CANADD5_READ()

#define CANADD5_RCC(DEV)					__CANADD5_RCC(DEV)
#define CANADD5_GPIO(DEV)		 			__CANADD5_GPIO(DEV)	
#define CANADD5_GPIO_PIN(DEV) 	 	__CANADD5_GPIO_PIN(DEV)
#define CANADD5_READ(DEV)		 			__CANADD5_READ(DEV)		

#define __CANADD6_RCC(DEV)					DEV##_CANADD6_RCC
#define __CANADD6_GPIO(DEV)		 				DEV##_CANADD6_GPIO
#define __CANADD6_GPIO_PIN(DEV) 	 		DEV ## _CANADD6_GPIO_PIN
#define __CANADD6_READ(DEV)		 				DEV##_CANADD6_READ()

#define CANADD6_RCC(DEV)					__CANADD6_RCC(DEV)
#define CANADD6_GPIO(DEV)		 			__CANADD6_GPIO(DEV)	
#define CANADD6_GPIO_PIN(DEV) 	 	__CANADD6_GPIO_PIN(DEV)
#define CANADD6_READ(DEV)		 			__CANADD6_READ(DEV)		

#define __CANADD7_RCC(DEV)					DEV##_CANADD7_RCC
#define __CANADD7_GPIO(DEV)		 				DEV##_CANADD7_GPIO
#define __CANADD7_GPIO_PIN(DEV) 	 		DEV ## _CANADD7_GPIO_PIN
#define __CANADD7_READ(DEV)		 				DEV##_CANADD7_READ()

#define CANADD7_RCC(DEV)					__CANADD7_RCC(DEV)
#define CANADD7_GPIO(DEV)		 			__CANADD7_GPIO(DEV)	
#define CANADD7_GPIO_PIN(DEV) 	 	__CANADD7_GPIO_PIN(DEV)
#define CANADD7_READ(DEV)		 			__CANADD7_READ(DEV)		

void can_address_init(void);
uint8_t can_get_address(void);

#endif //__CAN_ADDRES_H__
