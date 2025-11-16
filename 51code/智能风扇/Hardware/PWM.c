#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIMBaseInitStructure;
	TIMBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIMBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIMBaseInitStructure.TIM_Period = 720; //ARR
	TIMBaseInitStructure.TIM_Prescaler = 100;//PSC
	TIMBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIMBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;//CCR
	TIM_OC3Init(TIM2,&TIM_OCInitStructure);
	
	TIM_Cmd(TIM2, ENABLE);
	
}

void PWM_SetCompare3_Positive(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	GPIO_SetBits(GPIOA, GPIO_Pin_5);	
}

void PWM_SetCompare3(uint16_t Compare)
{
//				PWM_SetCompare3(500);
	TIM_SetCompare3(TIM2, Compare);
}

