/*
 * MPU6050.c
 *
 *  Created on: Mar 30, 2026
 *      Author: 33062
 */
#include "i2c.h"

#define MPU6050_ADDR 	0xD0
#define PWR_MGMT_1		0x6B
#define ACCEL_XOUT_H	0x3B
#define GYRO_XOUT_H		0x43
#define MPU6050_WHO_AM_I_REG	0x75


//初始化MPU6050
//MPU6050 在上电之后默认处于睡眠模式，为了让传感器正常工作，就需要唤醒它
//通过向 PWR_MGMT_1 寄存器写入0x00，可以将MPU6050 从休眠模式中唤醒，从而使其开始正常采集数据。
//这个函数的主要功能就是完成 MPU6050 的唤醒条件，为后续的数据读取做好准备
void MPU6050_Init(I2C_HandleTypeDef *hi2c)
{
	//唤醒MPU6050
	HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, PWR_MGMT_1, 1, 0x00, 1, HAL_MAX_DELAY);
	//采样分频器
	HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, 0x19, 1, 0x07, 1, HAL_MAX_DELAY);
	//低通滤波器
	HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, 0x1A, 1, 0x06, 1, HAL_MAX_DELAY);
	//陀螺仪角速度，±500°/s
	HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, 0x1B, 1, 0x08, 1, HAL_MAX_DELAY);
	//加速度计 ±2g
	HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, 0x1C, 1, 0x00, 1, HAL_MAX_DELAY);
}

//读取MPU6050的16位数据
int16_t MPU6050_ReadData(I2C_HandleTypeDef *hi2c, uint8_t reg)
{
	uint8_t data[2];
	HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, reg, 1, data, 2, HAL_MAX_DELAY);
	return (int16_t)((data[0] << 8) | data[1]);
}

//读取MPU6050设备ID号
uint8_t MPU6050_ReadDeviceID(I2C_HandleTypeDef *hi2c)
{
	uint8_t device_id;
	HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_WHO_AM_I_REG, 1, &device_id, 1, HAL_MAX_DELAY);
	return device_id;
}

//读取加速度计数据
void MPU6050_ReadAccel(I2C_HandleTypeDef *hi2c, int16_t *ax, int16_t *ay, int16_t *az)
{
	*ax = MPU6050_ReadData(hi2c, ACCEL_XOUT_H);
	*ay = MPU6050_ReadData(hi2c, ACCEL_XOUT_H + 2);
	*az = MPU6050_ReadData(hi2c, ACCEL_XOUT_H + 4);
}

//读取陀螺仪数据
void MPU6050_ReadGyro(I2C_HandleTypeDef *hi2c, int16_t *gx, int16_t *gy, int16_t *gz)
{
	*gx = MPU6050_ReadData(hi2c, GYRO_XOUT_H);
	*gy = MPU6050_ReadData(hi2c, GYRO_XOUT_H + 2);
	*gz = MPU6050_ReadData(hi2c, GYRO_XOUT_H + 4);
}
