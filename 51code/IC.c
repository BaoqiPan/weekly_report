#include "stm32f10x.h"                  // Device header


void IC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIMBaseInitStructure;
	TIMBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//ʱ�ӷ�Ƶ
	TIMBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIMBaseInitStructure.TIM_Period=65536-1;//ARR�Զ���װ����ֵ
	TIMBaseInitStructure.TIM_Prescaler=72-1;//PSCԤ��Ƶ����ֵ
	TIMBaseInitStructure.TIM_RepetitionCounter=0;//�ظ���������ֵ
	TIM_TimeBaseInit(TIM3,&TIMBaseInitStructure);
	
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;//ѡ��ͨ���Ĳ���
	TIM_ICInitStructure.TIM_ICFilter=0xf;//����ѡ�����벶����˲�����ë���źź��������˳���
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;//����ѡ�������ػ����½��أ�
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;//��Ƶ��
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;//ѡ���źŴ��ĸ���������
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	
	TIM_SelectInputTrigger(TIM3,TIM_TS_TI1FP1);
	TIM_SelectSlaveMode(TIM3,TIM_SlaveMode_Reset);
	
	TIM_Cmd(TIM3,ENABLE);//������ʱ��
}

uint32_t IC_GetFreq(void)
{
	return 1000000/(TIM_GetCapture1(TIM3)+1);//��1��Ϊ�˴������
}
