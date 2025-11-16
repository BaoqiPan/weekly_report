#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Serial.h"
#include "Status.h"
#include "Key.h"
#include "AD.h"
#include "IRSensor.h"
#include "PWM.h"

uint8_t KeyNum;
uint8_t RxData;
uint16_t Main_Status = 0;
uint8_t Motor_Status = 0;
uint16_t Memo_Status;
uint16_t Last_Status;
uint8_t dir = 0;
uint8_t GPIO_Pin_4_status;
uint16_t ADValue;
//uint8_t GPIO_Pin_5_status;




int main(void)
{
	OLED_Init();
	Motor_Init();
	Serial_Init();
	Key_Init();
	AD_Init();
//	IRSensor_Init();
	
	Motor_Speed = 20;
	
	while(1)
	{
		switch(Main_Status)
		{
			case Init:
				OLED_ShowString(2, 5, "Welcome");
				Motor_Off();
				Main_Status = Motor_OFF;
				PWM_SetCompare3_Positive();
				Motor_SetSpeed(Motor_Speed);
				Motor_Status = 0;
				break;
			
			case Motor_Start_Status:
				OLED_ShowString(1, 1, "Speed:");
				OLED_ShowString(2, 1, "DIR:");
				OLED_ShowNum(1, 7, Motor_Speed, 3);
				OLED_ShowNum(2, 5, dir, 1);
				Motor_On();

				Motor_Status = 1;
				break;
			
			case Main_Change:
				OLED_Clear();
				Main_Status = Memo_Status;
				break;
			
			case Usart_Judge:
				RxData = Serial_GetRxData();
				if(RxData == 0x01)
				{
					printf("START OK\r\n");   //找到问题了，Meomo在发送完消息后一直为judge
					Main_Status = Motor_Start_Status;
					Motor_Status = 1;
				}
				if(RxData == 0x02)
				{
					printf("STOP OK\r\n");
					Main_Status = Motor_OFF;
					Motor_Status = 0;
				}
				if(RxData == 0x03)
				{
					printf("Speed Add OK\r\n");
					Main_Status = Motor_Speed_Add;
					if (Motor_Status == 0)
						Memo_Status = Motor_OFF;
					else
						Memo_Status = Motor_Start_Status;
				}
				if(RxData == 0x04)
				{
					printf("Speed Subtract OK\r\n");
					Main_Status = Motor_Speed_Sub;
					if (Motor_Status == 0)
						Memo_Status = Motor_OFF;
					else
						Memo_Status = Motor_Start_Status;
				}
				if(RxData == 0x05)
				{
					printf("Motor Reverse OK\r\n");
					Main_Status = Motor_Reverse;
					if (Motor_Status == 0)
						Memo_Status = Motor_OFF;
					else
						Memo_Status = Motor_Start_Status;
				}
				break;
				
			case Motor_OFF:
				Motor_Off();
				OLED_ShowString(3, 5, "Motor OFF");
/*				ADValue = AD_GetValue();
				if(ADValue < 2000)
				{
					Main_Status = Main_Change;
					Memo_Status = Motor_Start_Status;
				}*/
				Motor_Status = 0;
				break;
			
			case Motor_Speed_Add:
				if (Motor_Speed < 100)
				{
					Motor_Speed += 20;
				}
				else
				{
					printf("MAX Speed! Can't Add!\r\n");
				}
				Motor_SetSpeed(Motor_Speed);				
				Main_Status = Memo_Status;		//使用usart先加速，再开启会导致先正传一会，然后持续反	转
				break;
				
			case Motor_Speed_Sub:
				if (Motor_Speed > 0)
				{
					Motor_Speed -= 20;
				}
				else
				{
					printf("MIN Speed! Can't Subtract!\r\n");
				}
				Motor_SetSpeed(Motor_Speed);
				Main_Status = Memo_Status;
				break;
				
			case Motor_Reverse:
				GPIO_Pin_4_status = GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_4);
				Motor_Off();		
				Delay_ms(1000);
			
				if(GPIO_Pin_4_status == SET)
				{
					GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);
					GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
					dir = 0;
				}
				else
				{
					GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
					GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
					dir = 1;
				}
				Main_Status = Main_Change;
				break;
		}

	}
}
