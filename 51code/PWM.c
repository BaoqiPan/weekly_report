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
	TIMBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//ʱ�ӷ�Ƶ
	TIMBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIMBaseInitStructure.TIM_Period=100-1;//ARR�Զ���װ����ֵ
	TIMBaseInitStructure.TIM_Prescaler=720-1;//PSCԤ��Ƶ����ֵ
	TIMBaseInitStructure.TIM_RepetitionCounter=0;//�ظ���������ֵ
	TIM_TimeBaseInit(TIM2,&TIMBaseInitStructure);
	
	//��ʼ���Ƚϵ�Ԫ
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
//	TIM_OCInitStructure.TIM_OCIdleState=;���ֻ�и߼���ʱ�������õ�
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//��������Ƚϵ�ģʽ
//	TIM_OCInitStructure.TIM_OCNIdleState=;���ֻ�и߼���ʱ�������õ�
//	TIM_OCInitStructure.TIM_OCNPolarity=;���ֻ�и߼���ʱ�������õ�
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//��������Ƚϵļ���
//	TIM_OCInitStructure.TIM_OutputNState=;���ֻ�и߼���ʱ�������õ�
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//�������ʹ��
	TIM_OCInitStructure.TIM_Pulse=0;//�������������CCR��,����ȽϼĴ���
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);
	
	
	TIM_Cmd(TIM2,ENABLE);
	
}

void PWM_SetCompare1(uint16_t Compare)
{
	
	TIM_SetCompare1(TIM2,Compare);
}

