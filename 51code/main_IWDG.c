#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "OLED.h"
#include "Key.h"


uint8_t KeyNum;

int main(void)
{
	OLED_Init();
	Key_Init();

	OLED_ShowString(1, 1, "IWDG TEST");
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET)
	{
		OLED_ShowString(2, 1, "IWDGRST");
		Delay_ms(500);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(100);
		
		RCC_ClearFlag();
	}
	else
	{
		OLED_ShowString(3, 1, "RST");
		Delay_ms(500);
		OLED_ShowString(3, 1, "   ");
		Delay_ms(100);
	}
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_16);
	IWDG_SetReload(2499 + 1);
	IWDG_ReloadCounter();
	IWDG_Enable();
	//喂狗或者使能会在键寄存器写入5555之外的数值，也就是给寄存器写保护了
	
	
	while(1)
	{
		Key_GetNum();
		
		IWDG_ReloadCounter();
		
		OLED_ShowString(4, 1, "FEED");
		Delay_ms(200);
		OLED_ShowString(4, 1, "    ");
		Delay_ms(600);
		
	}
}
