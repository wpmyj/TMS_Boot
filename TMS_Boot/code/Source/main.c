#include "stm32f4xx.h"
#include "uart1.h"
#include "sysconfig.h"
#include "stdio.h"
#include "can_STM32.h"
#include "timer_STM32.h"
#include "timer.h"
#include "ObjDictMaster.h"
#include "led.h"
#include "user.h"
#include "iap.h"
#include "canfestival.h"
#include "iap.h"
#include "flash_if.h"
#include "can_address.h"
#include "TMS_Boot_ObjDict_m.h"
#include "TMS_Boot_ObjDict_s.h"
#define BOOT_MAGIC	0X4545
int main()
{
	UNS8 burn_flag = 0;
	CO_Data* md = &TMS_Boot_ObjDict_m_Data;
	CO_Data* sd = 0;
	UNS8 ChooseBoard;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  /* USART configuration -----------------------------------------------------*/
  USART_Config();
  ledInit( );
	can_address_init();
	ChooseBoard =  can_get_address();
  /* SysTick configuration ---------------------------------------------------*/
  SysTickConfig();
	

	DEBUG_PRINT("boot...\r\n");
	switch(ChooseBoard)
	{
		case MC1_MTD_NODEID:
		case M_MTD_NODEID:
		case MC2_MTD_NODEID:			
			APPLICATION_ADDRESS = MTD_APP1_ADDRES_FLASH; 
			USER_FLASH_SIZE = (USER_FLASH_END_ADDRESS - APPLICATION_ADDRESS + 1);
		break;
		
		case T_MTD_NODEID:
		case T1_MTD_NODEID:
		case T2_MTD_NODEID:
			APPLICATION_ADDRESS = MTD_APP2_ADDRES_FLASH;
			USER_FLASH_SIZE = (USER_FLASH_END_ADDRESS - APPLICATION_ADDRESS + 1);
			break;
			
		default :			//其他板卡都为0x8008000起始地址开始  size:0xF8000
			APPLICATION_ADDRESS = MTD_APP1_ADDRES_FLASH; 
			USER_FLASH_SIZE = (USER_FLASH_END_ADDRESS - APPLICATION_ADDRESS + 1);
			break;
	}
	if(IAP_FLAG == *(UNS32*)(IAP_FLAG_ADDR))
	{
		switch(ChooseBoard)
		{
			case MC1_MTD_NODEID:
			case M_MTD_NODEID:
			case MC2_MTD_NODEID:
				App_JumpFunction(MTD_APP1_ADDRES_FLASH);
			break;
			
			case T_MTD_NODEID:
			case T1_MTD_NODEID:
			case T2_MTD_NODEID:
				App_JumpFunction(MTD_APP2_ADDRES_FLASH);
				break;
			
			default :
				App_JumpFunction(MTD_APP1_ADDRES_FLASH);		//默认其他板卡都是从0x8010000 开始
				break;
		}
	}
	else
	{
		burn_flag = 1;
	}
			//判断该设备是否为主设备
	if((ChooseBoard & 0xf0) == 0x00 && !(ChooseBoard == 9 || ChooseBoard == 10))
		sd = &TMS_Boot_ObjDict_s_Data;
	InitNode(md,sd,ChooseBoard);
	IAP_Init(md,sd,ChooseBoard,0x161206);
	

	if(burn_flag){
		UNS8* arg = (UNS8*)(IAP_FLAG_ADDR);
		UNS32 reponse = 1;
		if(arg[0] == 0xAB)
		{
			IAP_SendReponse(arg[1],arg[2],arg[3],arg[4],arg[5],&reponse,sizeof(reponse)); //cmd有效位必须控制在32位以内
		}
	}
	
	
	while(1);
}
