#include "wdg.h"
#include "user_stdio.h"
#include "tools.h"
void bsp_InitIwdg(uint32_t _ulIWDGTime)
{
	u16 retry=0;
	DEBUG_PRINT("IWDG Init start��\r\n");
	/* ���ϵͳ�Ƿ�Ӷ������Ź���λ�лָ� */
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{		
		/* �����λ��־ */
		RCC_ClearFlag();
	}
	else
	{
		/* ��־û������ */

	}
	
	/* ʹ��LSI */
	RCC_LSICmd(ENABLE);
	
	/* �ȴ�ֱ��LSI���� */		/*���100��(1s)�ȴ�LSI��������û�о������ʼ�����Ź�ʧ��*/
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)		
	{
			retry++;
			delay_ms(10);
		
			if(retry==100)
			{
				DEBUG_PRINT("�������Ź�����ʧ��!");
				break;//LSI ����ʧ��.
			}
	}
	
	/* д��0x5555��ʾ�������IWDG_PR ��IWDG_RLR�Ĵ��� */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	/*  LSI/32 ��Ƶ*/
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	
	/*�ر�ע�⣬��������_ulIWDGTime����С��λ��ms, ����������װ������
	  ����ʱ ��Ҫ����1000
	 Counter Reload Value = (_ulIWDGTime / 1000) /(1 / IWDG counter clock period)
	                      = (_ulIWDGTime / 1000) / (32/LSI)
	                      = (_ulIWDGTime / 1000) / (32/LsiFreq)
	                      = LsiFreq * _ulIWDGTime / 32000
	 ʵ�ʲ���LsiFreq = 34000����������ȡ1��ʱ�� ��ž���1ms 
	*/
	IWDG_SetReload(_ulIWDGTime);
	
	/* ����IWDG���� */
	IWDG_ReloadCounter();
	
	/* ʹ�� IWDG (LSI oscillator ��Ӳ��ʹ��) */
	IWDG_Enable();		
}

void IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}
