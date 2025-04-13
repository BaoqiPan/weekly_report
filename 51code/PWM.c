#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIMBaseInitStructure;
	TIMBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//时钟分频
	TIMBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIMBaseInitStructure.TIM_Period=100-1;//ARR自动重装器的值
	TIMBaseInitStructure.TIM_Prescaler=720-1;//PSC预分频器的值
	TIMBaseInitStructure.TIM_RepetitionCounter=0;//重复计数器的值
	TIM_TimeBaseInit(TIM2,&TIMBaseInitStructure);
	
	//初始化比较单元
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
//	TIM_OCInitStructure.TIM_OCIdleState=;这个只有高级定时器才能用到
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//设置输出比较的模式
//	TIM_OCInitStructure.TIM_OCNIdleState=;这个只有高级定时器才能用到
//	TIM_OCInitStructure.TIM_OCNPolarity=;这个只有高级定时器才能用到
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//设置输出比较的极性
//	TIM_OCInitStructure.TIM_OutputNState=;这个只有高级定时器才能用到
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//设置输出使能
	TIM_OCInitStructure.TIM_Pulse=0;//这个是用来设置CCR的,捕获比较寄存器
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);
	
	
	TIM_Cmd(TIM2,ENABLE);
	
}

void PWM_SetCompare1(uint16_t Compare)
{
	
	TIM_SetCompare1(TIM2,Compare);
}

