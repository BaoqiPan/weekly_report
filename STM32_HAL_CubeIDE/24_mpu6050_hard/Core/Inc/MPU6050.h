/*
 * MPU6050.h
 *
 *  Created on: Mar 30, 2026
 *      Author: 33062
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

void MPU6050_Init(I2C_HandleTypeDef *hi2c);
//读取MPU6050的16位数据
int16_t MPU6050_ReadData(I2C_HandleTypeDef *hi2c, uint8_t reg);
//读取MPU6050设备ID号
uint8_t MPU6050_ReadDeviceID(I2C_HandleTypeDef *hi2c);
//读取加速度计数据
void MPU6050_ReadAccel(I2C_HandleTypeDef *hi2c, int16_t *ax, int16_t *ay, int16_t *az);
//读取陀螺仪数据
void MPU6050_ReadGyro(I2C_HandleTypeDef *hi2c, int16_t *gx, int16_t *gy, int16_t *gz);

#endif /* INC_MPU6050_H_ */
