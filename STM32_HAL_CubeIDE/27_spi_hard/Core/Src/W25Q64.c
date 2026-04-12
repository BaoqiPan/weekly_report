#include "W25Q64.h"
#include "spi.h"
#include "gpio.h"
#include "W25Q64_Ins.h"

#define W25Q64_CS_LOW()		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define W25Q64_CS_HIGH()	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)

void W25Q64_Init(void)
{
	W25Q64_CS_HIGH();
}

void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
	uint8_t cmd = W25Q64_JEDEC_ID;
	uint8_t response[3];

	W25Q64_CS_LOW();//选中芯片
	HAL_SPI_Transmit(&hspi1, &cmd, 1, 1000);
	HAL_SPI_Receive(&hspi1, response, 3, 1000);
	W25Q64_CS_HIGH();//取消选中
	*MID = response[0];
	*DID = (response[1] << 8) | response[2];
}

void W25Q64_WriteEnable(void)
{
	W25Q64_CS_LOW();
	HAL_SPI_Transmit(&hspi1, W25Q64_WRITE_ENABLE, 1, 1000);
	W25Q64_CS_HIGH();
}

void W25Q64_WaitBusy(void)
{
	uint32_t Timeout = 100000;
	uint8_t status;

	W25Q64_CS_LOW();
	HAL_SPI_Transmit(&hspi1, W25Q64_READ_STATUS_REGISTER_1, 1, 1000);
	HAL_SPI_Receive(&hspi1, &status, 1, 1000);

	while ((status & 0x01 ) == 0x01)//循环等待忙标志位
	{
		Timeout--;
		if (Timeout == 0)
		{
			break;
		}
	}
	W25Q64_CS_HIGH();
}

/**
 * 函  数：W25Q64页编程
 * 参  数：Address 页编程的起始地址，范围：0x000000~0x7FFFFF
 * 参  数：DataArray	用于写入数据的数组
 * 参  数：Count 要写入数据的数量，范围：0~256
 * 返 回 值：无
 * 注意事项：写入的地址范围不能跨页
 */
//页编程：在一页里面进行写数据
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
	uint8_t cmd[4] = {W25Q64_PAGE_PROGRAM, (uint8_t)((Address >> 16) & 0xFF),
			(uint8_t)((Address >> 8) & 0xFF), (uint8_t)(Address & 0xFF)};
	W25Q64_WriteEnable();
	W25Q64_CS_LOW();
	HAL_SPI_Transmit(&hspi1, cmd, 4, 1000);
	HAL_SPI_Transmit(&hspi1, DataArray, Count, 1000);
	W25Q64_CS_HIGH();
	W25Q64_WaitBusy();
}

//擦除地址所在整个4KB的扇区
void W25Q64_SectorErase(uint32_t Address)
{
	uint8_t cmd[4] = {W25Q64_SECTOR_ERASE_4KB, (uint8_t)((Address >> 16) & 0xFF),
			(uint8_t)((Address >> 8) & 0xFF), (uint8_t)((Address) & 0xFF)};
	W25Q64_WriteEnable();
	W25Q64_CS_LOW();
	HAL_SPI_Transmit(&hspi1, cmd, 4, 1000);
	W25Q64_CS_HIGH();
	W25Q64_WaitBusy();
}

void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
	uint8_t cmd[4] = {W25Q64_READ_DATA, (uint8_t)((Address >> 16) & 0xFF),
			(uint8_t)((Address >> 8) & 0xFF), (uint8_t)(Address & 0xFF)};
	W25Q64_CS_LOW();
	HAL_SPI_Transmit(&hspi1, cmd, 4, 1000);
	HAL_SPI_Receive(&hspi1, DataArray, Count, 1000);
	W25Q64_CS_HIGH();
}


