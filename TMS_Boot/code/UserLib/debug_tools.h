#ifndef __DEBUG_TOOLS_H
#define __DEBUG_TOOLS_H
#include "stm32f4xx_can.h"
extern void CanRxMsg_print(const CanRxMsg*msg);
extern void CanTxMsg_print(const CanTxMsg*msg);
#endif//__DEBUG_TOOLS_H

