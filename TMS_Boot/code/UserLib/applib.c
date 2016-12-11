#include "applib.h"
#include "ObjDictMaster.h"
#include "can_address.h"
#include "user_stdio.h"
#include "MTD_CCU_CAN3_MCP2515.h"
#include "iap_cfg.h"
#include "iap.h"
#include "can_STM32.h"
void InitNode(CO_Data* md,CO_Data* sd,UNS8 ChooseBoard)
{
	initTimer(); 
	if(ChooseBoard & 0xf0)//子卡  can2用500K
		canInit(CAN2,CAN_BAUD_500K);
	else
		canInit(CAN2,CAN_BAUD_125K); //主卡  can2用125K
	setNodeId (md, can_get_address()&0x0f);
	md->canHandle = CAN2;
	setState(md,Operational);
	
	if(sd != 0){
		
		canInit(CAN3,MCP2515_CAN_BAUD_500K);
		setNodeId (sd, CAN3_NODE_ID);
		sd->canHandle = CAN3;
		setState(sd,Operational);
	}
}

