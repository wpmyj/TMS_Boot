/**
  ******************************************************************************
  * @file    STM32F4xx_IAP/inc/flash_if.h 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    10-October-2011
  * @brief   This file provides all the headers of the flash_if functions.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_IF_H
#define __FLASH_IF_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbyte */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbyte */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbyte */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbyte */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbyte */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbyte */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbyte */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbyte */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbyte */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbyte */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbyte */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbyte */

/* End of the Flash address */
#define USER_FLASH_END_ADDRESS        0x080FFFFF
/* Define the user application size */
//#define USER_FLASH_SIZE   (USER_FLASH_END_ADDRESS - APPLICATION_ADDRESS + 1)
extern uint32_t USER_FLASH_SIZE ;
extern uint32_t APPLICATION_ADDRESS ;

/* Define the address from where user application will be loaded.
   Note: the 1st sector 0x08000000-0x08003FFF is reserved for the IAP code */
//#define APPLICATION_ADDRESS   (uint32_t)0x08008000 
//extern uint32_t  APPLICATION_ADDRESS;

/* Exported macro ------------------------------------------------------------*/
#define CCU1_NODEID  				1		//总线控制器1：节点号 1
#define CCU2_NODEID	 				2		//总线控制器2：节点号 2	
#define MC1_MTD_NODEID	 		3		//MC1车监控装置：节点号 3
#define T_MTD_NODEID   	 		4		//T车监控装置：节点号4
#define M_MTD_NODEID		 		5		//M车监控装置： 节点号5
#define T1_MTD_NODEID				6		//T1车监控装置：节点号6
#define T2_MTD_NODEID   		7		//T2车监控装置：节点号7
#define MC2_MTD_NODEID   		8		//MC2车监控装置：节点号8
#define DDU1_NODEID					9   //监控显示器1：节点号9   (司机显示单元)
#define DDU2_NODEID   			10	//监控显示器2：节点号10
#define ERM1_NODEID					11	//事件记录仪1: 节点号11
#define ERM2_NODEID					12  //事件记录仪2：节点号12

#define MC1_MFD1_NODEID				0x31
#define MC1_MFD2_NODEID				0x32
#define MC1_MFD3_NODEID				0x33
#define MC1_MFD4_NODEID				0x34

#define T_MFD1_NODEID					0x41
#define T_MFD2_NODEID					0x42
#define T_MFD3_NODEID					0x43
#define T_MFD4_NODEID					0x44
		
#define M_MFD1_NODEID					0x51
#define M_MFD2_NODEID					0x52
#define M_MFD3_NODEID					0x53
#define M_MFD4_NODEID					0x54
		
#define T1_MFD1_NODEID					0x61
#define T1_MFD2_NODEID					0x62
#define T1_MFD3_NODEID					0x63
#define T1_MFD4_NODEID					0x64
		
#define T2_MFD1_NODEID					0x71
#define T2_MFD2_NODEID					0x72
#define T2_MFD3_NODEID					0x73
#define T2_MFD4_NODEID					0x74
		
#define MC2_MFD1_NODEID				0x81
#define MC2_MFD2_NODEID				0x82
#define MC2_MFD3_NODEID				0x83
#define MC2_MFD4_NODEID				0x84

#define  MTD_APP1_ADDRES_FLASH    	0x08010000 
#define  MTD_APP2_ADDRES_FLASH			0x08080000

extern void	App_JumpFunction(uint32_t AppAddress);

/* Exported functions ------------------------------------------------------- */
void FLASH_If_Init(void);
uint32_t FLASH_If_Erase(uint32_t StartSector);
uint32_t FLASH_If_Write(__IO uint32_t* FlashAddress, uint32_t* Data, uint32_t DataLength);
uint16_t FLASH_If_GetWriteProtectionStatus(void);
uint32_t FLASH_If_DisableWriteProtection(void);
uint32_t GetSector(uint32_t Address);
u32 FLASH_If_ReadWord(u32 faddr);		  	//读出字  
void FLASH_If_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead);   		//从指定地址开始读出指定长度的数据
#endif  /* __FLASH_IF_H */

/*******************(C)COPYRIGHT 2011 STMicroelectronics *****END OF FILE******/
