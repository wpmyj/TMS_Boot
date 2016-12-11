#ifndef __IAP_H
#define __IAP_H
#include "canfestival.h"
#include "IAP_UART.h"
#include "stm32f4xx.h"
#include "iap_cfg.h"

#define IAP_FLAG_ADDR			0x0800C000
#define IAP_FLAG					0xAFFA2016

#define FILE_BLK_MAX_SIZE	244//����4�ֽڶ���  256����10�ֽ�Э���ֽڻ���2�ֽڿ���

extern void IAP_DelaySend(CO_Data* d, UNS32 id);
extern void IAP_DelaySendReponse(CO_Data* d, UNS32 id);


typedef struct _ReponseStr{
	UNS8 src_mNode;
	UNS8 src_sNode;
	UNS8 dst_mNode;
	UNS8 dst_sNode;
	UNS8 funcCode;
	UNS8 len;
	void* pCmdVal;
} ReponseStr;
typedef struct struct_IAP_Info_t
{
	CO_Data* mCo_data;
	CO_Data* sCo_data;
	UNS32 	softversion;
	short timerHandle;
	UNS8  ChooseBoard;
	UNS8 	lastFuncCode;
}IAP_Info_t ;
extern IAP_Info_t IAP_info;
extern  __inline UNS8 IS_MAIN_DEVICE(void);
extern  __inline UNS8 MAIN_DEVICE_NODE_ID(void);
void IAP_SendCmd(UNS8 src_mNode,UNS8 src_sNode,UNS8 dst_mNode,UNS8 dst_sNode,UNS8 funcCode,void* pCmdVal,UNS8 len); 
void IAP_SendReponse(UNS8 src_mNode,UNS8 src_sNode,UNS8 dst_mNode,UNS8 dst_sNode,UNS8 funcCode,void* pCmdVal,UNS8 len); //cmd��Чλ���������32λ����

#ifndef USE_CUSTOM_IAP_SEND_BYTES_TO_PC
#include <stdio.h>
static  __inline void IAP_SEND_BYTES_TO_PC(UNS8* buff,UNS16 count)
{
	__disable_irq();//��ֹ���ʹ���IAP֡����ӡ��������
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);//��ֹ���ϴ�ӡ�����Ȱ����Ĵ�ӡ�ַ������
	while(count--)  fputc(*buff++,(FILE*)1);
	__enable_irq();
}
#endif//USE_CUSTOM_IAP_SEND_BYTES_TO_PC

//for user
extern void IAP_Init(CO_Data* md,CO_Data* sd,UNS8 ChooseBoard,UNS32 softversion);
#endif//__IAP_H
