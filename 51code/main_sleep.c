#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "OLED.h"
#include "Serial.h"

uint8_t RxData;

int main(void)
{
	OLED_Init();
	OLED_ShowString(1, 1, "RxData:");
	
	Serial_Init();

	
	while(1)
	{
		//这种是通过查询来实现串口数据接收的，如果代码比较简单，可以用这个
		//if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
		//{
		//	RxData = USART_ReceiveData(USART1);
		//	OLED_ShowHexNum(1, 1, RxData, 2);
		//}
		
		
		//这种是通过中断来实现串口数据接收的
		if(Serial_GetRxFlag() == 1)
		{
			RxData = Serial_GetRxData();
			OLED_ShowHexNum(1, 8, RxData, 2);
			Serial_SendByte(RxData);
		}
		OLED_ShowString(2, 1, "Running");
		Delay_ms(100);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(100);
		
		__WFI();
		
	}
}
