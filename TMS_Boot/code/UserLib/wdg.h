#ifndef __WDG_H__
#define __WDG_H__
#include "stm32f4xx.h"
extern void IWDG_Feed(void);
extern void bsp_InitIwdg(uint32_t _ulIWDGTime);
#endif//__WDG_H__