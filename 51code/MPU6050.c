#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS 0xD0


void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);
	MyI2C_ReceiveAck();
	MyI2C_Stop();
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	MyI2C_Start();//注意这个重复起始位不要忘记
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);//读写位置1
	MyI2C_ReceiveAck();
	Data = MyI2C_ReceiveByte();
	MyI2C_SendAck(1);//因为此时SDA线归从机所控制，所以是发送应答/非应答
	MyI2C_Stop();
	
	return Data;
}

void MPU6050_Init(void)
{
	MyI2C_Init();
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);//配置电源管理寄存器1
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);//配置电源管理寄存器2
	
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);//配置采样率分频，这八位决定数据输出的快慢,0x09是10分频
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);//配置寄存器
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);//陀螺仪配置寄存器，选择最大量程
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);//加速度计配置寄存器
}//接触睡眠，选择陀螺仪时钟；6个轴均不待机；采样分频为10；滤波参数给最大；陀螺仪和加速度计都选择最大量程


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
	*AccX = (DataH << 8) | DataL;//不用担心这个没问题，不会导致某些位丢失
	
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
//这个函数还有一个更高效的方法，就是使用IIC读取多个字节的时序，从一个基地址开始，连续读取一片的寄存器
