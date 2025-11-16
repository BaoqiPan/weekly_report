#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "status.h"

extern uint16_t Main_Status;
extern uint8_t Motor_Status;
extern uint16_t Memo_Status;
extern uint16_t Last_Status;

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line =EXTI_Line1;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
}

uint8_t Key_GetNum(void)
{
	uint8_t KeyNum=0;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0);
		Delay_ms(20);
		KeyNum=1;
	}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0);
		Delay_ms(20);
		KeyNum=2;
	}	
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==0);
		Delay_ms(20);
		KeyNum=3;
	}	
	return KeyNum;
}

void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		Delay_ms(30);
		if(EXTI_GetITStatus(EXTI_Line1) != RESET)
		{
			Main_Status = Motor_Speed_Add;	
			while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0);
			Delay_ms(20);
		}		
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
		Delay_ms(30);
		if(EXTI_GetITStatus(EXTI_Line6) != RESET)
		{
			if(Motor_Status == 0)
			{
				Main_Status = Main_Change;
				Memo_Status = Motor_Start_Status;
				Motor_Status = 1;
			}
			else
			{
				Main_Status = Main_Change;	
				Memo_Status = Motor_OFF;
				Motor_Status = 0;
			}
			while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==0);
			Delay_ms(20);

		}
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	if(EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
		Delay_ms(30);
		if(EXTI_GetITStatus(EXTI_Line7) != RESET)
		{
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==0)
			{
				Main_Status = Main_Change;
				Memo_Status = Motor_Start_Status;
				Delay_ms(100);
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	

}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line11) != RESET)
	{
		Delay_ms(30);
		if(EXTI_GetITStatus(EXTI_Line11) != RESET)
		{
			Main_Status = Motor_Speed_Sub;	
			while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0);
			Delay_ms(20);
		}			
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		Delay_ms(30);
		if(EXTI_GetITStatus(EXTI_Line0) != RESET)
		{
			Main_Status = Motor_Reverse;	
			while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0);
			Delay_ms(20);
					
/*			if(Motor_Status == 1)
			{
				Memo_Status = Motor_Start_Status;
				Motor_Status = 1;
			}
			else
			{	
				Memo_Status = Motor_OFF;
				Motor_Status = 0;
			}*/
			while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0);
			Delay_ms(20);

		}			
		EXTI_ClearITPendingBit(EXTI_Line0);
	}	
}
