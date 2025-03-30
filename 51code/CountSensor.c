#include "stm32f10x.h"                  // Device header
#include "delay.h"

uint16_t CountSensor_Count;


void CountSensor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//第一步：打开时钟外设
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);
	//当执行完这个函数之后，AFIO的第14个数据选择器就拨好了
	//其中输入端被拨到GPIOB外设上，对应的就是PB14号引脚
	//输出端固定连接的是EXTI的第14个中断线路
	//这样PB14号引脚的电平信号就可以顺利通过AFIO，进入后级EXTI电路了
	
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line=EXTI_Line14;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//按理来说，这个分组方式，整个芯片只能用一种，所以按理说
	//这个分组的代码整个工程只需要执行一次就行了
	//如果把它放在模块里进行分组，要确保每个模块分组都是选的同一个
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
}

uint16_t CountSensor_Get(void)
{
	return CountSensor_Count;
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line14)==SET)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==0)
		{
			CountSensor_Count++;
			Delay_ms(100);
			//由于硬件原因，需要配置为下拉输入，延时并且判断14口的电平，共三处修改，才能尽可能正确
		}

		EXTI_ClearITPendingBit(EXTI_Line14);		
	}

}
