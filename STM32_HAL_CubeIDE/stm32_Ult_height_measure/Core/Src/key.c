#include <stdint.h>
#include <main.h>

uint8_t Key_Num;
extern uint8_t Home_Flag;
extern uint8_t Home_Flag_Record;//用于回到息屏前的界面
extern uint8_t sec_count;
extern uint8_t shutdown_flag;

uint8_t Key_GetNum(void)
{
	uint8_t Temp;
	if (Key_Num)
	{
		Temp = Key_Num;
		Key_Num = 0;
		return Temp;
	}
	else
		return 0;
}

uint8_t Key_GetState(void)
{
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_RESET)
	{
		return 1;
	}
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET)
	{
		return 2;
	}
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET)
	{
		return 3;
	}
	else
	{
		return 0;
	}
}

void Key_Tick(void)
{
	static uint8_t Count;
	static uint8_t CurrentState, PreState;
	Count++;
	if (Count >= 20)
	{
		Count = 0;
		PreState = CurrentState;
		CurrentState = Key_GetState();

		if (PreState != 0 && CurrentState == 0)
		{
			Home_Flag = Home_Flag_Record;
			Key_Num = PreState;
			sec_count = 0;
			shutdown_flag = 0;
		}
	}
}

