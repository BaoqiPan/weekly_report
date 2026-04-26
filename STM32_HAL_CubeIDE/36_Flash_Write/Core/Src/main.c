/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
 * 函数：FLASH读取一个32位的字
 * 参数：Address 要读取数据的字节地址
 * 返回值：指定地址下的数据
 */
uint32_t MyFLASH_ReadWord(uint32_t Address)
{
	//将Address（无符号整数）转换为指针类型
	//指定该指针为volatile uint32_t* (即指向32为无符号整型的易变指针)
	//通过*读取4字节数据
	return *((__IO uint32_t *)(Address));
}

/**
 * 函数：FLASH读取一个16位的字
 * 参数：Address 要读取数据的字节地址
 * 返回值：指定地址下的数据
 */
uint32_t MyFLASH_ReadHalfWord(uint32_t Address)
{
	//将Address（无符号整数）转换为指针类型
	//指定该指针为volatile uint32_t* (即指向32为无符号整型的易变指针)
	//通过*读取2字节数据
	return *((__IO uint16_t *)(Address));
}

/**
 * 函数：FLASH读取一个8位的字
 * 参数：Address 要读取数据的字节地址
 * 返回值：指定地址下的数据
 */
uint32_t MyFLASH_ReadByte(uint32_t Address)
{
	//将Address（无符号整数）转换为指针类型
	//指定该指针为volatile uint32_t* (即指向32为无符号整型的易变指针)
	//通过*读取1字节数据
	return *((__IO uint8_t *)(Address));
}

/**
 * @brief 执行STM32F103C8T6的Flash页擦除操作（全1KB版本）
 * @param PageAddress：要擦除的页起始地址
 * @retval HAL状态值：HAL_OK（成功）或错误码
 */
HAL_StatusTypeDef FlashPageErase(uint32_t PageAddress)
{
	HAL_StatusTypeDef status = HAL_OK;
	FLASH_EraseInitTypeDef eraseInit;
	uint32_t sectorError = 0;

	//解锁flash
	HAL_FLASH_Unlock();

	//配置擦除参数
	eraseInit.NbPages = 1;
	eraseInit.PageAddress = PageAddress;
	eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;

	//执行页擦除
	status = HAL_FLASHEx_Erase(&eraseInit, &sectorError);

	//锁定Flash控制器
	HAL_FLASH_Lock();

	//返回操作状态
	if(status != HAL_OK)
	{
		return status;
	}
	return HAL_OK;

}

/**
 * @brief 向FLASH写入数据
 * @param StartAddress：写入起始地址（必须页对齐）
 * @param pData：指向要写入数据的指针
 * @param DataSize：要写入的数据大小（单位：字节）
 * @retval HAL状态值：HAL_OK（成功）或错误码
 */
HAL_StatusTypeDef FlashWrite(uint32_t StartAddress, uint8_t* pData, uint32_t DataSize)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint32_t address = StartAddress;
	uint32_t dataIndex = 0;
	uint32_t wordData = 0;

	//解锁Flash控制器
	status = HAL_FLASH_Unlock();
	if (status != HAL_OK)
	{
		return status;
	}

	//循环写入数据（按32位操作）
	while(dataIndex < DataSize)
	{
		//构建32位数据
		wordData = 0;
		for (int i = 0; i < 4&&(dataIndex + i) < DataSize; i++)
		{
			wordData |= (uint32_t)pData[dataIndex + i] << (i * 8);
		}

		//执行32位写入
		status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, wordData);
		if(status != HAL_OK)
		{
			break;
		}

		//更新地址和索引
		address += 4;
		dataIndex += 4;
	}

	//锁定Flash控制器
	HAL_FLASH_Lock();

	return status;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	OLED_Init();
	OLED_ShowHexNum(1, 1, MyFLASH_ReadWord(0x8000000), 8);
	OLED_ShowHexNum(2, 1, MyFLASH_ReadHalfWord(0x8000000), 4);
	OLED_ShowHexNum(3, 1, MyFLASH_ReadByte(0x8000000), 2);

//	FlashPageErase(0x08000800);
	uint8_t pData[] = {0x12, 0x34, 0x56, 0x78};
	FlashWrite(0x08010000, &pData, 4);
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
