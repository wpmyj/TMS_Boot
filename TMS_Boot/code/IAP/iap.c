#include "iap.h"
#include "iap_cfg.h"
#include "canfestival.h"
#include "IAP_Processor.h"
#include "tools.h"
static  void IAP_CMD_SDO_Callback(CO_Data* d, UNS8 nodeId);
static UNS32 writeLocalDict8Bit( CO_Data* d, UNS16 wIndex, UNS8 bSubindex, UNS8 dat);
static UNS32 writeLocalDict32Bit( CO_Data* d, UNS16 wIndex, UNS8 bSubindex, UNS32 dat);
static UNS32 writeLocalDict64Bit( CO_Data* d, UNS16 wIndex, UNS8 bSubindex, UNS64 dat);
static UNS32 proceed_IAP_CMD(CO_Data* d, const indextable *, UNS8 bSubindex);
static UNS32 proceed_IAP_CMD_reponse(CO_Data* d, const indextable *pIndexTable, UNS8 bSubindex);
 UNS8 IS_MAIN_DEVICE()
{
	return (IAP_info.ChooseBoard & 0xf0) == 0x00;
}
 UNS8 MAIN_DEVICE_NODE_ID()
{
	return IAP_info.ChooseBoard >> 4;
}

#define IAP_CMD_STATE_FREE	0
#define IAP_CMD_STATE_BUSY	(IAP_CMD_STATE_FREE+1)



#define SDO_NORMAL    (SDO_PROVIDED_BUFFER_TOO_SMALL+1)
#define SDO_UNKNOW    (SDO_NORMAL+1)
#define SDO_NO_SDO_CLIENT_WITH_NODE    (SDO_UNKNOW+1)
#define	IAP_CMD_RESPONSE_FUNC_CODE_ERROR	(SDO_NO_SDO_CLIENT_WITH_NODE+1)


IAP_Info_t IAP_info = {0,0,0,0,0,0};
/***********************************
param:
	md���������ֵ�(�������豸���豸����Ӧ��CAN2)
	sd:	�������ֵ�(��Ӧ�����豸��CAN3)
************************************/
void IAP_Init(CO_Data* md,CO_Data* sd,UNS8 ChooseBoard,UNS32 softversion)
{
	IAP_info.mCo_data = md;
	IAP_info.sCo_data = sd;
	IAP_info.ChooseBoard = ChooseBoard;
	

	
	/**************************************************
	�汾�����ֵ��ʽ(������)	
	mmmm bsss yyyy yyyy mmmm mmmm dddd dddd
	����mΪ���豸NODE ID�Ķ����Ʊ��룬sΪ���豸NODE ID�Ķ����Ʊ���, bΪBOOT��APP�����ֱ�־��yyyy mmmm ddddΪ�����յ�BCD��
	**************************************************/
	if((*md->bDeviceNodeId & 0xf0) != 0x00)//���豸
	{
		softversion |= (*md->bDeviceNodeId & 0xF0) << 24;
		softversion |= (*md->bDeviceNodeId & 0x0F) << 24;
	}
	else
	{
		softversion |= (*md->bDeviceNodeId & 0x0F) << 28;
	}
#ifdef __BOOT__
	softversion |= 0x08000000;
#endif
	IAP_info.softversion = softversion;

	RegisterSetODentryCallBack(md, IAP_MAIN_INDEX, IAP_SEG_INDEX, proceed_IAP_CMD);
	RegisterSetODentryCallBack(md, IAP_MAIN_INDEX, IAP_CMD_SUB_INDEX, proceed_IAP_CMD);
	RegisterSetODentryCallBack(md, IAP_MAIN_INDEX, IAP_CMD_REPONSE_SUB_INDEX, proceed_IAP_CMD_reponse);
	if(sd != 0)
	{
		RegisterSetODentryCallBack(sd, IAP_MAIN_INDEX, IAP_CMD_REPONSE_SUB_INDEX, proceed_IAP_CMD_reponse);
	}
	


}

//CAN����ʽ:
//sdflxxxxxxxx...xxxx
//һ�ֽ�Դ�ڵ�+һ�ֽ�Ŀ�Ľڵ�+һ�ֽڹ�����+һ�ֽڳ���+�����ֽ�����
//���������ΪUPLOAD �����ݶδ�����Զ��IAP_SEG�ֵ��ڣ��������ݳ��ȱ�������4�ֽ��ڴ�����IAP_CMD�ֵ���
void IAP_SendCmd(UNS8 src_mNode,UNS8 src_sNode,UNS8 dst_mNode,UNS8 dst_sNode,UNS8 funcCode,void* pCmdVal,UNS8 len)
{
	int i;
	UNS8 IAP_S_INDEX;
	UNS8 _pCmdVal[UART_RX_BUFF_SIZE] = {0};
	UNS32 cmdLen;
	UNS32 type;
	if(funcCode == PROGRAMING)
	{
		type = domain;
		cmdLen = 256;//IAP SEG�ֵ��С
	}
	else
	{
		type = uint64;
		cmdLen = 8;//IAP CMD�ֵ��С
	}
	_pCmdVal[0] = src_mNode<<4|src_sNode;
	_pCmdVal[1] = dst_mNode<<4|dst_sNode;
	_pCmdVal[2] = funcCode;
	_pCmdVal[3] = len;
	
	if(funcCode == PROGRAMING)
		IAP_S_INDEX = IAP_SEG_INDEX;
	else
		IAP_S_INDEX = IAP_CMD_SUB_INDEX;
	for( i = 0;i<len;i++)
		*(_pCmdVal+4+i) = *((UNS8*)pCmdVal+i);
	if(IS_MAIN_DEVICE() && dst_mNode == *IAP_info.mCo_data->bDeviceNodeId && dst_sNode == 0)//���豸Ϊ���豸����������豸
	{
			writeLocalDict(IAP_info.mCo_data, IAP_MAIN_INDEX, IAP_S_INDEX, _pCmdVal ,&cmdLen,0);
	}
	else 	if(IS_MAIN_DEVICE() && dst_mNode == *IAP_info.mCo_data->bDeviceNodeId && dst_sNode != 0)//���豸Ϊ���豸����������豸�����豸 ʹ��CAN3��
	{
		UNS32 val = 0x600 | dst_sNode;
		writeLocalDict32Bit(IAP_info.sCo_data, IAP_CMD_CSDO_PARA_INDEX, 0x01, val);//�޸Ŀͻ���TSDO node id
		val = 0x580 | dst_sNode;
		writeLocalDict32Bit(IAP_info.sCo_data, IAP_CMD_CSDO_PARA_INDEX, 0x02, val);//�޸Ŀͻ���RSDO node id
		writeLocalDict8Bit(IAP_info.sCo_data, IAP_CMD_CSDO_PARA_INDEX, 0x03, dst_sNode);//�޸Ŀͻ���SDO SERVER
		writeNetworkDictCallBack(IAP_info.sCo_data, dst_sNode, IAP_MAIN_INDEX, IAP_S_INDEX,cmdLen, (UNS32)type,_pCmdVal ,IAP_CMD_SDO_Callback,0);
	}
	else if(IS_MAIN_DEVICE() && dst_mNode != *IAP_info.mCo_data->bDeviceNodeId)//���豸Ϊ���豸����������������豸�����豸
	{
		UNS32 val = 0x600 | dst_mNode;
		writeLocalDict32Bit(IAP_info.mCo_data, IAP_CMD_CSDO_PARA_INDEX, 0x01, val);//�޸Ŀͻ���TSDO node id
		val = 0x580 | dst_mNode;
		writeLocalDict32Bit(IAP_info.mCo_data, IAP_CMD_CSDO_PARA_INDEX, 0x02, val);//�޸Ŀͻ���RSDO node id
		writeLocalDict8Bit(IAP_info.mCo_data, IAP_CMD_CSDO_PARA_INDEX, 0x03, dst_mNode);
		writeNetworkDictCallBack(IAP_info.mCo_data, dst_mNode, IAP_MAIN_INDEX, IAP_S_INDEX,cmdLen, (UNS32)type,_pCmdVal ,IAP_CMD_SDO_Callback,0);
		
	}
	else if(MAIN_DEVICE_NODE_ID() == dst_mNode &&   dst_sNode == *IAP_info.mCo_data->bDeviceNodeId)//���豸Ϊ���豸����������豸
	{
		writeLocalDict(IAP_info.mCo_data, IAP_MAIN_INDEX, IAP_S_INDEX, _pCmdVal ,&cmdLen,0);

	}
	else if(MAIN_DEVICE_NODE_ID() == dst_mNode && dst_sNode !=0 && dst_sNode != *IAP_info.mCo_data->bDeviceNodeId )//���豸Ϊ���豸,�����ͬ���������豸
	{
	}
	else if(!IS_MAIN_DEVICE() && MAIN_DEVICE_NODE_ID() != dst_mNode)//���豸Ϊ���豸,��������������豸���Լ������豸  Ŀ��CAN3 IDΪ8
	{
	}

}
//CAN����ʽ:
//sdflxxxxxxxx...xxxx
//һ�ֽ�Դ�ڵ�+һ�ֽ�Ŀ�Ľڵ�+һ�ֽڹ�����+һ�ֽڳ���+�����ֽ�����
//���ݳ��ȱ�������4�ֽ��ڴ�����IAP_CMD�ֵ���
void IAP_SendReponse(UNS8 src_mNode,UNS8 src_sNode,UNS8 dst_mNode,UNS8 dst_sNode,UNS8 funcCode,void* pCmdVal,UNS8 len) //cmd��Чλ���������32λ����
{
	int i;
	UNS8 cmdLen = 8;
	UNS8 _pCmdVal[UART_RX_BUFF_SIZE] = {0};
	_pCmdVal[0] = src_mNode<<4|src_sNode;
	_pCmdVal[1] = dst_mNode<<4|dst_sNode;
	_pCmdVal[2] = funcCode;
	_pCmdVal[3] = len;
	for( i  = 0;i<len;i++)
		*(_pCmdVal+4+i) = *((UNS8*)pCmdVal+i);
	if(IS_MAIN_DEVICE() && src_mNode == *IAP_info.mCo_data->bDeviceNodeId && src_sNode == 0)//���豸Ϊ���豸����������豸	ʹ��CAN3��
	{
		writeLocalDict64Bit(IAP_info.mCo_data, IAP_MAIN_INDEX, IAP_CMD_REPONSE_SUB_INDEX, *(UNS64*)_pCmdVal);
	}
	else if(IS_MAIN_DEVICE() && src_mNode == *IAP_info.mCo_data->bDeviceNodeId && src_sNode != 0)//���豸Ϊ���豸����������豸�����豸
	{
		UNS32 val = 0x600 | src_sNode;
		writeLocalDict32Bit(IAP_info.sCo_data, IAP_CMD_CSDO_PARA_INDEX, 0x01, val);//�޸Ŀͻ���TSDO node id
		val = 0x580 | src_sNode;
		writeLocalDict32Bit(IAP_info.sCo_data, IAP_CMD_CSDO_PARA_INDEX, 0x02, val);//�޸Ŀͻ���RSDO node id
		writeLocalDict8Bit(IAP_info.sCo_data, IAP_CMD_CSDO_PARA_INDEX, 0x03, src_sNode);//�޸Ŀͻ���RSDO node id
		writeNetworkDictCallBack(IAP_info.sCo_data, (UNS8)src_mNode, (UNS16)IAP_MAIN_INDEX, (UNS8)IAP_CMD_REPONSE_SUB_INDEX,cmdLen, (UNS32)uint64,_pCmdVal ,IAP_CMD_SDO_Callback,0);
	}
	else if(IS_MAIN_DEVICE() && src_mNode != *IAP_info.mCo_data->bDeviceNodeId)//���豸Ϊ���豸����������������豸�����豸
	{
		UNS32 val = 0x600 | src_mNode;
		writeLocalDict32Bit(IAP_info.mCo_data, IAP_CMD_CSDO_PARA_INDEX, 0x01, val);//�޸Ŀͻ���TSDO node id
		val = 0x580 | src_mNode;
		writeLocalDict32Bit(IAP_info.mCo_data, IAP_CMD_CSDO_PARA_INDEX, 0x02, val);//�޸Ŀͻ���RSDO node id
		writeLocalDict8Bit(IAP_info.mCo_data, IAP_CMD_CSDO_PARA_INDEX, 0x03, src_mNode);//�޸Ŀͻ���RSDO node id
		writeNetworkDictCallBack(IAP_info.mCo_data, (UNS8)src_mNode, (UNS16)IAP_MAIN_INDEX, (UNS8)IAP_CMD_REPONSE_SUB_INDEX,cmdLen, (UNS32)uint64,_pCmdVal ,IAP_CMD_SDO_Callback,0);
	}
	else if(MAIN_DEVICE_NODE_ID() == src_mNode &&   src_sNode == *IAP_info.mCo_data->bDeviceNodeId)//���豸Ϊ���豸����������豸
	{
		writeLocalDict64Bit(IAP_info.mCo_data, IAP_MAIN_INDEX, IAP_CMD_REPONSE_SUB_INDEX, *(UNS64*)_pCmdVal);
	}
	else if(MAIN_DEVICE_NODE_ID() == src_mNode && src_sNode != *IAP_info.mCo_data->bDeviceNodeId && src_sNode != 0)//���豸Ϊ���豸,�����ͬ���������豸
	{
	}
	else if(!IS_MAIN_DEVICE() && (MAIN_DEVICE_NODE_ID() != src_mNode || (MAIN_DEVICE_NODE_ID() == src_mNode && src_sNode ==0)  ) )//���豸Ϊ���豸,��������������豸���Լ������豸  Ŀ��CAN3 IDΪ8
	{
		UNS32 val = 0x600 | CAN3_NODE_ID;
		writeLocalDict32Bit(IAP_info.mCo_data, IAP_CMD_CSDO_PARA_INDEX, 0x01, val);//�޸Ŀͻ���TSDO node id
		val = 0x580 | CAN3_NODE_ID;
		writeLocalDict32Bit(IAP_info.mCo_data, IAP_CMD_CSDO_PARA_INDEX, 0x02, val);//�޸Ŀͻ���RSDO node id
		writeLocalDict8Bit(IAP_info.mCo_data, IAP_CMD_CSDO_PARA_INDEX, 0x03, CAN3_NODE_ID);//�޸Ŀͻ���RSDO node id
		writeNetworkDictCallBack(IAP_info.mCo_data, (UNS8)CAN3_NODE_ID, (UNS16)IAP_MAIN_INDEX, (UNS8)IAP_CMD_REPONSE_SUB_INDEX,cmdLen, (UNS32)uint64,_pCmdVal ,IAP_CMD_SDO_Callback,0);
	}
}
static ReponseStr s_Delay_stru = {0};
void IAP_DelaySend(CO_Data* d, UNS32 id)
{
	ReponseStr* s_Delay_stru = (ReponseStr*)id;
	IAP_SendCmd(s_Delay_stru->src_mNode,s_Delay_stru->src_sNode,s_Delay_stru->dst_mNode,s_Delay_stru->dst_sNode,s_Delay_stru->funcCode,s_Delay_stru->pCmdVal,s_Delay_stru->len);
}
void IAP_DelaySendReponse(CO_Data* d, UNS32 id)
{
	ReponseStr* s_Delay_stru = (ReponseStr*)id;
	IAP_SendReponse(s_Delay_stru->src_mNode,s_Delay_stru->src_sNode,s_Delay_stru->dst_mNode,s_Delay_stru->dst_sNode,s_Delay_stru->funcCode,s_Delay_stru->pCmdVal,s_Delay_stru->len);
}
static UNS32 proceed_IAP_CMD(CO_Data* d, const indextable *pIndexTable, UNS8 bSubindex)
{
	UNS8* pCmdVal = ((UNS8*)(pIndexTable->pSubindex[bSubindex].pObject));
	s_Delay_stru.src_mNode = pCmdVal[0] >> 4;
	s_Delay_stru.src_sNode = pCmdVal[0] & 0x0f;
	s_Delay_stru.dst_mNode = pCmdVal[1] >> 4;
	s_Delay_stru.dst_sNode = pCmdVal[1] & 0x0f;
	s_Delay_stru.funcCode =  pCmdVal[2];
	s_Delay_stru.len =  		 pCmdVal[3];
	s_Delay_stru.pCmdVal = 	 pCmdVal + 4;
	//DEBUG_PRINT("--->  cmd value : %lx \r\n",*(UNS32*)s_Delay_stru.pCmdVal);
	if((IS_MAIN_DEVICE() && (s_Delay_stru.dst_mNode == *IAP_info.mCo_data->bDeviceNodeId) && (s_Delay_stru.dst_sNode == 0)) 
		|| ((MAIN_DEVICE_NODE_ID() == s_Delay_stru.dst_mNode )&& (s_Delay_stru.dst_sNode == *IAP_info.mCo_data->bDeviceNodeId)))//���豸ΪĿ���豸
	{
		*(UNS32 *)s_Delay_stru.pCmdVal = iap_processor[s_Delay_stru.funcCode].execute(s_Delay_stru.src_mNode,s_Delay_stru.src_sNode,s_Delay_stru.dst_mNode,s_Delay_stru.dst_sNode,s_Delay_stru.funcCode,s_Delay_stru.pCmdVal,s_Delay_stru.len);
		s_Delay_stru.len = 0x04;
		SetAlarm(IAP_info.mCo_data,(UNS32) &s_Delay_stru,IAP_DelaySendReponse,MS_TO_TIMEVAL(1),0);
	}
	else{
		SetAlarm(IAP_info.mCo_data,(UNS32) &s_Delay_stru,IAP_DelaySend,MS_TO_TIMEVAL(1),0);
	}
	
	return OD_SUCCESSFUL;
}
static UNS32 proceed_IAP_CMD_reponse(CO_Data* d, const indextable *pIndexTable, UNS8 bSubindex)
{
	UNS8* pCmdVal = ((UNS8*)(pIndexTable->pSubindex[bSubindex].pObject));
	s_Delay_stru.src_mNode = pCmdVal[0] >> 4;
	s_Delay_stru.src_sNode = pCmdVal[0] & 0x0f;
	s_Delay_stru.dst_mNode = pCmdVal[1] >> 4;
	s_Delay_stru.dst_sNode = pCmdVal[1] & 0x0f;
	s_Delay_stru.funcCode =  pCmdVal[2];
	s_Delay_stru.len =  		 pCmdVal[3];
	s_Delay_stru.pCmdVal = 	 pCmdVal + 4;

	//DEBUG_PRINT("<---  reponse value : %lx \r\n",*(UNS32*)s_Delay_stru.pCmdVal);
	if((IS_MAIN_DEVICE() && s_Delay_stru.src_mNode == *IAP_info.mCo_data->bDeviceNodeId && s_Delay_stru.src_sNode == 0) 
		|| (MAIN_DEVICE_NODE_ID() == s_Delay_stru.src_mNode && s_Delay_stru.src_sNode == *IAP_info.mCo_data->bDeviceNodeId))//���豸ΪĿ���豸
	{
		iap_processor[s_Delay_stru.funcCode].reponse(s_Delay_stru.src_mNode,s_Delay_stru.src_sNode,s_Delay_stru.dst_mNode,s_Delay_stru.dst_sNode,s_Delay_stru.funcCode,s_Delay_stru.pCmdVal,s_Delay_stru.len);
	}
	else
	{
		SetAlarm(IAP_info.mCo_data,(UNS32) &s_Delay_stru,IAP_DelaySendReponse,MS_TO_TIMEVAL(1),0);
	}
	return OD_SUCCESSFUL;
}


static UNS32 writeLocalDict8Bit( CO_Data* d, UNS16 wIndex, UNS8 bSubindex, UNS8 dat)
{
	UNS8 val = dat;
	UNS32 size = sizeof(val);
	return writeLocalDict(d, wIndex, bSubindex, &val,&size, 0);
}
static UNS32 writeLocalDict32Bit( CO_Data* d, UNS16 wIndex, UNS8 bSubindex, UNS32 dat)
{
	UNS32 val = dat;
	UNS32 size = sizeof(val);
	return writeLocalDict(d, wIndex, bSubindex, &val,&size, 0);
}
static UNS32 writeLocalDict64Bit( CO_Data* d, UNS16 wIndex, UNS8 bSubindex, UNS64 dat)
{
	UNS64 val = dat;
	UNS32 size = sizeof(val);
	return writeLocalDict(d, wIndex, bSubindex, &val,&size, 0);
}
static  void IAP_CMD_SDO_Callback(CO_Data* d, UNS8 nodeId)
{
	UNS32 abortCode;
	UNS32 ret = getWriteResultNetworkDict(d, nodeId,  &abortCode);
	if( ret != SDO_FINISHED) //ͨѶ�쳣
	{
		DEBUG_PRINT("Result : Failed in getting information for slave %2.2x, AbortCode :%4.4x \r\n", nodeId, (unsigned int)abortCode);
	}else
	{
//		DEBUG_PRINT("Result : sdo write success\r\n");
	}
	/* Finalize last SDO transfer with this node */
	closeSDOtransfer(d, nodeId, SDO_CLIENT);
	
}
