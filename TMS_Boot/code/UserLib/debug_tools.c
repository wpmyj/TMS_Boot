#include "stm32f4xx.h"
#include "user_stdio.h"
#include "debug_tools.h"
void CanRxMsg_print(const CanRxMsg*msg)
{
		u8 i;
		/***********just for test can low ****************************/
		DEBUG_PRINT("can master revcive data++++++++++++++++++++++++++++++++++++\n");	
		DEBUG_PRINT("StdId : %x\r\n",msg->StdId);
		DEBUG_PRINT("ExtId : %x\r\n",msg->ExtId);
		DEBUG_PRINT("IDE : %x\r\n",msg->IDE);
		DEBUG_PRINT("DLC : %x\r\n",msg->DLC);
		for(i = 0;i < msg->DLC;i++)
		{
			DEBUG_PRINT("data[%d] : %x\r\n",i,msg->Data[i]);
		}
}

void CanTxMsg_print(const CanTxMsg*msg)
{
		u8 i;
		/***********just for test can low ****************************/
		DEBUG_PRINT("can master revcive data++++++++++++++++++++++++++++++++++++\n");	
		DEBUG_PRINT("StdId : %x\r\n",msg->StdId);
		DEBUG_PRINT("ExtId : %x\r\n",msg->ExtId);
		DEBUG_PRINT("IDE : %x\r\n",msg->IDE);
		DEBUG_PRINT("DLC : %x\r\n",msg->DLC);
		for(i = 0;i < msg->DLC;i++)
		{
			DEBUG_PRINT("data[%d] : %x\r\n",i,msg->Data[i]);
		}
}
