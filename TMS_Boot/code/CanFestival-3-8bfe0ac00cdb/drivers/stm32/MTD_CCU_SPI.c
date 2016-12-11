#include "MTD_CCU_SPI.h"


/*
	STM32F4XX ʱ�Ӽ���.
		HCLK = 168M
		PCLK1 = HCLK / 4 = 42M
		PCLK2 = HCLK / 2 = 84M

		SPI2��SPI3 �� PCLK1, ʱ��42M
		SPI1       �� PCLK2, ʱ��84M

		STM32F4 ֧�ֵ����SPIʱ��Ϊ 37.5 Mbits/S, �����Ҫ��Ƶ��
*/

/*
	���Ŷ��� 
	SPI_MISO   PA6			SPI1_MISO		����ӳ�
	SPI_MOSI   PA7			SPI1_MOSI		��������
	SPI_SCK		 PA5			SPI1_SCK		ʱ��
	SPI_NSS��  PA4			SPI1_NSS  	Ƭѡ
*/

//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��
void MTDCCU_SPI1_Init(void)
{	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  	SPI_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);	//ʹ��SPI1ʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	//ʹ��GPIOAʱ��

	/* SPI GPIO Configuration --------------------------------------------------*/
	
	/* ���� SCK, MISO �� MOSI Ϊ���ù��� */
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1); //PA5����Ϊ SPI1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1); //PA6����Ϊ SPI1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1); //PA7����Ϊ SPI1
 
  //GPIOA 5,6,7��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6 |GPIO_Pin_7;	//PA4~7���ù������	,PA4Ƭѡ�������������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;															//���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;														//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;													//50MHz
  GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_DOWN;														//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);																		//��ʼ��
	
	GPIO_InitStructure.GPIO_Mode =	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = MTDCCU_SPI1_CS_CPIO_PIN;
	GPIO_Init(MTDCCU_SPI1_CS_GPIO, &GPIO_InitStructure);

	 /* ����Ƭѡ����Ϊ�������ģʽ */
	MTDCCU_SPI1_CS_HIGH();		/* Ƭѡ�øߣ���ѡ�� */

	//����ֻ���SPI�ڳ�ʼ��
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);		//��λSPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);	//ֹͣ��λSPI1

	/* SPI_CPOL��SPI_CPHA���ʹ�þ���ʱ�Ӻ����ݲ��������λ��ϵ��
	   ����: ���߿����Ǹߵ�ƽ,��2�����أ������ز�������)
	*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;												//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;										//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;													//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;												//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;														//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;									//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;														//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure); 															  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����

	MTDCCU_SPI1_SendByte(0xff);//��������		 
}   

//SPI1�ٶ����ú���
//SPI�ٶ�=fAPB2/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2ʱ��һ��Ϊ84Mhz��
void MTDCCU_SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
	SPI1->CR1&=0XFFC7;								//λ3-5���㣬�������ò�����
	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI2�ٶ� 
	SPI_Cmd(SPI1,ENABLE);						 //ʹ��SPI1
} 

//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 MTDCCU_SPI1_SendByte(u8 TxData)
{	
	/* �ȴ��ϸ�����δ������� */	
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);//�ȴ���������  	
	/* ͨ��SPIӲ������1���ֽ� */
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ��byte  ����
	
	/* �ȴ�����һ���ֽ�������� */	
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); //�ȴ�������һ��byte  
	/* ���ش�SPI���߶��������� */
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����	   
}

/*���豸�ж�ȡ���ֽڵ�����*/
u8 MTDCCU_SPI1_ReadByte(void)
{
	return (MTDCCU_SPI1_SendByte(0xFF));
}

extern void MCP2515_INT1_IRQ(void);		//ִ��can3���жϺ���
void EXTI9_5_IRQHandler(void)
{
		MCP2515_INT1_IRQ();
}




