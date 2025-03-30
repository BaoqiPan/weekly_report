#include "stm32f10x.h"                  // Device header


void Timer_Init(void)
{
	//第一步：开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	
	//第二步：选择时基单元的时钟
	TIM_InternalClockConfig(TIM2);
	//由于定时器上电后，默认就是使用内部时钟，所以可写可不写
	
	
	//第三步：初始化时基单元
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_InitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//内部时钟分频
	TIM_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
	TIM_InitStructure.TIM_Period=10000-1;//配置ARR自动重装的值
	TIM_InitStructure.TIM_Prescaler=7200-1;//代表PSC预分频器的值
	TIM_InitStructure.TIM_RepetitionCounter=0;//高级，重复计数器的值，不用，所以给0就行
	TIM_TimeBaseInit(TIM2,&TIM_InitStructure);
	
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//清除一下更新中断标志位
	
	
	//第四步，使能更新中断，或者说配置输出中断控制
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	
	//第五步，配置NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	
	
	//第六步，启动定时器
	TIM_Cmd(TIM2,ENABLE);
}


/*void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
	{
		
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}*/
