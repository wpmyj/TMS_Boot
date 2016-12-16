#include "IAP_UART.h"
#include "tools.h"
#include "IAP_Processor.h"
#include "iap.h"
static void parse_UART_Pkg(UNS8* buff,UNS16 count);
#define	 IAP_RECV_STATE_INIT	0
#define	 IAP_RECV_STATE_FLAG	(IAP_RECV_STATE_INIT+1)
#define	 IAP_RECV_STATE_LEN		(IAP_RECV_STATE_FLAG+1)
void IAP_UART_Proceed_Cell(UNS8 ch)
{
	static UNS8 rx_buff[UART_RX_BUFF_SIZE];
	static UNS16 rx_num = 0;
	static UNS16 len = 0;
	static UNS8 state = IAP_RECV_STATE_INIT;	
	switch(state)
	{
		case IAP_RECV_STATE_INIT:
			if(rx_num == 0 && ch == UART_RX_HEAD_FLAG1)
			{
				rx_buff[rx_num++] = ch;
			}
			else if(rx_num == 1  )
			{
				if(ch == UART_RX_HEAD_FLAG2)
				{
					state = IAP_RECV_STATE_FLAG;
					rx_buff[rx_num++] = ch;
				}
				else
				{
					state = IAP_RECV_STATE_INIT;
					rx_num = 0;
				}
			}
			break;
		case IAP_RECV_STATE_FLAG:
			if(rx_num == 2 )
			{
				rx_buff[rx_num++] = ch;
			}
			else if(rx_num == 3)
			{
				rx_buff[rx_num++] = ch;
				len = *(UNS16*)(rx_buff+2);
				if( len<= UART_RX_BUFF_SIZE)
				{
					state = IAP_RECV_STATE_LEN;
				}
				else
				{
					rx_num = 0;
					len = 0;
					state = IAP_RECV_STATE_INIT;
				}
			}
			break;
		case IAP_RECV_STATE_LEN:
			if(rx_num < len)
			{
				rx_buff[rx_num++] = ch;
			}
			//���治����else if ��Ҫ�ڽ������һ�ֽں������ж��Ƿ����㳤��
			if(rx_num == len)
			{
				UNS16 r_crc = *(UNS16*)(rx_buff+len - 2);
				if(r_crc == CRC16(rx_buff,len - 2))//crc����ͨ��
				{
					parse_UART_Pkg(rx_buff,len);
				}
				else
				{
					printf("");
				}
				//����crcУ���Ƿ�ͨ������ִ�иò���
				rx_num = 0;
				len = 0;
				state = IAP_RECV_STATE_INIT;
			}
			break;
	}
}
//����UART��:
//	fflldcxxxx...xxxxcc
//���ֽڱ�־+���ֽڳ���+һ�ֽ�Ŀ��ڵ� +һ�ֽڹ�����+iap���������+���ֽ�CRC
//Ŀ��ڵ� : 4b���ڵ�+3b�ӽڵ�+1b�� ,�������������̻�����Ӧ���̣�����Ӧ�ڴ�PC���ܿض˵ķ��ͷ�
//����		 : �������ȣ�������־��crcУ�飬�����ֽ�����PCһ��
static void parse_UART_Pkg(UNS8* buff,UNS16 count)
{
	UNS8 dst_mNode = buff[4] >> 4;
	UNS8 dst_sNode = buff[4] & 0x0F;
	UNS8 funcCode = buff[5] ;
	UNS8* pCmdVal = 	buff+6;
	UNS16 cmdValLen = count-8;//��ȥ8��Э���ֽڿ���
	if(CRC16(buff,count-2)!= *(UNS16*)(pCmdVal+cmdValLen))
	{
		IAP_MSG_WARN("UART Pkg CRC invalid!\r\n");
		return;
	}
	if(IS_MAIN_DEVICE())
		IAP_SendCmd(*IAP_info.mCo_data->bDeviceNodeId, 0,dst_mNode,dst_sNode, funcCode, pCmdVal,cmdValLen);
	else
	 IAP_SendCmd(MAIN_DEVICE_NODE_ID(), *IAP_info.mCo_data->bDeviceNodeId,dst_mNode,dst_sNode, funcCode, pCmdVal,cmdValLen);

	return;
}

//���͵�PC��Ӧ��
//	ffdcxxxxcc
//���ֽڱ�־+һ�ֽ�Ŀ��ڵ� +һ�ֽڹ�����+���ֽ�iap���������+���ֽ�CRC
void send_UART_Pkg(UNS8 dst_mNode,UNS8 dts_sNode,UNS8 funcCode,UNS32 cmdVal)
{
	static UNS8 tx_buff[10];
	tx_buff[0] = UART_RX_HEAD_FLAG1;
	tx_buff[1] = UART_RX_HEAD_FLAG2;
	tx_buff[2] = dst_mNode<<4 | dts_sNode;
	tx_buff[3] = funcCode;
	*(UNS32*)(tx_buff+4) = cmdVal;
	*(UNS16*)(tx_buff+8) = CRC16(tx_buff,8);
	IAP_SEND_BYTES_TO_PC(tx_buff,10);
	
	
}
