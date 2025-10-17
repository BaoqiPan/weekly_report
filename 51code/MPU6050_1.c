#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS 0xD0

void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout;//��ֹ��ʱ
	Timeout = 10000;
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)//EV5
	{
		Timeout--;
		if (Timeout == 0)
		{
			break;
		}
	}	
}

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
//	MyI2C_Start();
//	MyI2C_SendByte(MPU6050_ADDRESS);
//	MyI2C_ReceiveAck();
//	MyI2C_SendByte(RegAddress);
//	MyI2C_ReceiveAck();
//	MyI2C_SendByte(Data);
//	MyI2C_ReceiveAck();
//	MyI2C_Stop();
	
	I2C_GenerateSTART(I2C2, ENABLE);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);//EV5
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);//EV6
	
	I2C_SendData(I2C2, RegAddress);												//EV8_1
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);//EV8

	I2C_SendData(I2C2, Data);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);//EV8_2
	
	I2C_GenerateSTOP(I2C2, ENABLE);
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress)//������ֱȽ��ѣ��࿴����
{
	uint8_t Data;
//	
//	MyI2C_Start();
//	MyI2C_SendByte(MPU6050_ADDRESS);
//	MyI2C_ReceiveAck();
//	MyI2C_SendByte(RegAddress);
//	MyI2C_ReceiveAck();
//	
//	MyI2C_Start();//ע������ظ���ʼλ��Ҫ����
//	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);//��дλ��1
//	MyI2C_ReceiveAck();
//	Data = MyI2C_ReceiveByte();
//	MyI2C_SendAck(1);//��Ϊ��ʱSDA�߹�ӻ������ƣ������Ƿ���Ӧ��/��Ӧ��
//	MyI2C_Stop();
//	
	
	I2C_GenerateSTART(I2C2, ENABLE);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);//EV5
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);//EV6
	
	I2C_SendData(I2C2, RegAddress);												//EV8_1
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);//EV8
	//������transmitted����transmitting�����ԣ�ǰ����ã���Ϊ�������Ҫ��
	//�����Ƿ��ϸ�ʽָ����ַ��һ����
	I2C_GenerateSTART(I2C2, ENABLE);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);//EV5
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);//EV6
	
	I2C_AcknowledgeConfig(I2C2, DISABLE);
	I2C_GenerateSTOP(I2C2, ENABLE);
	
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);//EV7
	Data = I2C_ReceiveData(I2C2);
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	//Ĭ��״̬��ACK����1������Ĭ��״̬
	return Data;
}

void MPU6050_Init(void)
{
//	MyI2C_Init();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_ClockSpeed = 50000;//0~100k�Ǳ�׼ģʽ��100k~400k�ǿ���ģʽ
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;//��׼ģʽ����������ʲô������1:1
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;//���������STM32��Ϊ�ӻ��Ż��õ��Ĳ���
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;//���Ҳ��STM32��Ϊ�ӻ�ʱ�Ż�ʹ�ã�����ָ��STM32�������ַ
	I2C_Init(I2C2,&I2C_InitStructure);
	
	I2C_Cmd(I2C2, ENABLE);
	
	
	
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);//���õ�Դ����Ĵ���1
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);//���õ�Դ����Ĵ���2
	
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);//���ò����ʷ�Ƶ�����λ������������Ŀ���,0x09��10��Ƶ
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);//���üĴ���
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);//���������üĴ�����ѡ���������
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);//���ٶȼ����üĴ���
}//�Ӵ�˯�ߣ�ѡ��������ʱ�ӣ�6�������������������ƵΪ10���˲���������������Ǻͼ��ٶȼƶ�ѡ���������


uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}


void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	*AccX = (DataH << 8) | DataL;//���õ������û���⣬���ᵼ��ĳЩλ��ʧ
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	*AccY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (DataH << 8) | DataL;
	
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	*GyroX = (DataH << 8) | DataL;

	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	*GyroY = (DataH << 8) | DataL;

	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	*GyroZ = (DataH << 8) | DataL;
}
//�����������һ������Ч�ķ���������ʹ��IIC��ȡ����ֽڵ�ʱ�򣬴�һ������ַ��ʼ��������ȡһƬ�ļĴ���
