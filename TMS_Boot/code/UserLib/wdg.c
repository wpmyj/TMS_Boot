#include "wdg.h"
#include "user_stdio.h"
#include "tools.h"
void bsp_InitIwdg(uint32_t _ulIWDGTime)
{
	u16 retry=0;
	DEBUG_PRINT("IWDG Init start！\r\n");
	/* 检测系统是否从独立看门狗复位中恢复 */
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{		
		/* 清除复位标志 */
		RCC_ClearFlag();
	}
	else
	{
		/* 标志没有设置 */

	}
	
	/* 使能LSI */
	RCC_LSICmd(ENABLE);
	
	/* 等待直到LSI就绪 */		/*最多100次(1s)等待LSI就绪，若没有就绪则初始化看门狗失败*/
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)		
	{
			retry++;
			delay_ms(10);
		
			if(retry==100)
			{
				DEBUG_PRINT("独立看门狗开启失败!");
				break;//LSI 开启失败.
			}
	}
	
	/* 写入0x5555表示允许访问IWDG_PR 和IWDG_RLR寄存器 */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	/*  LSI/32 分频*/
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	
	/*特别注意，由于这里_ulIWDGTime的最小单位是ms, 所以这里重装计数的
	  计数时 需要除以1000
	 Counter Reload Value = (_ulIWDGTime / 1000) /(1 / IWDG counter clock period)
	                      = (_ulIWDGTime / 1000) / (32/LSI)
	                      = (_ulIWDGTime / 1000) / (32/LsiFreq)
	                      = LsiFreq * _ulIWDGTime / 32000
	 实际测试LsiFreq = 34000，所以这里取1的时候 大概就是1ms 
	*/
	IWDG_SetReload(_ulIWDGTime);
	
	/* 重载IWDG计数 */
	IWDG_ReloadCounter();
	
	/* 使能 IWDG (LSI oscillator 由硬件使能) */
	IWDG_Enable();		
}

void IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}
