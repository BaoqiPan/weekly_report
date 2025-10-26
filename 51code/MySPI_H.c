#include "stm32f10x.h"                  // Device header

void MySPI_W_SS(uint8_t BitVlaue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitVlaue);
}

void MySPI_W_SCK(uint8_t BitVlaue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)BitVlaue);
}

void MySPI_W_MOSI(uint8_t BitVlaue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, (BitAction)BitVlaue);
}

uint8_t MySPI_R_MISO(void)
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
}

void MySPI_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//������ǰ�豸��SPI���������Ǵӻ�
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//SPI�ü�����
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //������8λ����16λ����֡
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//��λ���л��ǵ�λ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;//������Ԥ��Ƶ��������SCKʱ�ӵ�Ƶ��
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//����ʱ�Ӽ��ԣ�HighĬ�ϸߵ�ƽ��LowĬ�ϵ͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//ʱ����λ��1Edge�����һ�����ؿ�ʼ������2ͬ��������ϸ�������������Ϊ������Ϊģʽ0
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//���Ǽƻ�ʹ��GPIOģ�⣬�����NSS���������ò����������������
	SPI_InitStructure.SPI_CRCPolynomial = 7;//�����CRCУ��Ķ���ʽ������ٶ��У����ǲ���
	SPI_Init(SPI1, &SPI_InitStructure);
	
	SPI_Cmd(SPI1, ENABLE);
	
	MySPI_W_SS(1);
}

void MySPI_Start(void)
{
	MySPI_W_SS(0);
}

void MySPI_Stop(void)
{
	MySPI_W_SS(1);
}

uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET);
	
	SPI_I2S_SendData(SPI1, ByteSend);
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET);
	
	return SPI_I2S_ReceiveData(SPI1);
}


/*uint8_t MySPI_SwapByte(uint8_t ByteSend)//���ַ���Ч�ʱȽϸ�
{
	uint8_t i = 0x00;
	
	for (i = 0; i < 8; i++)
	{
		MySPI_W_MOSI(ByteSend & 0x80);
		ByteSend <<= 1;
		MySPI_W_SCK(1);
		if (MySPI_R_MISO() == 1){ByteSend |= 0x01;}
		MySPI_W_SCK(0);	
	}
	return ByteSend;
}*/


