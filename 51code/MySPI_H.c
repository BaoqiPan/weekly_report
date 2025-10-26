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
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//决定当前设备是SPI的主机还是从机
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//SPI裁剪引脚
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //配置是8位还是16位数据帧
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//高位先行还是低位先行
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;//波特率预分频器，配置SCK时钟的频率
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//配置时钟极性，High默认高电平，Low默认低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//时钟相位，1Edge代表第一个边沿开始采样，2同理，这个和上个参数的配置是为了配置为模式0
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//我们计划使用GPIO模拟，外设的NSS引脚我们用不到，所以软件即可
	SPI_InitStructure.SPI_CRCPolynomial = 7;//这个是CRC校验的多项式，填多少都行，我们不用
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


/*uint8_t MySPI_SwapByte(uint8_t ByteSend)//这种方法效率比较高
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


