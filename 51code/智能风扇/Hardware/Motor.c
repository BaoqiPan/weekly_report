#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "Status.h"

uint16_t Motor_Speed = 0;
extern uint8_t dir;
extern uint16_t Last_Status;
extern uint8_t Motor_Status;
extern uint16_t Main_Status;
extern uint16_t Memo_Status;
void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	PWM_Init();
	
	//直流电机总控制引脚STBY
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void Motor_SetSpeed(uint16_t Motor_Speed)
{
	uint16_t Speed_CCR;
	Speed_CCR = 7.2 * Motor_Speed;//720 - 7.2 * Motor_Speed;
	
/*	if (dir == 0)
	{
		PWM_SetCompare3_Positive();
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		GPIO_SetBits(GPIOA, GPIO_Pin_4);			
	}*/
	PWM_SetCompare3((uint16_t)Speed_CCR);
}

void Motor_Off(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
}

void Motor_On(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
	
}

