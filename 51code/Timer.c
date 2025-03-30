#include "stm32f10x.h"                  // Device header


void Timer_Init(void)
{
	//��һ��������ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	
	//�ڶ�����ѡ��ʱ����Ԫ��ʱ��
	TIM_InternalClockConfig(TIM2);
	//���ڶ�ʱ���ϵ��Ĭ�Ͼ���ʹ���ڲ�ʱ�ӣ����Կ�д�ɲ�д
	
	
	//����������ʼ��ʱ����Ԫ
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_InitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//�ڲ�ʱ�ӷ�Ƶ
	TIM_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���
	TIM_InitStructure.TIM_Period=10000-1;//����ARR�Զ���װ��ֵ
	TIM_InitStructure.TIM_Prescaler=7200-1;//����PSCԤ��Ƶ����ֵ
	TIM_InitStructure.TIM_RepetitionCounter=0;//�߼����ظ���������ֵ�����ã����Ը�0����
	TIM_TimeBaseInit(TIM2,&TIM_InitStructure);
	
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//���һ�¸����жϱ�־λ
	
	
	//���Ĳ���ʹ�ܸ����жϣ�����˵��������жϿ���
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	
	//���岽������NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	
	
	//��������������ʱ��
	TIM_Cmd(TIM2,ENABLE);
}


/*void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
	{
		
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}*/
