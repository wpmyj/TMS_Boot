#include "stm32f4xx.h"
#include "canfestival.h"
#include "ObjDict.h"
#include "ObjDictMaster.h"
#include "user.h"
#include "iap.h"
#include "MTD_CCU_CAN3_MCP2515.h"
#include "can_STM32.h"

/**
  * @brief  canInit
  * @param  CANx:CAN1 or CAN2 bitrate
  * @retval 0：Success
  */
unsigned char canInit(CAN_TypeDef* CANx,unsigned int bitrate)
{
		CAN_InitTypeDef        CAN_InitStructure;
		CAN_FilterInitTypeDef  CAN_FilterInitStructure;
		GPIO_InitTypeDef  GPIO_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;	         
		/* CAN GPIOs configuration **************************************************/
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;               
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		if(CANx == CAN1)
		{
				/* Enable GPIO clock */
				RCC_AHB1PeriphClockCmd(CAN1_GPIO_CLK(Z_DEVICE), ENABLE);
			
				/* Connect CAN pins to AF9 */
				GPIO_PinAFConfig(CAN1_GPIO_PORT(Z_DEVICE), CAN1_RX_SOURCE(Z_DEVICE), GPIO_AF_CAN1);
				GPIO_PinAFConfig(CAN1_GPIO_PORT(Z_DEVICE), CAN1_TX_SOURCE(Z_DEVICE), GPIO_AF_CAN1); 
				
				/* Configure CAN RX and TX pins */
				GPIO_InitStructure.GPIO_Pin = CAN1_RX_PIN(Z_DEVICE) | CAN1_TX_PIN(Z_DEVICE);
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
				GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
				GPIO_Init(CAN1_GPIO_PORT(Z_DEVICE), &GPIO_InitStructure);
			
				/* CAN configuration ********************************************************/  
				/* Enable CAN clock */
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
				
				/* CAN register init */
				CAN_DeInit(CANx);
				CAN_StructInit(&CAN_InitStructure);
			
				/* CAN cell init */
				CAN_InitStructure.CAN_TTCM = DISABLE;    
				CAN_InitStructure.CAN_ABOM = ENABLE;   
				CAN_InitStructure.CAN_AWUM = DISABLE;		
				CAN_InitStructure.CAN_NART = DISABLE;	
				CAN_InitStructure.CAN_RFLM = DISABLE;	
				CAN_InitStructure.CAN_TXFP = DISABLE;		
				CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	
				CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;  
				 
				CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
				CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;
				CAN_InitStructure.CAN_Prescaler = bitrate;   
				CAN_Init(CANx, &CAN_InitStructure);
			
				CAN_FilterInitStructure.CAN_FilterNumber = 0;	 
				CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;   
				CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
				CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
				CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
				CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
				CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
				CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;           
				CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;           
				CAN_FilterInit(&CAN_FilterInitStructure);
						
				/* Enable FIFO 0 message pending Interrupt */
				CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);	              
				
				NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
				NVIC_Init(&NVIC_InitStructure);

		}
		else if(CANx == CAN2)
		{
				/* Enable GPIO clock */
				RCC_AHB1PeriphClockCmd(CAN2_GPIO_CLK(Z_DEVICE), ENABLE);
			
				/* Connect CAN pins to AF9 */
				GPIO_PinAFConfig(CAN2_GPIO_PORT(Z_DEVICE), CAN2_RX_SOURCE(Z_DEVICE), GPIO_AF_CAN2);
				GPIO_PinAFConfig(CAN2_GPIO_PORT(Z_DEVICE), CAN2_TX_SOURCE(Z_DEVICE), GPIO_AF_CAN2); 
				
				/* Configure CAN RX and TX pins */
				GPIO_InitStructure.GPIO_Pin = CAN2_RX_PIN(Z_DEVICE) | CAN2_TX_PIN(Z_DEVICE);
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
				GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
				GPIO_Init(CAN2_GPIO_PORT(Z_DEVICE), &GPIO_InitStructure);
			
				/* CAN configuration ********************************************************/  
				/* Enable CAN clock */
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
				
				/* CAN register init */
				CAN_DeInit(CANx);
				CAN_StructInit(&CAN_InitStructure);
			
				/* CAN cell init */
				CAN_InitStructure.CAN_TTCM = DISABLE;    
				CAN_InitStructure.CAN_ABOM = ENABLE;    
				CAN_InitStructure.CAN_AWUM = ENABLE;	
				CAN_InitStructure.CAN_NART = DISABLE;	
				CAN_InitStructure.CAN_RFLM = DISABLE;	
				CAN_InitStructure.CAN_TXFP = DISABLE;		
				CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	
				CAN_InitStructure.CAN_SJW = CAN_SJW_1tq; 
					
				CAN_InitStructure.CAN_BS1 = CAN_BS1_11tq;
				CAN_InitStructure.CAN_BS2 = CAN_BS1_2tq;
				CAN_InitStructure.CAN_Prescaler = bitrate;    
				CAN_Init(CANx, &CAN_InitStructure);		
                           
				CAN_SlaveStartBank(14);                             
				CAN_FilterInitStructure.CAN_FilterNumber = 14;	 
				CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;   
				CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
				CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
				CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
				CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
				CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
				CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;           
				CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;         

				CAN_FilterInit(&CAN_FilterInitStructure);
						
				/* Enable FIFO 0 message pending Interrupt */
				CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);	             
				NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;	  
				NVIC_Init(&NVIC_InitStructure);                       
		}
		else if(CANx == CAN3)		//初始化can3
		{
				u8 res;
				res = MTDCCU_CAN3_MCP2515_Init(bitrate);
				if(res == MCP2515_OK)
					return 0;
				else
					return CAN_FAILINIT;
		}
		return 0;
}

void CAN1_RX0_IRQHandler(void)
{
	int i;
	CanRxMsg RxMessage;
	Message m;
  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	m.cob_id	= RxMessage.StdId;
	m.rtr = RxMessage.RTR;				                          
	m.len = RxMessage.DLC;                       
	for(i=0;i<RxMessage.DLC;i++)                                 
	{
			m.data[i] = RxMessage.Data[i];
	}
//	CanRxMsg_print(&RxMessage);
	if(IAP_info.mCo_data->canHandle == CAN1)
		canDispatch(IAP_info.mCo_data,&m);
	
//  if ((RxMessage.StdId == 0x321)&&(RxMessage.IDE == CAN_ID_STD) && (RxMessage.DLC == 1))
//  {
//    //LED_Display(RxMessage.Data[0]);
//    //ubKeyNumber = RxMessage.Data[0];
//  }
	/*****************end test can low***************************************/
}
void CAN2_RX0_IRQHandler(void)
{
	int i;
	CanRxMsg RxMessage;
	Message m;
  CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);
	m.cob_id	= RxMessage.StdId;
	m.rtr = RxMessage.RTR;				                          
	m.len = RxMessage.DLC;                       
	for(i=0;i<RxMessage.DLC;i++)                                 
	{
			m.data[i] = RxMessage.Data[i];
	}
	if(IAP_info.mCo_data->canHandle == CAN2)
		canDispatch(IAP_info.mCo_data,&m);
}
/**
  * @brief  canSend
	* @param  CANx:CAN1 or CAN2   m:can message
  * @retval 0：Success
  */
unsigned char canSend(CAN_PORT CANx, Message *m)	                
{
		unsigned char ret;
		unsigned char i;
		CanTxMsg TxMessage;
		uint8_t  mailbox_no,TxBufNum;
		u8 timeout=0;
		TxMessage.StdId = (uint32_t)(m->cob_id);
		TxMessage.ExtId = 0x00;
		TxMessage.RTR = m->rtr;								  
		TxMessage.IDE = CAN_ID_STD;                           
		TxMessage.DLC = m->len;                              
		for(i=0;i<m->len;i++)                                 
		{
				TxMessage.Data[i] = m->data[i];
		}
		if(CANx == CAN3)
		{
			do{
						mailbox_no = MTDCCU_CAN3_MCP2515_Get_Free_TxBuf(&TxBufNum); // info = addr.
						timeout++;
						if (timeout == 0xFF)
						{		
							printf(" 3 no mailbox!\r\n");
							break;
						}
			} while (mailbox_no == MCP2515_ALLTXBUSY);
					
			if (timeout != 0xFF)			//获得邮箱成功,可以发送数据
			{	
				//debugprintf("can3 whh22\r\n");
				MTDCCU_CAN3_MCP2515_Write_CAN_Message(TxBufNum, &TxMessage);
				return 0;
			}
			else					//表示还没有可以发送的邮箱
			{
				DEBUG_PRINT("can3 no mailbox !\r\n");
				return 1;
			}
		}
		//CanTxMsg_print(&TxMessage);
		ret = CAN_Transmit(CANx, &TxMessage);
		if(ret == CAN_TxStatus_NoMailBox)
		{
				return 1;	//1
		}
		else 
		{
				return 0;    
		}
}

//unsigned char canReceive(CAN_PORT notused,Message *m)
//{
//	unsigned int i = 0;
//			
//	CanRxMsg rx_msg;

//	CAN_Receive(CAN1, CAN_FIFO0, &rx_msg);	        //从CAN1 FIFO0接收数据
//	m->cob_id = (uint16_t)(rx_msg.StdId);
//	m->rtr = rx_msg.RTR;
//	m->len = rx_msg.DLC;
//	for(i=0;i<rx_msg.DLC;i++)
//	{
//		m->data[i] = rx_msg.Data[i];
//	}
//	return 1;
//}
