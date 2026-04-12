/*
 * MySPI.c
 *
 *  Created on: Apr 9, 2026
 *      Author: 33062
 */

#include "gpio.h"
#include <stdint.h>

void MySPI_W_SS(uint8_t BitValue)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, BitValue);
}

void MySPI_W_SCK(uint8_t BitValue)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, BitValue);
}

void MySPI_W_MOSI(uint8_t BitValue)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, BitValue);
}

uint8_t MySPI_R_MISO(void)
{
	return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6);
}

void MySPI_Init(void)
{
	MX_GPIO_Init();
	MySPI_W_SS(1);
	MySPI_W_SCK(0);
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
	uint8_t i, ByteReceive = 0x00;
	for (i = 0; i < 8; i++)
	{
		MySPI_W_MOSI(!!(ByteSend & (0x80 >> i)));
		MySPI_W_SCK(1);
		if (MySPI_R_MISO())
		{
			ByteReceive |= (0x80 >> i);
		}
		MySPI_W_SCK(0);
	}
	return ByteReceive;
}


