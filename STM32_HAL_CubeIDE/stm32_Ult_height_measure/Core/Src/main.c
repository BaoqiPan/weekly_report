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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "OLED.h"
#include "key.h"
#include "menu.h"
#include "SR_04.h"
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
uint32_t distance = 0;
uint8_t countcount = 0;
uint8_t KeyNum1;
uint8_t shutdown_flag = 0;//息屏标志位

extern uint8_t Mode_Flag;
//volatile
float single_height_data[3] = {0};
float multi_height_data[3] = {0};
uint8_t sec_count = 0;//无操作计时



volatile uint8_t Home_Flag = First_Home;
uint8_t Home_Flag_Record = First_Home;//用于回到息屏前的界面

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();
  OLED_Clear();
  HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_Base_Start_IT(&htim4);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);//关闭蜂鸣器

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	  KeyNum1 = Key_GetNum();
//	  if (KeyNum1 == 3)
//	  {
//	  	  SR_04_Start();
//	  	  HAL_Delay(200);
//	  	  OLED_ShowNum(0, 0, distance, 5, OLED_8X16);
//	  	  OLED_Update();
//	  	  HAL_Delay(1000);
//	  }
	  switch(Home_Flag)
	  {
		  uint8_t Temp;
		  case First_Home:
			  Temp = First_Page_Home();
			  if(Temp == 1){Home_Flag = Mode_Select;Home_Flag_Record = Mode_Select;}
			  else if (Temp == 2){Home_Flag = Show_Data_Mode;Home_Flag_Record = Show_Data_Mode;}
			  else if (Temp == 3){Home_Flag = shutdown;}
			  break;
		  case Mode_Select:
			  Temp = Mode_Select_Home();
			  if (Temp == 1){Home_Flag = First_Home;Home_Flag_Record = First_Home;}
			  else if (Temp == 2){Home_Flag = Single_Mode;Mode_Flag = 1;Home_Flag_Record = Single_Mode;}
			  else if (Temp == 3){Home_Flag = Multi_Mode;Mode_Flag = 2;Home_Flag_Record = Multi_Mode;}
			  break;
		  case Single_Mode:
			  Temp = Single_Mode_Home();
			  if (Temp == 1){Home_Flag = Mode_Select;Home_Flag_Record = Mode_Select;}
			  else if (Temp == 3){Home_Flag = Show_Data; Mode_Flag = 1;Home_Flag_Record = Show_Data;}
			  break;
		  case Multi_Mode:
			  Temp = Multi_Mode_Home();
			  if (Temp == 1){Home_Flag = Mode_Select;Home_Flag_Record = Mode_Select;}
			  else if (Temp == 3){Home_Flag = Show_Data; Mode_Flag = 2;Home_Flag_Record = Show_Data;}
			  break;
		  case Show_Data:
			  Temp = Single_Data_Home(Mode_Flag);
			  if (Temp == 1){Home_Flag = Temp; Home_Flag_Record = First_Home;}
			  break;
		  case Show_Data_Mode:
			  Temp = Select_Data_Home();
			  if (Temp == 1){Home_Flag = First_Home;Home_Flag_Record = First_Home;}
			  else if (Temp == 2){Home_Flag = Show_Data; Mode_Flag = 1;Home_Flag_Record = Show_Data;}
			  else if (Temp == 3){Home_Flag = Show_Data; Mode_Flag = 2;Home_Flag_Record = Show_Data;}
			  break;
		  case shutdown:
//			  OLED_Clear();
//			  OLED_ShowNum(1,1,1,1,OLED_8X16);
			  OLED_Clear();
			  OLED_Update();
			  break;
	  }
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_11)
	{
		GPIO_PinState pinState = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11);
		if(pinState == GPIO_PIN_SET)
		{
			countcount++;
			  HAL_UART_Transmit(&huart1, &countcount, 1, HAL_MAX_DELAY);
			__HAL_TIM_SET_COUNTER(&htim1, 0);
			HAL_TIM_Base_Start(&htim1);
		}
		else
		{
			HAL_TIM_Base_Stop(&htim1);
			uint32_t count = __HAL_TIM_GET_COUNTER(&htim1);
			distance = count * 0.34 / 2;
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);//关闭蜂鸣器
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	if(htim->Instance == TIM3)
	{
			Key_Tick();
	}
	if (htim->Instance == TIM4)
	{
		sec_count++;
		if (sec_count >= 30)
		{
			shutdown_flag = 1;
			sec_count = 0;
//			OLED_Clear();
		}
	}
}
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
