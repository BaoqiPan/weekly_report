#include "stm32f10x.h"                  // Device header
#include "Delay.h"

//#define SCL_PORT GPIOB
//#define SCL_PIN GPIO_Pin_10

void MyI2C_W_SCL(uint8_t BitValue)//网课里是uint8_t，实际上不好，可以改成BitAction
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);
	Delay_us(10);//加延时防止引脚翻转过快，导致MPU6050跟不上
}

void MyI2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);
	Delay_us(10);
}

uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
	Delay_us(10);
	return BitValue;
}


void MyI2C_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_11 | GPIO_Pin_10);
}

void MyI2C_Start(void)
{	
	MyI2C_W_SDA(1);//注意最好要把释放SDA放在前面，因为要兼容重复起始条件Sr
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}

void MyI2C_Stop(void)
{
	MyI2C_W_SDA(0);//默认确保SCL是低电平，注意验证这个
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}
//除了终止条件，SCL以高电平结束，所有单元我们都会保证SCL以低电平结束，方便各个单元拼接
void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i = 0;
	for(i = 0; i < 8; i++)
	{
		MyI2C_W_SDA(Byte & (0x80 >> i));//1000 0000 与清零 或置一
		//这里传入的参数有非0即1的特性（看函数的参数类型），所以不用单独把最高位1提出来
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);		
	}
}

uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t Byte = 0x00;
	uint8_t i = 0;
	MyI2C_W_SDA(1);
	for(i = 0; i < 8; i++)
	{
		MyI2C_W_SCL(1);
		if (MyI2C_R_SDA() == 1){Byte |= (0x80 >> i);}
		MyI2C_W_SCL(0);		
	}
	return Byte;
}

void MyI2C_SendAck(uint8_t AckBit)
{
	MyI2C_W_SDA(AckBit);
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);		
}

uint8_t MyI2C_ReceiveAck(void)
{
	uint8_t AckBit;
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	AckBit = MyI2C_R_SDA();
	MyI2C_W_SCL(0);	
	return AckBit;
}

